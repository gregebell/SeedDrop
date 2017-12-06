#include "Game.h"
#include "TextureManager.h"
#include "Background.h"
#include "Components.h"
#include "Collision.h"

Background *background;
Manager manager;
SDL_Event Game::event;

std::vector<ColliderComponent*> Game::colliders;

SDL_Renderer* Game::renderer = nullptr; // static global renderer so we dont have to copy it a bunch

auto& seed(manager.addEntity());  // new entity seed, 
auto& game_over(manager.addEntity());
auto& gameStart(manager.addEntity());


Game::Game()
{

}

Game::~Game()
{

}

void Game::init(const char * title, int xpos, int ypos, int width, int height, bool fullscreen)
{
	int flags = 0;

	if (fullscreen)
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}


	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		cout << "SDL Initialized!" << endl;

		// create window
		this->window = SDL_CreateWindow("Game Title", xpos, ypos, width, height, flags);

		if (window) // window successfully made
		{
			cout << "Window successfully created!" << endl;
		}
		else
		{
			cout << "Could not create window: " << SDL_GetError() << endl;
		}

		// create renderer
		this->renderer = SDL_CreateRenderer(window, -1, 0);

		if (this->renderer)
		{
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			cout << "Renderer successfully created!" << endl;
		}
		else
		{
			cout << "Could not create renderer: " << SDL_GetError() << endl;
		}

		this->running = true;
	}
	else 
	{
		cout << "SDL could not initialize! SDL Error: " << SDL_GetError() << endl;
		this->running = false;
	}


	// Entity Component System (ECS) implementation

	seed.addComponent<TransformComponent>(400.0, 150.0, 38, 38, 2.5, 0);
	seed.addComponent<SpriteComponent>("assets/seed38.png", 4, 200);
	seed.addComponent<KeyBoardController>();
	seed.addComponent<ColliderComponent>("seed");
	seed.addGroup(groupPlayers); // add to players group

	createMiddleRoot(-2.0);

	gameStart.addComponent<TransformComponent>(0.0, 0.0, 640, 832, 1, 0);
	gameStart.addComponent<SpriteComponent>("assets/start_screen.png");
	gameStart.addGroup(groupScreens);

	game_over.addComponent < TransformComponent > (0.0, 0.0, 640, 832, 1, 0);
	game_over.addComponent<SpriteComponent>("assets/game_over.png");
	game_over.addGroup(groupScreen2);

	auto& background(manager.addEntity());

	background.addComponent<TransformComponent>(0, 0, 640, 832, 1, 0);
	background.addComponent<SpriteComponent>("assets/background_moving.png", 3, 125);
	background.addGroup(groupMap);
}

void Game::update(int &powerUpTime, int &powerUp2Time, unsigned int &gameTime)
{
	manager.refresh();
	manager.update();


	for (auto& c : colliders) // checks collisions with every object in game
	{

		if (Collision::AABB(seed.getComponent<ColliderComponent>(), *c))
		{
			if (c->collider.x > 0 || c->collider.y > 0)
			{
				if (c->tag == "powerup") // if it hits a powerup
				{

					if (powerUp2Time > 0)
					{
						powerUp2Time = 0;
					}

					powerUpTime = 400;
					seed.getComponent<SpriteComponent>().setTex("assets/seed_leaf38.png");

					actionEffects.powerUp();

					c->entity->destroy();
				}

				else if (c->tag == "sun")
				{
					if (powerUpTime > 0)
					{
						powerUpTime = 0;
					}
					powerUp2Time = 400;
					seed.getComponent<SpriteComponent>().setTex("assets/blinking_seed.png");
					actionEffects.powerUp();

					c->entity->destroy();
				}
				else if (c->tag != "seed" && powerUp2Time == 0) // if anything other than itself is hit
				{
					//manager.freezeEntities();

					 /////////////////////////////////////////////////////////////////////////////////

					actionEffects.objectHit();

					score(gameTime);
					printScore(gameTime);
					this->playerDead = true;
					powerUp2Time = 200;
					powerUpTime = 0;
					seed.getComponent<SpriteComponent>().setTex("assets/blinking_seed.png");
					gameTime = 2;
					break;
				}
			}
		}
	}

	if (powerUpTime > 0)
	{
		powerUpTime--;
	}

	if (powerUp2Time > 0)
	{
		powerUp2Time--;
	}

	if (powerUpTime == 1 || powerUp2Time == 1)
	{
		seed.getComponent<SpriteComponent>().setTex("assets/seed38.png");

		seed.getComponent<SpriteComponent>().setSpeed(100);
	}
	if (powerUp2Time == 100)

	{
		seed.getComponent<SpriteComponent>().setSpeed(70);
	}
	

	
	if (gameTime % 1000 <= 600 && gameTime % 1000 <= 1000)
	{
		if ((gameTime % 144) == 0 || (gameTime % 193) == 0)
		{
			if (gameTime > 0)
			{
				createRock(400, 200);
			}
		}
		if ((gameTime % 163) == 0 || (gameTime % 108) == 0)
		{
			if (gameTime > 0)
			{
				createWorm(400, 200);
			}
		}
		if ((gameTime % 301) == 0 && gameTime > 0)
		{
			createSun(400, 200);
		}
		if ((gameTime % 550) == 0 && gameTime > 0)
		{
			createPowerUp(400, 200);
		}
	}
	else
	{
		if ((gameTime % 100) == 0 && gameTime > 0)
		{
			createRock(100, 350);
		}
		if ((gameTime % 150) == 0 && gameTime > 0)
		{
			createWorm(100, 350);
		}
		if ((gameTime % 300) == 0 && powerUp2Time == 0)
		{
			createSun(100,350);
		}
		if ((gameTime % 400) == 0 && powerUpTime == 0)
		{
			createPowerUp(100, 350);
		}
	}

	createRoot(powerUpTime, gameTime); // combines middle, left, right root creation with velocity increase with time 

	if (gameTime == 1)
	{
		if (Game::event.type == SDL_KEYDOWN)
		{
			gameTime++;
		}
	}
	else
	{
		gameTime++;
	}
}

auto& tiles(manager.getGroup(groupMap));
auto& players(manager.getGroup(groupPlayers));
auto& enemies(manager.getGroup(groupEnemies));
auto& screen1(manager.getGroup(groupScreens));
auto& screen2(manager.getGroup(groupScreen2));
auto& Score(manager.getGroup(groupScore));



void Game::render(unsigned int &gameTime)
{
	SDL_RenderClear(this->renderer);
	// this is where we would add stuff to render

	if (gameTime == 1)
	{
		for (auto& s : screen1)
		{
			s->draw();
			SDL_RenderPresent(this->renderer);
			return;
		}
	}
	else if (this->playerDead == true) // game ended
	{
		for (auto& t : tiles)
		{
			t->draw();
		}


		for (auto& p : players)
		{
			p->draw();
		}

		for (auto& e : enemies)
		{
			e->draw();
		}

		for (auto& s : screen2)
		{
			s->draw();
		}
		for (auto& S : Score)
		{
			S->draw();
		}

		SDL_RenderPresent(this->renderer);

		while (true)
		{
			SDL_PollEvent(&event);

			if (Game::event.type == SDL_KEYDOWN)
			{
				this->playerDead = false; // restart game
				seed.getComponent<TransformComponent>().position.x = 300;
				/*for (auto& S : Score)
				{
					S->deleteGroup(groupScore);
				}*/
				break;
			
			}
		}
		
		for (auto & e : enemies)
		{
			e->destroy();
		}
		return;
	}
	else
	{

		for (auto& t : tiles)
		{
			t->draw();
		}


		for (auto& p : players)
		{
			p->draw();
		}

		for (auto& e : enemies)
		{
			e->draw();
		}
	}

	SDL_RenderPresent(this->renderer);
}

void Game::clean()
{
	SDL_DestroyWindow(this->window);
	SDL_DestroyRenderer(this->renderer);
	SDL_Quit();

	cout << "Game cleaned." << endl;
}

void Game::handleEvents()
{

	SDL_PollEvent(&event);

	switch (event.type)
	{
	case SDL_QUIT:
		this->running = false;
		break;

	default:
		break;
	}
}

bool Game::isRunning()
{
	return this->running;
}

void Game::setRunning(bool running)
{
	// print game over, go to main menu, show score
	this->running = running;
}

void Game::addTile(int id, int x, int y)
{
	auto& tile(manager.addEntity());
	tile.addComponent<TileComponent>(x, y, 64, 64, id);
	tile.getComponent<TransformComponent>().velocity.y = -2;
	tile.addGroup(groupMap);
}
void Game::printScore(int gametime)
{
	if (gametime < 100)
	{
		auto& num1(manager.addEntity());
		const char *Num1 = numb1.c_str();
		num1.addComponent<TransformComponent>(200.0, 380.0, 38, 38, 3, 0);
		num1.addComponent<SpriteComponent>(Num1);
	//	num1.addComponent<ColliderComponent>("score");
		num1.addGroup(groupScore);
	}
	else if (gametime >= 100 && gametime < 1000)
	{
		auto& num1(manager.addEntity());
		auto& num2(manager.addEntity());
		const char *Num1 = numb1.c_str();
		const char *Num2 = numb2.c_str();
		num1.addComponent<TransformComponent>(200.0, 380.0, 38, 38, 3, 0);
		num2.addComponent<TransformComponent>(280.0, 380.0, 38, 38, 3, 0);
		num1.addComponent<SpriteComponent>(Num1);
		num2.addComponent<SpriteComponent>(Num2);
	//	num1.addComponent<ColliderComponent>("score");
	//	num2.addComponent<ColliderComponent>("score");
		num1.addGroup(groupScore);
		num2.addGroup(groupScore);
	}
	else if (gametime >= 1000 )
	{
		auto& num1(manager.addEntity());
		auto& num2(manager.addEntity());
		auto& num3(manager.addEntity());
		const char *Num1 = numb1.c_str();
		const char *Num2 = numb2.c_str();
		const char *Num3 = numb3.c_str();
		
		num1.addComponent<TransformComponent>(150.0, 380.0, 38, 38, 3, 0);
		num2.addComponent<TransformComponent>(230.0, 380.0, 38, 38, 3, 0);
		num3.addComponent<TransformComponent>(310.0, 380.0, 38, 38, 3, 0);
		num1.addComponent<SpriteComponent>(Num1);
		num2.addComponent<SpriteComponent>(Num2);
		num3.addComponent<SpriteComponent>(Num2);
	//	num1.addComponent<ColliderComponent>("score");
	//	num2.addComponent<ColliderComponent>("score");
	//	num3.addComponent<ColliderComponent>("score");
		num1.addGroup(groupScore);
		num2.addGroup(groupScore);
		num3.addGroup(groupScore);
	}
}
void Game::score(int gametime)
{
	int num = 0;
	num = (gametime / 10);
	int n1 = 0, n2 = 0, n3 = 0;
	n1 = num % 10;
	num /= 10;
	n2 = num % 10;
	num /= 10;
	n3 = num % 10;
	num /= 10;

	int list[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	std::string fileList[10] = { "assets/zer.png", "assets/one.png", "assets/two.png", "assets/thr.png", "assets/for.png", "assets/fiv.png", "assets/six.png", "assets/sev.png", "assets/eig.png", "assets/nin.png" };
	for (int i = 0; i <= 10; i++)
	{
		if (n1 == list[i])
		{
			numb1 = fileList[i];
		}
		if (n2 == list[i])
		{
			numb2 = fileList[i];
		}
		if (n3 == list[i])
		{
			numb3 = fileList[i];
		}
	}
	
}
void Game::createRoot(unsigned int gameTime, double velchange, int freq1, int freq2)
{

		for (auto& e : enemies)
		{
			if (!e->hasGroup(groupMap))
			{
				e->getComponent<TransformComponent>().velocity.y = velchange;
			}
		}
		if (gameTime % 1000 > 0 && gameTime % 1000 < 600)
		{
			if (gameTime % freq1 == 0)
			{
				createMiddleRoot(velchange);
			}
		}
		else if (gameTime % 1000 >= 600 && gameTime % 1000 < 700)
		{
			if (gameTime % freq2 == 0)
			{
				createLeftRoot(velchange, gameTime);
			}
		}
		else if (gameTime % 1000 >= 700 && gameTime % 1000 < 800)
		{
			if (gameTime % freq2 == 0)
			{
				createRightRoot(velchange, gameTime);
			}
		}
		else if (gameTime % 1000 >= 800 && gameTime % 1000 < 900)
		{
			if (gameTime % freq2 == 0)
			{
				createLeftRoot(velchange, gameTime);
			}
		}
		else if (gameTime % 1000 >= 900 && gameTime % 1000 < 1000)
		{
			if (gameTime % freq2 == 0)
			{
				createRightRoot(velchange, gameTime);
			}
		}
		
	
}
void Game::createRoot(int powerTime, unsigned int gameTime)
{
	if (powerTime == 0)
	{
		if (gameTime <= 1000)
		{
			createRoot(gameTime, -2.0, 20, 10); //createRoot(gameTime, velocity, frequency of middle roots, frequency of swerve roots)
		}
		else if (gameTime > 1000 && gameTime <= 2000)
		{
			createRoot(gameTime, -3.0, 16, 8);
		}
		else if (gameTime > 2000 && gameTime <= 3000)
		{
			createRoot(gameTime, -4.0, 12, 7);
		}
		else if (gameTime > 3000)
		{
			createRoot(gameTime, -5.0, 8, 5);
		}
	}
	else
	{
		createRoot(gameTime, -1.5, 20, 10);
	}		
}
void Game::createRock(int pos, int pos2)
{
	auto& rock(manager.addEntity());

	float xPos = 0; // must be between 0 and -200?
	float yPos = 0; // between 300 and 640

	xPos = rand() % pos + pos2;
	
	yPos = 670.0;


	rock.addComponent<TransformComponent>(xPos, yPos, 24, 24, 3, 2);
	rock.addComponent<SpriteComponent>("assets/rock.png");
	rock.addComponent<ColliderComponent>("rock");
	rock.addGroup(groupEnemies);
}
void Game::createWorm(int pos, int pos2)
{
	auto& worm(manager.addEntity());

	float xPos = 0; // must be between 0 and -200?
	float yPos = 0; // between 300 and 640

	xPos = rand() % pos + pos2;

	yPos = 670.0;


	worm.addComponent<TransformComponent>(xPos, yPos, 62, 23, 2, 2);
	worm.addComponent<SpriteComponent>("assets/worm.png");
	worm.addComponent<ColliderComponent>("worm");
	worm.addGroup(groupEnemies);
}
void Game::createSun(int pos, int pos2)
{
	auto& sun(manager.addEntity());

	float xPos = 0; // must be between 0 and -200?
	float yPos = 0; // between 300 and 640

	xPos = rand() % pos + pos2;


	yPos = 670.0;


	sun.addComponent<TransformComponent>(xPos, yPos, 32, 32, 2, 5);
	sun.addComponent<SpriteComponent>("assets/sun.png", 5, 200);
	sun.addComponent<ColliderComponent>("sun");
	sun.addGroup(groupEnemies);
}
void Game::createPowerUp(int pos, int pos2)
{
	auto& powerUp(manager.addEntity());

	float xPos = 0; // must be between 0 and -200?
	float yPos = 0; // between 300 and 640

	xPos = rand() % pos + pos2;

	yPos = 670.0;

	powerUp.addComponent<TransformComponent>(xPos, yPos, 32, 32, 2, 5);
	powerUp.addComponent<SpriteComponent>("assets/water_droplet.png", 4, 200);

	powerUp.addComponent<ColliderComponent>("powerup");
	powerUp.addGroup(groupEnemies);
}

void Game::createMiddleRoot(int velocity) // randomly generate roots
{
	auto& rootL(manager.addEntity());
	auto& rootR(manager.addEntity());

	float xPosL = 0; // must be between 0 and -200?
	float yPosL = 650.0; // between 300 and 640

	float xPosR = 0;
	float yPosR = 0;


	xPosL = rand() % 70 -270.0;

	xPosR =rand() % 70 + 730.0;

	yPosR = 650.0;



	rootL.addComponent<TransformComponent>(xPosL, yPosL, 64, 320, 1, velocity);
	rootL.addComponent<SpriteComponent>("assets/root_l64.png");
	rootL.addComponent<ColliderComponent>("root");
	rootL.addGroup(groupEnemies);

	rootR.addComponent<TransformComponent>(xPosR, yPosR, 64, 320, 1, velocity);
	rootR.addComponent<SpriteComponent>("assets/root_r64.png");
	rootR.addComponent<ColliderComponent>("root");
	rootR.addGroup(groupEnemies);
}
void Game::createLeftRoot(int velocity, unsigned int gametime)
{
	auto& rootL(manager.addEntity());
	auto& rootR(manager.addEntity());

	float xPosL = 0; // must be between 0 and -200?
	float yPosL = 650.0; // between 300 and 640

	float xPosR = 0;
	float yPosR = 0;

	if (gametime < 2000)
	{
		xPosL = -50.0 - ((gametime % 100) * 1);
		xPosR = 720.0 - ((gametime % 100) * 1);
		yPosR = 650.0;
	}
	else
	{
		xPosL = -50.0 - ((gametime % 100) * 2);
		xPosR = 720.0 - ((gametime % 100) * 2);
		yPosR = 650.0;
	}


	rootL.addComponent<TransformComponent>(xPosL, yPosL, 64, 320, 1, velocity);

	rootL.addComponent<SpriteComponent>("assets/root_l64.png");
	rootL.addComponent<ColliderComponent>("root");
	rootL.addGroup(groupEnemies);


	rootR.addComponent<TransformComponent>(xPosR, yPosR, 64, 320, 1, velocity);

	rootR.addComponent<SpriteComponent>("assets/root_r64.png");
	rootR.addComponent<ColliderComponent>("root");
	rootR.addGroup(groupEnemies);
}
void Game::createRightRoot(int velocity, unsigned int gametime)
{
	auto& rootL(manager.addEntity());
	auto& rootR(manager.addEntity());

	float xPosL = 0; // must be between 0 and -200?
	float yPosL = 650.0; // between 300 and 640

	float xPosR = 0;
	float yPosR = 0;

	if (gametime < 2000)
	{
		xPosL = -150.0 + ((gametime % 100) * 1);
		xPosR = 620.0 + ((gametime % 100) * 1);
		yPosR = 650.0;
	}
	else
	{
		xPosL = -250.0 + ((gametime % 100) * 2);
		xPosR = 520.0 + ((gametime % 100) * 2);
		yPosR = 650.0;
	}



	rootL.addComponent<TransformComponent>(xPosL, yPosL, 64, 320, 1, velocity);
	rootL.addComponent<SpriteComponent>("assets/root_l64.png");
	rootL.addComponent<ColliderComponent>("root");
	rootL.addGroup(groupEnemies);

	rootR.addComponent<TransformComponent>(xPosR, yPosR, 64, 320, 1, velocity);
	rootR.addComponent<SpriteComponent>("assets/root_r64.png");
	rootR.addComponent<ColliderComponent>("root");
	rootR.addGroup(groupEnemies);
}



//my stuff
//calcualte score
//return int
int Game::calculateScore(int total_ticks)
{
	int score = 0;

	score = total_ticks / 50;

	return score;
}

