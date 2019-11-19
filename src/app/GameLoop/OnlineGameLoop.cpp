#include "OnlineGameLoop.hpp"
#include "ImageLoader.hpp"
#include <signal.h>


OnlineGameLoop::OnlineGameLoop(Render* renderer) : render{renderer} {}

bool OnlineGameLoop::init(Args* options) {
    // Set up player 1
    Player* player = new Player(0, 0, true); // Position set from server
    Sprite *red_player_tank = new Sprite(render->getRenderer(), "src/res/images/red_tank.png");
	red_player_tank->init();
	red_player_tank->sheetSetup(30, 30, 3);

    Sprite *player1_turret = new Sprite(render->getRenderer(), "src/res/images/red_turret.png");
	player1_turret->init();
	player->setSprite(red_player_tank);
	player->setTurretSprite(player1_turret);

    // Set up player 2
    Player* player2 = new Player(0, 0, false); // Position set from server
	Sprite* blue_player_tank = new Sprite(render->getRenderer(), "src/res/images/blue_tank.png");
	blue_player_tank->init();
    blue_player_tank->sheetSetup(30, 30, 3);

    Sprite *player2_turrent = new Sprite(render->getRenderer(), "src/res/images/blue_turret.png");
	player2_turrent->init();

	player2->setSprite(blue_player_tank);
	player2->setTurretSprite(player2_turrent);

	// Create client and initialize
	client = new Client(options->ip, options->port);
	client->init();

    // Add player and network players to render in vector format
    players.push_back(player);
	playerEnemies.push_back(player2);

	render->setPlayer(players);
	render->setPlayerEnemies(playerEnemies);

    // Set up bullet
    bullet = new Sprite(render->getRenderer(), "src/res/images/bullet.png");
	bullet->init();
	shell = new Sprite(render->getRenderer(), "src/res/images/shell.png");
	shell->init();
	pinksplosion = new Sprite(render->getRenderer(), "src/res/images/pinksplosion.png");
    pinksplosion->init();
    pinksplosion->sheetSetup(42, 42, 6);

    // Set up loading screens
    loadingScreen1 = loadImage("src/res/images/loadingScreen1.png", render->getRenderer());
    loadingScreen2 = loadImage("src/res/images/loadingScreen2.png", render->getRenderer());
    loadingScreen3 = loadImage("src/res/images/loadingScreen3.png", render->getRenderer());
    loadingScreen4 = loadImage("src/res/images/loadingScreen4.png", render->getRenderer());
    loadingScreen5 = loadImage("src/res/images/loadingScreen5.png", render->getRenderer());
    loadingScreen6 = loadImage("src/res/images/loadingScreen6.png", render->getRenderer());
    loadingScreen7 = loadImage("src/res/images/loadingScreen7.png", render->getRenderer());

    // Set up cursor
    cursor = loadImage("src/res/images/cursor.png", render->getRenderer());

    // Host-Network initialization
    std::cout << options->isHost << std::endl;
	if(options->isHost == true){
		int pid = fork();
		if(pid == 0) {
			//child process
			std::vector<char *> args;

			args.push_back("build/bin/ServerProcess");
			args.push_back((char *)options->ip.c_str());
			args.push_back((char *)std::to_string(options->port).c_str());
			args.push_back(NULL);

			std::cout << args[0] << " | " << args[1] << " | " << args[2] << std::endl;

			if (execvp(args[0], args.data()) == -1) {
				//this line should not run!
				std::cout << "execvp failed: " << strerror(errno) << std::endl;
				exit(-1);
			} else {
				// exit normally
				exit(0);
			}
		} else { // Parent
			server_pid = pid;
			std::cout << "Created server process " << server_pid << std::endl;
		}
	}

	// Get initial position of the players from the server
	std::cout << "Waiting on init" << std::endl;
	int screenCounter = 0;
	while(!client->pollInitData()) {
		displayLoadingScreen(screenCounter);
        screenCounter++;
        if(screenCounter == 7) {
            screenCounter = 0;
        }

	}
	auto initData = client->initData;
	player->setId(initData[0]);
	player->setX(initData[1]);
	player->setY(initData[2]);
	player2->setX(initData[3]);
	player2->setY(initData[4]);

    buildMap();

	// Initialized successfully
	return true;	
}

void OnlineGameLoop::buildMap() {
	//wait for both players to connect (Map data arrives when both connect)
    int screenCounter = 0;
    while(!client->pollMap()) {}

	std::vector<std::vector<int>> map2D;
	// init the first row
	map2D.push_back(std::vector<int>((SCREEN_WIDTH - BORDER_GAP - TILE_SIZE) / TILE_SIZE - 1));
	int row = 0;
	int col = 0;
	for (auto tile : client->gameMap) {
		if(col == (SCREEN_HEIGHT - TILE_SIZE) / TILE_SIZE - 1) {
			row++;
			col = 0;
			map2D.push_back(std::vector<int>((SCREEN_WIDTH - BORDER_GAP - TILE_SIZE) / TILE_SIZE - 1));
		}
		map2D[row][col] = tile;
		col++;
	}

	int count = 0;
	for (int x = BORDER_GAP + TILE_SIZE, i = 0; x < SCREEN_WIDTH - BORDER_GAP - TILE_SIZE; x+=TILE_SIZE, i++) {
		for (int y = TILE_SIZE, j = 0; y < SCREEN_HEIGHT - TILE_SIZE; y+=TILE_SIZE, j++) {
			SDL_Rect cur_out = { x, y, TILE_SIZE, TILE_SIZE};
			if(map2D[i][j] == 2){
				tileArray.push_back(cur_out);
				projectileObstacles.push_back(cur_out);
			}
		}
	}

	render->setTileMap(&map2D);

	for (auto player : players) {
		player->setObstacleLocations(&tileArray);
	}

	// Set collision for network players 
	for (auto enemy : playerEnemies) {
		enemy->setObstacleLocations(&tileArray);
	}
}

int OnlineGameLoop::run() {
    SDL_Event e;
	previous_time = std::chrono::system_clock::now(); // get current time of system
	lag_time = 0.0;	// Set duration of time to 0
	
	const Uint8 *keystate;
	const Uint8 *keyStatePacket;
	int temp = 0; // For calculating tickrate TODO change later
    // Main game loop
    while (client->gameOn)
	{
		current_time = std::chrono::system_clock::now();
		elapsed_time = current_time - previous_time;
		previous_time = current_time;
		lag_time += elapsed_time.count();

		// 1. Process input
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				client->gameOn = false;
				// Kill server/client thread
				std::cout << "Killing server process " << server_pid << std::endl;
				kill(server_pid, SIGTERM);
                return -1; // close window
			}
            if (e.key.keysym.sym == SDLK_ESCAPE) 
            {
                return 0; // return to menu
            } 
		}

		assert(players.size() == 1);
		for(auto player : players) {
			// Send same keystate to player object and to the client to send
			// TODO find a good rate to send player keystates
			keystate = SDL_GetKeyboardState(nullptr);
			player->getEvent(elapsed_time, &e, keystate);
			
			//network version of player firing bullet
			if (player->getFire() == true) {
                Projectile *newBullet = new Projectile(player->getX() + TANK_WIDTH/4, player->getY() + TANK_HEIGHT/4, player->getTurretTheta());
                newBullet->setSprite(shell);
                newBullet->setObstacleLocations(&tileArray);
                projectiles.push_back(newBullet);
                render->setProjectiles(projectiles);
                player->setFire(false);
			}
		}

		// Set inputs of enemy players over network
		for(auto playerEnemy : playerEnemies) {
			// Get the keystates from network
			keyStatePacket = client->getKeyState(0);

			// Apply keysates to the network player
			playerEnemy->getEvent(elapsed_time, &e, keyStatePacket);
		}

		// 2. Update
		// Update if time since last update is >= MS_PER_UPDATE
		while(lag_time >= MS_PER_UPDATE) {
			// Local player
			// TODO change player from vector to single class - localPlayer
			players.at(0)->setTurretTheta();
			players.at(0)->update();

			// Basically add a keyframe every ~2 updates -> 30 times a second
			temp += 1;
			// TODO Consolidate tickrates
			if (temp > 2 && keystate != nullptr) {
				// Add keystate from local player to send
				client->addLocalKeyState(keystate, players.at(0)->turretTheta);
				keystate = nullptr; //only need to send one per update loop
				temp = 0;
			}

			for (auto playerEnemy : playerEnemies) {
				int theta = client->getTurretTheta(0);
				std::cout << "CLIENT: Network player theta is " << theta << std::endl;
				fflush(stdout);
				playerEnemy->setTurretTheta(theta);
				playerEnemy->update();
			}

			int count = 0;
            for (auto projectile : projectiles) {
                projectile->update();
                if(projectile->isExploding()) {
                    projectile->setSprite(pinksplosion);
                } else if(projectile->isFinished()) {
                    projectiles.erase(projectiles.begin() + count);
                    projectile->~Projectile();
                    render->setProjectiles(projectiles);
                }
                count++;
            }
			lag_time -= MS_PER_UPDATE;
		}

		int cursorX = 0, cursorY = 0;
		if(e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN) {
			SDL_GetMouseState(&cursorX, &cursorY);
		}

		SDL_Rect cursorRect = {cursorX, cursorY, CROSSHAIR_SIZE, CROSSHAIR_SIZE};

		// 3. Render
		// Render everything
		render->draw(lag_time / MS_PER_UPDATE);
		SDL_RenderCopy(render->getRenderer(), cursor, NULL, &cursorRect);
	}

	// Exit normally
	return 0;
}

void OnlineGameLoop::displayLoadingScreen(int screenCounter) {
    SDL_Rect loadingScreenRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    switch(screenCounter) {
        case 0:
			std::cout << "display load 1" << std::endl;
            SDL_RenderCopy(render->getRenderer(), loadingScreen1, NULL, &loadingScreenRect);
            break;
        case 1:
            SDL_RenderCopy(render->getRenderer(), loadingScreen2, NULL, &loadingScreenRect);
            break;
        case 2:
            SDL_RenderCopy(render->getRenderer(), loadingScreen3, NULL, &loadingScreenRect);
            break;
        case 3:
            SDL_RenderCopy(render->getRenderer(), loadingScreen4, NULL, &loadingScreenRect);
            break;
        case 4:
            SDL_RenderCopy(render->getRenderer(), loadingScreen5, NULL, &loadingScreenRect);
            break;
        case 5:
            SDL_RenderCopy(render->getRenderer(), loadingScreen6, NULL, &loadingScreenRect);
            break;
        case 6:
            SDL_RenderCopy(render->getRenderer(), loadingScreen7, NULL, &loadingScreenRect);
            break;
        default:
            SDL_RenderCopy(render->getRenderer(), loadingScreen1, NULL, &loadingScreenRect);
            break;
    }
	SDL_RenderPresent(render->getRenderer());
    sleep(1);
}