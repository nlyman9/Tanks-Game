#include "LocalGameLoop.hpp"
#include "MapGenerator.hpp"
#include "ImageLoader.hpp"

LocalGameLoop::LocalGameLoop(Render* renderer) : render{renderer} {}

bool LocalGameLoop::init() {

    // Solve left over bullet and bomb on new game issue
    render->clear();

    // Set up player
    player = new Player(SCREEN_WIDTH/2 + 100, 50, true);
    Sprite *player_tank = new Sprite(render->getRenderer(), "src/res/images/red_tank.png");
    player_tank->init();
    player_tank->sheetSetup(30, 30, 3);
    Sprite *player_turrent = new Sprite(render->getRenderer(), "src/res/images/red_turret.png");
    player_turrent->init();
    player->setSprite(player_tank);
    player->setTurretSprite(player_turrent);

    // Add player to render in vector format
    std::vector<Player*> playerVector;
    playerVector.push_back(player);
    render->setPlayer(playerVector);

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
	greensplosion =  new Sprite(render->getRenderer(), "src/res/images/greensplosion.png");
    greensplosion->init();
    greensplosion->sheetSetup(48, 48, 6);
	purplesplosion =  new Sprite(render->getRenderer(), "src/res/images/purplesplosion.png");
    purplesplosion->init();
    purplesplosion->sheetSetup(48, 48, 6);
	whitesplosion =  new Sprite(render->getRenderer(), "src/res/images/whitesplosion.png");
    whitesplosion->init();
    whitesplosion->sheetSetup(48, 48, 6);


    // Set up enemy
    enemy_tank_blue = new Sprite(render->getRenderer(), "src/res/images/blue_tank.png");
    enemy_tank_blue->init();
    enemy_tank_blue->sheetSetup(30, 30, 3);
    enemy_tank_green = new Sprite(render->getRenderer(), "src/res/images/green_tank.png");
    enemy_tank_green->init();
    enemy_tank_green->sheetSetup(30, 30, 3);
    enemy_tank_purple = new Sprite(render->getRenderer(), "src/res/images/purple_tank.png");
    enemy_tank_purple->init();
    enemy_tank_purple->sheetSetup(30, 30, 3);
	enemy_tank_spider = new Sprite(render->getRenderer(), "src/res/images/spider_tank.png");
    enemy_tank_spider->init();
    enemy_tank_spider->sheetSetup(48, 48, 8);
    enemy_turret_blue = new Sprite(render-> getRenderer(), "src/res/images/blue_turret.png");
    enemy_turret_blue->init();
    enemy_turret_green = new Sprite(render->getRenderer(), "src/res/images/green_turret.png");
    enemy_turret_green->init();
    enemy_turret_purple = new Sprite(render->getRenderer(), "src/res/images/purple_turret.png");
    enemy_turret_purple->init();
	enemy_turret_spider = new Sprite(render->getRenderer(), "src/res/images/spider_turret.png");
    enemy_turret_spider->init();

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

    isGameOn = true;

    generateMap();

    // Initialized successfully
    return true;
}

void LocalGameLoop::generateMap() {
    MapGenerator* mapGen = new MapGenerator();
    std::vector<std::vector<int>>* map = mapGen->generateMap();
    render->setTileMap(map);
    std::vector<std::vector<int>>& mapVectors = *map;

    int count = 0;
    for (int x = BORDER_GAP + TILE_SIZE, i = 0; x < SCREEN_WIDTH - BORDER_GAP - TILE_SIZE; x+=TILE_SIZE, i++) {
        for (int y = TILE_SIZE, j = 0; y < SCREEN_HEIGHT - TILE_SIZE; y+=TILE_SIZE, j++) {
            SDL_Rect cur_out = { x, y, TILE_SIZE, TILE_SIZE};
            SDL_Rect hole_tile = { x+5, y+5, TILE_SIZE-10, TILE_SIZE-10 }; //does not work, enemy AI needs update
            if(mapVectors[i][j] >= 2) {
                tileArray.push_back(cur_out);
                enemyTileArray.push_back(cur_out);
                projectileObstacles.push_back(cur_out);
            } else if(mapVectors[i][j] == 1) {
                tileArray.push_back(hole_tile);
                enemyTileArray.push_back(cur_out);
            }
        }
    }

    player->setObstacleLocations(&tileArray);

    //spawn random number of enemies between 1 - 4
    //int numEnemies = rand() % 4 + 1;
	int numEnemies = 4;
    for(int i = 0; i < numEnemies; i++){
      int randEnemyType = rand() % 4;
	  //int randEnemyType = 2;
      std::vector<int> enemySpawn = spawnEnemies(map, 1);
      enemies.push_back(new Enemy(enemySpawn.at(0), enemySpawn.at(1), player, randEnemyType));
    }
    //std::vector<int> enemySpawn = spawnEnemies(map, 1);
    //enemies.push_back(new Enemy(enemySpawn.at(0), enemySpawn.at(1), player, 0));

    render->setEnemies(enemies);

    for (auto enemy : enemies) {
			if(enemy->getEnemyType() == 0) {
				enemy->setSprite(enemy_tank_blue);
				enemy->setTurretSprite(enemy_turret_blue);
			}
			else if(enemy->getEnemyType() == 1) {
				enemy->setSprite(enemy_tank_green);
				enemy->setTurretSprite(enemy_turret_green);
			}
			else if(enemy->getEnemyType() == 2) {
				enemy->setSprite(enemy_tank_purple);
				enemy->setTurretSprite(enemy_turret_purple);
			}
			else {
				enemy->setSprite(enemy_tank_spider);
				enemy->setTurretSprite(enemy_turret_spider);
			}
			enemy->setObstacleLocations(&tileArray);
			enemy->setTileMap(map);
			SDL_Rect curEnemy = {(int)enemy->getX(), (int)enemy->getY(), TANK_WIDTH, TANK_HEIGHT};
			enemyBoxes.push_back(&curEnemy);
	}
	player->setEnemies(enemyBoxes);
	enemyBoxes.clear();
}

/**
 * @brief Create an unoccupied spawn location for enemy tanks
 * @param map   Vector representing current map state
 * @param count Total number of enemies spawnpoints to generate
 * @return      Vector of ints with enemy spawnpoints as pixel coordinates, (0=x1, 1=y1, etc)
 * TODO:        Allow to be used for multiple tanks
 */
std::vector<int> LocalGameLoop::spawnEnemies(std::vector<std::vector<int>> *map, int count)
{
    std::vector<std::vector<int>> tileMap = *map;
    std::vector<int> coords;
    int enemy_x, enemy_y;
    int i = 0;

    while(true)
    {
        enemy_x = (rand() % 16) + 4;
        enemy_y = (rand() % 3) + 10;

        if(tileMap[enemy_x][enemy_y] == 0)
        {
            if(i < count)
            {
                i++;
            }
            else
            {
                break;
            }
        }
    }

    coords.push_back(enemy_x * 48 + 64);
    coords.push_back(enemy_y * 48 + 48);

    return coords;
}


// Returns a vector with two int values, x at 0 and y at 1
// Values represent pixel coordinates of enemy spawn point
std::vector<int> LocalGameLoop::spawnEnemy(std::vector<std::vector<int>> *map)
{
    std::vector<std::vector<int>> tileMap = *map;
    std::vector<int> coords;
    int enemy_x, enemy_y;

    while(true)
    {
        enemy_x = (rand() % 16) + 4;
        enemy_y = (rand() % 4) + 9;

        if(tileMap[enemy_y][enemy_x] == 0)
        {
            break;
        }
    }

    coords.push_back(enemy_x * 48 + 100);
    coords.push_back(enemy_y * 48 + 48);

    return coords;
}

int LocalGameLoop::run() {
    SDL_Event e;
    previous_time = std::chrono::system_clock::now(); // get current time of system
    lag_time = 0.0;	// Set duration of time to 0
    long gameTimer = TIMER_LENGTH;
    auto startTime = std::chrono::system_clock::now();

    while(isGameOn) {

        current_time = std::chrono::system_clock::now();
        elapsed_time = current_time - previous_time;
        previous_time = current_time;
        lag_time += elapsed_time.count();

        // Check Game Over Conditions
        auto timeSinceStart = current_time - startTime;
        long timeRemaining = gameTimer - (timeSinceStart.count() / 1000000000);
        render->setTimer(timeRemaining);
        if(enemies.size() == 0) {
            return 1;
        }

        if(player->isHit() || timeRemaining <= 0) {
            return 2;
        }


        // 1. Process input
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                return -1; // close window
            }
            if (e.key.keysym.sym == SDLK_ESCAPE)
            {
                // check that event type is keyboard (768), otherwise mousing
                // over left margin of screen will return to main menu
                if(e.type == 768)
                {
                    return 0; // return to menu
                }
            }
        }

        player->getEvent(elapsed_time, &e, SDL_GetKeyboardState(nullptr));

        //The player fired a bullet

        if (player->getFire() == true) {
            Projectile *newBullet = new Projectile(player->getX() + TANK_WIDTH/4, player->getY() + TANK_HEIGHT/4, player->getTurretTheta(), 1);
            newBullet->setSprite(bullet);
            newBullet->setObstacleLocations(&projectileObstacles);
            newBullet->setFriendly(true);
            newBullet->setTileArray(render->getTileMap());
            for(auto enemy : enemies) {
                newBullet->addTargetLocation(enemy->get_box());
            }
            projectiles.push_back(newBullet);
            render->setProjectiles(projectiles);
            player->setFire(false);
        }


        //The player dropped a bomb
        if (player->getBomb() == true) {
            Bomb* newBomb = new Bomb(player->get_box(), player->getTheta(), bombBlack, bombRed, bombPlayerExplosion);
            bombs.push_back(newBomb);
            render->setBombs(bombs);
            player->setBomb(false);
        }

        // Generate projectile for each enemy if they shot.
        for(auto enemy : enemies) {
            if(enemy->getFire() == true) {

                Projectile *newBullet;
                if(enemy->getEnemyType() == 1) {
					newBullet = new Projectile(enemy->getX() + TANK_WIDTH/4, enemy->getY() + TANK_HEIGHT/4, enemy->getTurretTheta(), 2);
                }
                else if (enemy->getEnemyType() == 2) {
					newBullet = new Projectile(enemy->getX() + TANK_WIDTH/4, enemy->getY() + TANK_HEIGHT/4, enemy->getTurretTheta(), 3);
          newBullet->setBounces(3);
                }
                else {
                    newBullet = new Projectile(enemy->getX() + TANK_WIDTH/4, enemy->getY() + TANK_HEIGHT/4, enemy->getTurretTheta(), 1);
                }
                newBullet->setSprite(shell);
                newBullet->setObstacleLocations(&projectileObstacles);
                newBullet->setFriendly(false);
                newBullet->setTileArray(render->getTileMap());
                newBullet->addTargetLocation(player->get_box());
                projectiles.push_back(newBullet);
                render->setProjectiles(projectiles);
                enemy->setFire(false);

            }
            //The enemy dropped a bomb
            if (enemy->getBomb() == true) {
                Bomb* newBomb = new Bomb(enemy->get_box(), player->getTheta(), bombBlack, bombRed, bombPlayerExplosion);
                newBomb->setSprite(bombBlack);
                bombs.push_back(newBomb);
                render->setBombs(bombs);
                enemy->setBomb(false);
            }
        }

        // 2. Update
        // Update if time since last update is >= MS_PER_UPDATE
        while(lag_time >= MS_PER_UPDATE) {
            player->setTurretTheta();
            player->update();

			if(player->isDestroyed()) {
        //get rid of player and display a you lose screen or something
        //delete player; //This will delete the player, but gets a segfault if you click after it's deleted
				//erase player from render
			}
			for (int i = 0; i < enemies.size(); i++) {
				enemies.at(i)->update();
				enemies.at(i)->setProjectiles(projectiles);
				enemies.at(i)->setEnemies(enemies);
				SDL_Rect* curEnemy = enemies.at(i)->get_box();
				enemyBoxes.push_back(curEnemy);
				if(enemies.at(i)->isDestroyed()) {
					enemies.erase(enemies.begin()+i);
					render->setEnemies(enemies);
				}
			}
			player->setEnemies(enemyBoxes);
			enemyBoxes.clear();

            // Update every active projectile
            int count = 0;
            for (int i = 0; i < projectiles.size(); i++) {
				projectiles.at(i)->clearTargets();
				if(projectiles.at(i)->getFriendly() == true) {
					for(auto enemy : enemies) {
						projectiles.at(i)->addTargetLocation(enemy->get_box());
					}
				} else {
					projectiles.at(i)->addTargetLocation(player->get_box());
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
                    updateObstacleArrays(tile_map);
                }

				if(projectiles.at(i)->isHit()) {
					SDL_Rect* hitObject = projectiles.at(i)->getTarget();
					SDL_Rect* playerRect = player->get_box();
					if(playerRect->x == hitObject->x && playerRect->y == hitObject->y && !player->isHit()) {
						player->setHit(true);

						Explosion* explosion = new Explosion(playerRect->x, playerRect->y, player->getTheta());
						explosion->setSprite(redsplosion);
						explosions.push_back(explosion);
						render->setExplosions(explosions);

						projectiles.at(i)->setFinished(true);
					}

					for(auto enemy: enemies) {
						SDL_Rect* enemyRect = enemy->get_box();
						if(enemyRect->x == hitObject->x && enemyRect->y == hitObject->y && !enemy->isHit()) {
                            if (enemy->getEnemyType() == 2 && !enemy->purpHit()) {
								enemy->setPurpHit(true);
                            } else {
                                enemy->setHit(true);

                                Explosion* explosion = new Explosion(enemyRect->x, enemyRect->y, enemy->getTheta());

								switch(enemy->getEnemyType()) {
									case 0:
										explosion->setSprite(bluesplosion);
										break;
									case 1:
										explosion->setSprite(greensplosion);
										break;
									case 2:
										explosion->setSprite(purplesplosion);
										break;
									case 3:
										explosion->setSprite(whitesplosion);
										break;
								}

								explosions.push_back(explosion);
								render->setExplosions(explosions);
								projectiles.at(i)->setFinished(true);
                            }
							break;
						}
					}
				}
				if(projectiles.at(i)->isExploding()) {
					Explosion* explosion = new Explosion(projectiles.at(i)->getX(), projectiles.at(i)->getY(), projectiles.at(i)->getTheta());
					explosion->setSprite(pinksplosion);
					explosions.push_back(explosion);
					render->setExplosions(explosions);

					projectiles.at(i)->setFinished(true);
				}
                if(projectiles.at(i)->isFinished()){
					projectiles.erase(projectiles.begin()+i);
					render->setProjectiles(projectiles);
					i--;
				}
				count++;
			}

            // Check if bullets are colliding mid-air
			for(auto bullet1 : projectiles){
				for(auto bullet2 : projectiles){
					if(bullet1 != bullet2){
						if(bullet1->projCollisionCheck(bullet2)){
							bullet1->setExploding(true);
							bullet2->setExploding(true);
						}
					}
				}
			}

            // Update every bomb in the game
			int bombCount = 0;
			for(auto& bomb : bombs) {
				// Update bombs
				bomb->update();

				if(bomb->isExploding()) {
                    // Check if bomb blew up a destructable tile
                    std::vector<std::vector<int>> tile_map = render->getTileMap();
                    int xIndex = (bomb->getX() + BOMB_WIDTH / 2 - BORDER_GAP) / TILE_SIZE - 1;
                    int yIndex = (bomb->getY() + BOMB_HEIGHT / 2) / TILE_SIZE - 1;
                    int currValue = tile_map[xIndex][yIndex];
                    //destroy tile bomb is on
                    tile_map[xIndex][yIndex] = 0;
                    //destroy tile above if not border tile
                    if (yIndex-1 != 0) {
                      int tileAbove = tile_map[xIndex][yIndex-1];
                      tile_map[xIndex][yIndex-1] = 0;
                    }
                    //destroy tile below bomb within bounds
                    if (yIndex+1 != 14) {
                      int tileBelow = tile_map[xIndex][yIndex+1];
                      tile_map[xIndex][yIndex+1] = 0;
                    }
                    //destroy tile left
                    if (xIndex-1 != 0) {
                      int tileLeft = tile_map[xIndex-1][yIndex];
                      tile_map[xIndex-1][yIndex] = 0;
                    }
                    //destroy tile right
                    if (xIndex+1 != 24) {
                      int tileRight = tile_map[xIndex+1][yIndex];
                      tile_map[xIndex+1][yIndex] = 0;
                    }
                    /*
                    if(currValue == 4)
                    {
                        tile_map[xIndex][yIndex] = 3;
                    }
                    if(currValue == 3)
                    {
                        tile_map[xIndex][yIndex] = 0;
                    }
                    */
                    render->setTileMap(&tile_map);

                    // update tile arrays
                    updateObstacleArrays(tile_map);

					// Check if bomb hits player
					if(player->check_collision(bomb)) {
						player->setHit(true);
						player->setSprite(redsplosion);
						player->resetFrame();
					}

					// Check if bomb hits enemy
					for(auto enemy: enemies) {
						if(enemy->check_collision(bomb)) {
							enemy->setHit(true);
							enemy->setSprite(bluesplosion);
							enemy->resetFrame();
						}
					}
				}

				// Check if bomb is done exploding
				if(bomb->getFinished()) {
					bombs.erase(bombs.begin() + bombCount);
					render->setBombs(bombs);
					bombCount--;
				}
				bombCount++;
            }

			int explosionCount = 0;
			//update Explosions
			for(auto& explosion : explosions) {
				explosion->update();
				render->setExplosions(explosions);
				if(explosion->isFinished()) {
					explosions.erase(explosions.begin() + explosionCount);
					render->setExplosions(explosions);
					explosionCount--;
				}
				explosionCount++;
			}

            lag_time -= MS_PER_UPDATE;
        }

        // Set position of mouse to draw cursor in render
        if(e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN) {
		    SDL_GetMouseState(&render->cursorX, &render->cursorY);
	    }

        // 3. Render
        // Render everything
        render->draw(lag_time / MS_PER_UPDATE);
    }

    // Exit normally
    // return 0;
}

// Updates player, enemies, and projectiles obstacle arrays
// Called when destroying destructible terrain
void LocalGameLoop::updateObstacleArrays(std::vector<std::vector<int>> tile_map)
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

    player->setObstacleLocations(&player_obstacle_array);

    for(auto enemy : enemies)
    {
        enemy->setObstacleLocations(&enemy_obstacle_array);
        enemy->setTileMap(&tile_map);
    }

    for(auto projectile : projectiles)
    {
        projectile->setObstacleLocations(&projectile_obstacle_array);
    }

    tileArray = player_obstacle_array;
    enemyTileArray = enemy_obstacle_array;
    projectileObstacles = projectile_obstacle_array;
}
