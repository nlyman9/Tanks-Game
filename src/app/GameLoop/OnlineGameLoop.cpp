#include "OnlineGameLoop.hpp"
#include "ImageLoader.hpp"
#include <signal.h>


OnlineGameLoop::OnlineGameLoop(Render* renderer) : render{renderer} {}

int OnlineGameLoop::init(Args* options) {
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

    // Set up bullet
    bullet = new Sprite(render->getRenderer(), "src/res/images/bullet.png");
	bullet->init();
	shell = new Sprite(render->getRenderer(), "src/res/images/shell.png");
	shell->init();
	pinksplosion = new Sprite(render->getRenderer(), "src/res/images/pinksplosion.png");
    pinksplosion->init();
    pinksplosion->sheetSetup(48, 48, 6);
	redsplosion =  new Sprite(render->getRenderer(), "src/res/images/redsplosion.png");
	redsplosion->init();
	redsplosion->sheetSetup(48, 48, 6);
	bluesplosion =  new Sprite(render->getRenderer(), "src/res/images/bluesplosion.png");
	bluesplosion->init();
	bluesplosion->sheetSetup(48, 48, 6);

    // Set up loading screens
    loadingScreen1 = loadImage("src/res/images/loadingScreen1.png", render->getRenderer());
    loadingScreen2 = loadImage("src/res/images/loadingScreen2.png", render->getRenderer());
    loadingScreen3 = loadImage("src/res/images/loadingScreen3.png", render->getRenderer());
    loadingScreen4 = loadImage("src/res/images/loadingScreen4.png", render->getRenderer());
    loadingScreen5 = loadImage("src/res/images/loadingScreen5.png", render->getRenderer());
    loadingScreen6 = loadImage("src/res/images/loadingScreen6.png", render->getRenderer());
    loadingScreen7 = loadImage("src/res/images/loadingScreen7.png", render->getRenderer());

	// Set up bomb
    bombBlack = new Sprite(render->getRenderer(), "src/res/images/bomb_black.png");
    bombBlack->init();
    bombRed = new Sprite(render->getRenderer(), "src/res/images/bomb_red.png");
    bombRed->init();
    bombPlayerExplosion = new Sprite(render->getRenderer(), "src/res/images/redsplosion.png");
    bombPlayerExplosion->init();
    bombPlayerExplosion->sheetSetup(48, 48, 6);
    bombEnemyExplosion = new Sprite(render->getRenderer(), "src/res/images/bluesplosion.png");
    bombEnemyExplosion->init();
    bombEnemyExplosion->sheetSetup(48, 48, 6);

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
	SDL_Event e;
	while(!client->pollInitData()) {
		displayLoadingScreen(screenCounter);
        screenCounter++;
        if(screenCounter == 7) {
            screenCounter = 0;
        }

		// Check if user wants to exit multiplayer or close window while waiting to connect to server
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				client->gameOn = false;
				// Kill server/client thread
				if (server_pid != 0) {
					std::cout << "Killing server process " << server_pid << std::endl;
					kill(server_pid, SIGTERM);
				}
                return -1; // close window
			}
            if (e.key.keysym.sym == SDLK_ESCAPE)
            {
                return 0; // return to menu
            }
		}
	}

	auto initData = client->initData;
	player->setId(initData[0]);
	player->setX(initData[1]);
	player->setY(initData[2]);
	player2->setX(initData[3]);
	player2->setY(initData[4]);

	// Add player and network players to render in vector format
    players.push_back(player);
	playerEnemies.push_back(player2);

	render->setPlayer(players);
	render->setPlayerEnemies(playerEnemies);

	// Initialized successfully
    return buildMap();
}

int OnlineGameLoop::buildMap() {
	//wait for both players to connect (Map data arrives when both connect)
	SDL_Event e;
    while(!client->pollMap()) {
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				client->gameOn = false;
				// Kill server/client thread
				if (server_pid != 0) {
					std::cout << "Killing server process " << server_pid << std::endl;
					kill(server_pid, SIGTERM);
				}
                return -1; // close window
			}
            if (e.key.keysym.sym == SDLK_ESCAPE)
            {
                return 0; // return to menu
            }
		}
	}
	map2D.clear();

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
			if(map2D[i][j] >= 2){
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

	return 1; // Success
}

int OnlineGameLoop::run() {
    SDL_Event e;
	previous_time = std::chrono::system_clock::now(); // get current time of system
	lag_time = 0.0;	// Set duration of time to 0

	const Uint8 *keystate;
	const Uint8 *keyStatePacket;
	int temp = 0; // For calculating tickrate TODO change later
    long gameTimer = TIMER_LENGTH + 2; // +2 to allow for set up time

    // Main game loop
    while (client->gameOn)
	{
		current_time = std::chrono::system_clock::now();
		elapsed_time = current_time - previous_time;
		previous_time = current_time;
		lag_time += elapsed_time.count();

        auto timeSinceStart = current_time.time_since_epoch().count() - client->getStartTime();
        long timeRemaining = gameTimer - timeSinceStart / 1000000000;

		// Don't decriment timer until less than 300
		if(timeRemaining > 300) {
			render->setTimer(300);
		} else {
        	render->setTimer(timeRemaining);
		}

		if(client->gameOver) {
			if(client->win) {
				return 1;
			} else {
				return 2;
			}
		}

		// 1. Process input
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				client->gameOn = false;
				// Kill server/client thread
				if (server_pid != 0) {
					std::cout << "Killing server process " << server_pid << std::endl;
					kill(server_pid, SIGTERM);
				}
                return -1; // close window
			}
            if (e.key.keysym.sym == SDLK_ESCAPE)
            {
                //return 0; // return to menu
            }
		}

		assert(players.size() == 1);
		//apply network gamestate
		if(client->stateSet){
			for(auto player : players) {
				if(client->id == 0){
					player->applyState(client->playerStates->at(0));
				}else{
					player->applyState(client->playerStates->at(1));
				}
			}
			for(auto playerEnemy : playerEnemies) {
				if(client->id == 0){
					playerEnemy->applyState(client->playerStates->at(1));
				}else{
					playerEnemy->applyState(client->playerStates->at(0));
				}			
			}
			client->stateSet = false;
		}

		for(auto player : players) {
			// Send same keystate to player object and to the client to send
			// TODO find a good rate to send player keystates
			if(client->stateSet){}
			keystate = SDL_GetKeyboardState(nullptr);
			player->getEvent(elapsed_time, &e, keystate);
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
			
			for (auto playerEnemy : playerEnemies) {
				playerEnemy->setTurretTheta(client->getTurretTheta(0));
				playerEnemy->setFireNOCD(client->getPlayerShot(0));
				playerEnemy->setBombNOCD(client->getPlayerBomb(0));
				playerEnemy->update();
			}

			// Basically add a keyframe every ~2 updates -> 30 times a second
			temp++;
			// TODO Consolidate tickrates
			if (temp > 2 && keystate != nullptr) {
				// Add keystate from local player to send
				client->addLocalKeyState(keystate, players.at(0)->turretTheta, players.at(0)->getFire(), players.at(0)->getBomb());
				keystate = nullptr; //only need to send one per update loop
				temp = 0;

				// network version of player firing bullet
				// TODO NOT DO THIS - not have this nested inside the tickrate - this is hacky
				if (players.at(0)->getFire() == true) {
					Projectile *newBullet = new Projectile(players.at(0)->getX() + TANK_WIDTH/4, players.at(0)->getY() + TANK_HEIGHT/4, players.at(0)->getTurretTheta(), 1);
					newBullet->setSprite(shell);
					newBullet->setObstacleLocations(&tileArray);
					newBullet->setID(players.at(0)->getId());
					newBullet->setTileArray(render->getTileMap());
					newBullet->setObstacleLocations(&projectileObstacles);
					projectiles.push_back(newBullet);
					render->setProjectiles(projectiles);
				}

				if(players.at(0)->getBomb() == true) {
					Bomb* newBomb(new Bomb(players.at(0)->get_box(), players.at(0)->getTheta(), bombBlack, bombRed, bombPlayerExplosion));
					bombs.push_back(newBomb);
					render->setBombs(bombs);
				}

				for (auto playerEnemy : playerEnemies) {
					playerEnemy->setTurretTheta(client->getTurretTheta(0));
					playerEnemy->setFire(client->getPlayerShot(0));
					playerEnemy->setBomb(client->getPlayerBomb(0));
					playerEnemy->update();

					if (playerEnemy->getFire() == true) {
						Projectile *newBullet = new Projectile(playerEnemy->getX() + TANK_WIDTH/4, playerEnemy->getY() + TANK_HEIGHT/4, playerEnemy->getTurretTheta(), 1);
						newBullet->setSprite(shell);
						newBullet->setObstacleLocations(&tileArray);
						newBullet->setID(playerEnemy->getId());
						newBullet->setTileArray(render->getTileMap());
						newBullet->setObstacleLocations(&projectileObstacles);
						projectiles.push_back(newBullet);
						render->setProjectiles(projectiles);
						playerEnemy->setFire(false);
					}

					if(playerEnemy->getBomb() == true) {
						Bomb* newBomb = new Bomb(playerEnemy->get_box(), playerEnemy->getTheta(), bombBlack, bombRed, bombPlayerExplosion);
						bombs.push_back(newBomb);
						render->setBombs(bombs);
						playerEnemy->setBomb(false);
					}
				}
				players.at(0)->setFire(false);
				playerEnemies.at(0)->setFire(false);
				players.at(0)->setBomb(false);
				playerEnemies.at(0)->setBomb(false);
			}

			// Update every active projectile
            int count = 0;
            for (int i = 0; i < projectiles.size(); i++) {
				projectiles.at(i)->clearTargets();
				if(projectiles.at(i)->getID() == players.at(0)->getId()) {
					for(auto enemy : playerEnemies) {
						projectiles.at(i)->addTargetLocation(enemy->get_box());
					}
				} else {
					for(auto player : players) {
						projectiles.at(i)->addTargetLocation(player->get_box());
					}
				}
				projectiles.at(i)->update();

				 // check if hitting destructible terrain
                if(projectiles.at(i)->hasDestructCollision())
                {
                    std::vector<std::vector<int>> tile_map = render->getTileMap();
                    int xIndex = projectiles.at(i)->getColX();
                    int yIndex = projectiles.at(i)->getColY();
                    int currValue = tile_map[xIndex][yIndex];

                    if(currValue == 4)
                    {
                        tile_map[xIndex][yIndex] = 3;
                    }
                    else
                    {
                        tile_map[xIndex][yIndex] = 0;
                    }

                    render->setTileMap(&tile_map);

                    projectiles.at(i)->setExploding(true);

                    // update tile arrays
                    client->updateMap(tile_map);
					updateObstacleArrays(tile_map);
                }

				if(projectiles.at(i)->isHit()) {
					SDL_Rect* hitObject = projectiles.at(i)->getTarget();
					for(auto player : players) {
						SDL_Rect* playerRect = player->get_box();
					
						if(playerRect->x == hitObject->x && playerRect->y == hitObject->y && !player->isHit()) {
							player->setHit(true);
							client->sendGameOver(client->id);
							player->setSprite(redsplosion);
							player->resetFrame();
						}
					}
					int count = 0;
					for(auto enemy: playerEnemies) {
						SDL_Rect* enemyRect = enemy->get_box();
						if(enemyRect->x == hitObject->x && enemyRect->y == hitObject->y && !enemy->isHit()) {
							enemy->setHit(true);
							int losingId;
							if(client->id == 0) {
								losingId = 1;
							} else {
								losingId = 0;
							}
							client->sendGameOver(losingId);
							enemy->resetFrame();
							break;
						}
					}
				}

				if(projectiles.at(i)->isExploding()) {
					projectiles.at(i)->setSprite(pinksplosion);
					projectiles.at(i)->setFinished(true);
				}
                if(projectiles.at(i)->isFinished()){
					projectiles.erase(projectiles.begin()+i);
					render->setProjectiles(projectiles);
					i--;
				}
				count++;
			}

			// Update every bomb in the game
			int bombCount = 0;
			for(auto& bomb : bombs) {
				// Update bombs
				bomb->update();

				std::vector<std::vector<int>> tile_map = render->getTileMap();
				if(bomb->isExploding()) {
					 // Check if bomb blew up a destructable tile
                    int xIndex = (bomb->getX() + BOMB_WIDTH / 2 - BORDER_GAP) / TILE_SIZE - 1;
                    int yIndex = (bomb->getY() + BOMB_HEIGHT / 2) / TILE_SIZE - 1;
                    int currValue = tile_map[xIndex][yIndex];
                    
                    if(currValue == 4)
                    {
                        tile_map[xIndex][yIndex] = 3;
                    }
                    if(currValue == 3)
                    {
                        tile_map[xIndex][yIndex] = 0;
                    }

                    render->setTileMap(&tile_map);

                    // update tile arrays
                    updateObstacleArrays(tile_map);
				}

				// Check if bomb is done exploding
				if(bomb->getFinished()) {
					bombs.erase(bombs.begin() + bombCount);
					render->setBombs(bombs);
					bombCount--;
					client->updateMap(tile_map);
				}
				
				bombCount++;
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

// Updates player, enemies, and projectiles obstacle arrays
// Called when destroying destructible terrain
void OnlineGameLoop::updateObstacleArrays(std::vector<std::vector<int>> tile_map)
{
    std::vector<SDL_Rect> player_obstacle_array;
    std::vector<SDL_Rect> enemy_obstacle_array;
    std::vector<SDL_Rect> projectile_obstacle_array;

    for (int x = BORDER_GAP + TILE_SIZE, i = 0; x < SCREEN_WIDTH - BORDER_GAP - TILE_SIZE; x+=TILE_SIZE, i++) {
        for (int y = TILE_SIZE, j = 0; y < SCREEN_HEIGHT - TILE_SIZE; y+=TILE_SIZE, j++) {
            SDL_Rect normal_tile = { x, y, TILE_SIZE, TILE_SIZE};
            SDL_Rect hole_tile = { x+5, y+5, TILE_SIZE-5, TILE_SIZE-5 }; //does not work, enemy AI needs update
            if(tile_map[i][j] >= 2) {
                player_obstacle_array.push_back(normal_tile);
                enemy_obstacle_array.push_back(normal_tile);
                projectile_obstacle_array.push_back(normal_tile);
            } else if(tile_map[i][j] == 1) {
                player_obstacle_array.push_back(hole_tile);
                enemy_obstacle_array.push_back(normal_tile);
            }
        }
    }

	for(auto player : players){
    	player->setObstacleLocations(&player_obstacle_array);
	}

    for(auto enemy : playerEnemies)
    {
        enemy->setObstacleLocations(&enemy_obstacle_array);
    }

    for(auto projectile : projectiles)
    {
        projectile->setObstacleLocations(&projectile_obstacle_array);
    }

    tileArray = player_obstacle_array;
    projectileObstacles = projectile_obstacle_array;
}