
#include "LocalGameLoop.hpp"
#include "MapGenerator.hpp"
#include "ImageLoader.hpp"

LocalGameLoop::LocalGameLoop(Render* renderer) : render{renderer} {}

bool LocalGameLoop::init() {

	// Solve left over bullet on new game issue
	if(render->gProjectiles.size() > 0) {
		render->gProjectiles.clear();
	}

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
    pinksplosion->sheetSetup(42, 42, 6);

    // Set up enemy
    enemy_tank = new Sprite(render->getRenderer(), "src/res/images/blue_tank.png");
	enemy_tank->init();
	enemy_tank->sheetSetup(30, 30, 3);
    enemy_turret = new Sprite(render-> getRenderer(), "src/res/images/blue_turret.png");
	enemy_turret->init();

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
			SDL_Rect hole_tile = { x+5, y+5, TILE_SIZE-5, TILE_SIZE-5 }; //does not work, enemy AI needs update
			if(mapVectors[i][j] == 2){
				tileArray.push_back(cur_out);
				enemyTileArray.push_back(cur_out);
				projectileObstacles.push_back(cur_out);
			}
			else if(mapVectors[i][j] == 1){
				tileArray.push_back(hole_tile);
				enemyTileArray.push_back(cur_out);
			}
		}
	}

    player->setObstacleLocations(&tileArray);

    std::vector<int> enemySpawn = spawnEnemies(map, 1);
	enemies.push_back(new Enemy(enemySpawn.at(0), enemySpawn.at(1), player));

    render->setEnemies(enemies);

    for (auto enemy : enemies) {
		enemy->setSprite(enemy_tank);
		enemy->setTurretSprite(enemy_turret);
        enemy->setObstacleLocations(&tileArray);
		enemy->setTileMap(map);
	}	
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
	// SET TO TRUE TO DEBUG
	std::vector<std::vector<int>> transpose;
	if(false)
	{
		std::cout << "LocalGameLoop::spawnEnemies()" << std::endl;

		for(int i = 0; i < Y_HIGH; i++)
		{
			transpose.push_back(std::vector<int>(X_WIDE));
			for(int j = 0; j < X_WIDE; j++)
			{
				transpose[i][j] = (*map)[j][i];
			}
		}

		for( auto i : transpose )
		{
			for( auto j : i )
				std::cout << j;
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}

	std::vector<std::vector<int>> tileMap = *map;
	std::vector<int> coords;
	int enemy_x, enemy_y;
	int i = 0;

	while(true)
	{
		enemy_x = (rand() % 16) + 4;
		enemy_y = (rand() % 3) + 10;

		if(transpose[enemy_y][enemy_x] == 0)
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

	// std::cout << "X" << enemy_x << ":Y" << enemy_y << std::endl;
	// std::cout << "V" << transpose[enemy_y][enemy_x] << std::endl;

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
	timer = 180;
	begin_timer = SDL_GetTicks();

    while(isGameOn) {
        current_time = std::chrono::system_clock::now();
		elapsed_time = current_time - previous_time;
		previous_time = current_time;
		lag_time += elapsed_time.count(); 

        // 1. Process input
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				return -1; // close window
			}
            if (e.key.keysym.sym == SDLK_ESCAPE) 
            {
                return 0; // return to menu
            } 
		}

        player->getEvent(elapsed_time, &e, SDL_GetKeyboardState(nullptr));

        //The player fired a bullet
        if (player->getFire() == true) {
            Projectile *newBullet = new Projectile(player->getX() + TANK_WIDTH/4, player->getY() + TANK_HEIGHT/4, player->getTurretTheta());
            newBullet->setSprite(shell);
            newBullet->setObstacleLocations(&projectileObstacles);
            projectiles.push_back(newBullet);
            render->setProjectiles(projectiles);
            player->setFire(false);
        }

        for(auto enemy : enemies) {
			if(enemy->getFire() == true) {
                Projectile *newBullet = new Projectile(enemy->getX() + TANK_WIDTH/4, enemy->getY() + TANK_HEIGHT/4, enemy->getTurretTheta());
                newBullet->setSprite(shell);
                newBullet->setObstacleLocations(&projectileObstacles);
                projectiles.push_back(newBullet);
				render->setProjectiles(projectiles);
				enemy->setFire(false);
			}
		}

        // 2. Update
		// Update if time since last update is >= MS_PER_UPDATE
		while(lag_time >= MS_PER_UPDATE) {
			
            player->setTurretTheta();
            player->update();

			for (auto enemy: enemies) {
				enemy->update();
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

        // Draw the cursor on the screen
        int cursorX = 0, cursorY = 0;
		if(e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN) {
			SDL_GetMouseState(&cursorX, &cursorY);
		}

		//Update current timer
		current_timer = SDL_GetTicks();
		timer = 180 - ((current_timer - begin_timer) / 1000);
		render->setTimer(timer);
		//std::cout << "timer : " << timer;

		SDL_Rect cursorRect = {cursorX, cursorY, CROSSHAIR_SIZE, CROSSHAIR_SIZE};

        // 3. Render
		// Render everything
		render->draw(lag_time / MS_PER_UPDATE);
		SDL_RenderCopy(render->getRenderer(), cursor, NULL, &cursorRect);
	}

	// Exit normally
	return 0;
}
