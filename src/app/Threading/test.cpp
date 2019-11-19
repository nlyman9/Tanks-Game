#include "Pool.hpp"
#include <iostream>
static int a = 0;
static int b = 5;
ThreadQueue* q;
int addToB(void*){
  //std::cout << "adding to b" << std::endl;
  b++;
  //std::cout << "a is : " << a << " b is : " << b << std::endl;
  return 0;
}
int addToA(void*){
  //std::cout << "adding to a" << std::endl;
  q->newJob(addToB, NULL);
  a++;
  //std::cout << "a is : " << a << " b is : " << b << std::endl;
  return 0;
}
int mul(void*){
 // std::cout << "multiplying a and b" << std::endl;
  //std::cout << "a is : " << a << " b is : " << b << std::endl;
  int c = a * b;
  //std::cout << "c is : " << c << std::endl;
  return 0;
}

int main(int argc, char* argv[]) {
	q = new ThreadQueue();
  q->newJob(addToA, NULL);
  q->newJob(addToA, NULL);
  q->newJob(addToB, NULL);
  q->newJob(addToB, NULL);
  q->newJob(addToB, NULL);
  q->newJob(mul, NULL);
  q->newJob(mul, NULL);
  q->newJob(addToA, NULL);
  q->newJob(addToB, NULL);
  q->newJob(addToB, NULL);
  q->newJob(addToB, NULL);
  q->newJob(addToA, NULL);
  q->newJob(mul, NULL);
  q->newJob(addToA, NULL);
  for(int i = 0; i < 70000; i++){
    //std::cout << "creating job";
    q->newJob(addToA, NULL);
    //usleep(1);
    //std::cout << "Number of jobs : " << njobs << std::endl;
  };
  std::cout << "Gave all jobs **note** addToA job gives addToB Job" << std::endl;
  
  while(q->getJobs() > 0){
    q->printinfo();
    usleep(5);
  }
  q->printinfo();
  return 0;
}
