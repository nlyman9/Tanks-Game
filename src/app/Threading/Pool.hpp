/*
    Heavily based on the implementation in Naev
    Using the same atomic queue design created by Maged M Michael and Michael L Scott
    Much of the implementation is also based on the code for the CTPL Library
    the ctpl_stl.h and Naev's implementation were very helpful
*/
#ifndef THREADPOOL_H
#define THREADPOOL_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_thread.h>
#include <iostream>
#include <unistd.h>
//how long we wait for a thread to receive work, if it takes too long we sleep that thread
#define TIMEOUT 500
//Signals to check what a worker is doing
#define SIG_STOP 1
#define SIG_RUN 0
//The maximum amount of jobs we can have in a queue at a time
#define MAX_JOBS 50000
//How long we sleep for if we try to add more jobs than the max -- waiting for the queue to decrease
//This is in microseconds
#define SLEEP_TIME 50000
SDL_SpinLock lock = 0;
typedef struct myNode{
    void* data;
    struct myNode *next;
};

typedef struct Queue{
            myNode* head;
            myNode* tail;
            SDL_sem *semaphore;
            SDL_mutex *tlock; //tail lock
            SDL_mutex *hlock; //head lock
};

typedef struct ThreadQueueData{
    int (* function) (void *);
    void *data;
};

typedef struct ThreadData{
    int (* function) (void *);
    void *data;
    int signal;
    SDL_sem *semaphore;
};
static int nJobs = 0;
static int totalJobs = 0;
static int jobsCompleted = 0;
static Queue *global_q = nullptr;
static Queue *idle, *stopped;
class ThreadQueue{
    public:
        //constructs the thread queue
        ThreadQueue(){
            if(global_q != nullptr){
                //queue already initialized
                std::cout << "There is already a queue" << std::endl;
                throw std::runtime_error("A queue already exists -- Cannot have more than one thread queue.");
            }
            nJobs = 0;
            totalJobs = 0;
            jobsCompleted = 0;
            //create global queue
            global_q = q_create();
            //create the threadpool handler
            SDL_CreateThread( threadpool_handler, "threadpool_handler", NULL);
        }
        int newJob( int (*function)(void *), void *data ){
            ThreadQueueData *node;

            if(global_q == NULL){
                //threadpool was not initialized
                std::cout << "Threadpool not initialized" << std::endl;
                return -1;
            }

            node = new ThreadQueueData();
            node->data = data;
            node->function = function;

            enqueue(global_q, node);
            SDL_AtomicLock(&lock);
            nJobs++;
            totalJobs++;
            SDL_AtomicUnlock(&lock);
            return 0;
        }
        void printinfo(){
            std::cout << "current n jobs " << nJobs << " : totaljobs: " << totalJobs << " jobs completed: " << jobsCompleted << std::endl;
        }
        int getJobs(){
            return nJobs;
        }
    private:    
        
        static const int MAXTHREADS = 8;  
        //this is used to initialize the other non-global private queues   
        static Queue* q_create(void){
            myNode *e = new myNode();
            Queue* nq = new Queue();
            e->next = nullptr;
            nq->head = e;
            nq->tail = e;

            nq->tlock = SDL_CreateMutex();
            nq->hlock = SDL_CreateMutex();
            nq->semaphore = SDL_CreateSemaphore( 0 );

            return nq;
        }
        static void enqueue(Queue* inq, void *data){
            myNode *e = new myNode();
            e->data = data;
            e->next = nullptr;
            //lock the tail
            SDL_mutexP(inq->tlock);
            
            inq->tail->next = e;
            inq->tail = e;

            SDL_SemPost(inq->semaphore);
            SDL_mutexV(inq->tlock);
        }
        static void* dequeue(Queue *tq){
            void *d;
            myNode *newHead;
            myNode *node;

            SDL_mutexP(tq->hlock);
            node = tq->head;
            newHead = node->next;

            if(newHead == NULL){
                //queue is empty
                while(newHead == NULL){
                    node = tq->head;
                    newHead = node->next;
                }
            }

            d = newHead->data;
            tq->head = newHead;
            SDL_mutexV(tq->hlock);
            delete node;
            return d;
        }
        static void destroy(Queue *tq)
        {
        while (tq->head->next != NULL)
            delete dequeue(tq);

        SDL_DestroySemaphore( tq->semaphore );
        SDL_DestroyMutex( tq->hlock );
        SDL_DestroyMutex( tq->tlock );

        delete tq->head;
        delete tq;
        }
        static int threadpool_worker(void* data){
            ThreadData *work;
            work = (ThreadData*) data;

            while(work->signal == SIG_RUN){
                while(SDL_SemWait(work->semaphore) == -1){
                    //semwait failure
                    std::cout << "SDL SemWait error " << SDL_GetError() << std::endl;
                }
                if(work->signal == SIG_STOP)
                    break;
                //std::cout << "New job taken on" << std::endl;
                SDL_AtomicLock(&lock);
                jobsCompleted++;
                nJobs--;
                SDL_AtomicUnlock(&lock);
                work->function(work->data); //do the actual function
                //std::cout << "job completed" << std::endl;
                enqueue(idle, work);
            }
            enqueue(stopped, work);
            return 0;
        }
        /*
            This is the main functionality, synchronization of threads.
            It takes a void* data just because threads have to have this signiture, but we do not use it
        */
        static int threadpool_handler(void* data){
            int i, nrunning;
            bool newthread;
            ThreadData *threaddata, *currThread;
            ThreadQueueData *node;

            // Initialize the idle and stopped queues
            idle     = q_create();
            stopped  = q_create();

            // Allocate threaddata to communicate with workers
            threaddata = (ThreadData*) calloc( MAXTHREADS, sizeof(ThreadData) );

            // Initialize data for all the threads 
            for (i=0; i<MAXTHREADS; i++) {
                threaddata[i].function  = NULL;
                threaddata[i].data      = NULL;
                threaddata[i].semaphore = SDL_CreateSemaphore( 0 );
                threaddata[i].signal    = SIG_RUN;
                //enqueue it in the stopped queue
                enqueue( stopped, &threaddata[i] );
            }

            nrunning = 0;

            //Main loop for handing threads
            while (true) {
               /*
                Wait for a new job. If no job arrives within TIMEOUT time, put the thread to sleep
                Once we receive a function, give it to a thread
               */
                if (nrunning > 0) {
                    //waiting for a job, if we timeout, stop the thread and kill it
                    if (SDL_SemWaitTimeout( global_q->semaphore, TIMEOUT ) != 0) {
                        if (SDL_SemTryWait( idle->semaphore ) == 0) {
                            std::cout << "Thread timedout" << std::endl;
                            currThread = (ThreadData *) dequeue( idle );
                            currThread->signal = SIG_STOP;
                            SDL_SemPost( currThread->semaphore );
                            nrunning -= 1;
                        }
                        continue;
                    }
                }
                else {
                    if (SDL_SemWait( global_q->semaphore ) == -1) {
                        std::cout << "SDL SemWait error " << SDL_GetError() << std::endl;
                        continue; //restart the entire loop
                    }
                }
                //At this point we have a new job in the queue!
                node = (ThreadQueueData *) dequeue( global_q );
                newthread = false;
                //check if we have an idle thread
                if (SDL_SemTryWait(idle->semaphore) == 0)
                    currThread = (ThreadData *) dequeue( idle );
                //check if we can restart a thread or make a new one  
                else if (SDL_SemTryWait(stopped->semaphore) == 0) {
                    currThread = (ThreadData *) dequeue( stopped );
                    currThread->signal = SIG_RUN;
                    newthread = true;
                } 
                else {//otherwise wait for an idle thread
                    while (SDL_SemWait(idle->semaphore) == -1) {
                        //sem wait failure
                        std::cout << "SDL SemWait error " << SDL_GetError() << std::endl;
                        //try to continue from here
                    }
                    currThread = (ThreadData *) dequeue( idle );
                }
                //give the thread the next function to do
                currThread->function = node->function;
                currThread->data = node->data;
                //wake up the thread
                SDL_SemPost( currThread->semaphore );

                if (newthread) {
                    SDL_CreateThread( threadpool_worker, "threadpool_worker", currThread );
                    nrunning += 1;
                    std::cout << "Creating a new thread: " << nrunning << std::endl;
                }

                delete node;
            }

            destroy(idle);
            destroy(stopped);
            delete threaddata;

            return 0;
        }
};
#endif