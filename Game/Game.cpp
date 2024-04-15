#include "Game.h"

void Game::initWindow()
{
	this->window = new sf::RenderWindow(sf::VideoMode(800, 600), "Swaglords of Space", sf::Style::Close | sf::Style::Titlebar);
	this->window->setFramerateLimit(144);
	this->window->setVerticalSyncEnabled(false);
}

void Game::initPlayer()
{
	this->player = new Player();
}

void Game::initEnemies()
{
	this->spawnTimerMax = 50.f;
	this->spawnTimer = this->spawnTimerMax;
}

void Game::initTextures()
{
	this->textures["BULLET"] = new sf::Texture();
	this->textures["BULLET"]->loadFromFile("Textures/laser-bullet.png");
}

void Game::initGUI()
{
	//Load font
	if (!this->font.loadFromFile("Fonts/PixellettersFull.ttf"))
	{
		cout << "ERROR::GAME::Failed to load font" << endl;
	}
	//Init point text
	this->pointText.setPosition(sf::Vector2f(600, 10.f));
	this->pointText.setFont(this->font);
	this->pointText.setCharacterSize(30);
	this->pointText.setFillColor(sf::Color::White);
	this->pointText.setString("Test");
	
	//gameOver text
	this->gameOverText.setFont(this->font);
	this->gameOverText.setCharacterSize(60);
	this->gameOverText.setFillColor(sf::Color::Red);
	this->gameOverText.setString("Game over!");
	this->gameOverText.setPosition(
		static_cast<float>(this->window->getSize().x) / 2.f - this->gameOverText.getGlobalBounds().width / 2.f,
		static_cast<float>(this->window->getSize().y) / 2.f - this->gameOverText.getGlobalBounds().height / 2.f
	);

	//hp init
	this->playerHpBar.setSize(sf::Vector2f(300.f, 15.f));
	this->playerHpBar.setFillColor(sf::Color::Red);
	this->playerHpBar.setPosition(sf::Vector2f(1.f, 20.f));
	
	this->playerHpBarBack = playerHpBar;
	this->playerHpBarBack.setFillColor(sf::Color::Black);
	this->playerHpBarBack.setPosition(sf::Vector2f(1.f, 20.f));
}

void Game::initWorld()
{
	if (!this->worldBackgroundTex.loadFromFile("Textures/background1.jpg"))
	{
		cout << "ERROR::GAME::Could not load background texture." << endl;
	}
	this->worldBackground.setTexture(this->worldBackgroundTex);
}

void Game::initSystems()
{
	this->points = 0;
}


Game::Game()
{
	this->initWindow();
	this->initTextures();
	this->initGUI();
	this->initWorld();
	this->initSystems();
	this->initPlayer();
	this->initEnemies();
}


Game::~Game()
{
	delete this->window;
	delete this->player;

	for (auto &i : this->textures)
	{
		delete i.second;
	}

	for (auto* i : this->bullets)
	{
		delete i;
	}

	for (auto* i : this->enemies)
	{
		delete i;
	}
}


void Game::run()
{
	while (this->window->isOpen())
	{
		this->updatePollEvents();
		if(this->player->getHp() > 0)
			this->update();
		this->render();
	}
}


void Game::updatePollEvents()
{
	sf::Event e;
	while (this->window->pollEvent(e))
	{
		if (e.Event::type == sf::Event::Closed)
			this->window->close();
		if (e.Event::KeyPressed && e.Event::key.code == sf::Keyboard::Escape)
			this->window->close();
	}
}

void Game::updateInput()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		this->player->move(-1.f, 0.f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		this->player->move(1.f, 0.f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		this->player->move(0.f, -1.f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		this->player->move(0.f, 1.f);
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && this->player->canAttack())
	{
		this->bullets.push_back(new Bullet(
			this->textures["BULLET"],
			this->player->getPos().x + this->player->getBounds().width/2.8f,
			this->player->getPos().y,
			0.f,
			-1.f,
			5.f
			)
		);
	}
}

void Game::updateGUI()
{
	stringstream ss;

	ss << "Points: " << this->points;

	this->pointText.setString(ss.str());

	//hpBar update
	float hpPercent = static_cast<float>(this->player->getHp()) / static_cast<float>(this->player->getHpMax());
	this->playerHpBar.setSize(sf::Vector2f(300.f * hpPercent, this->playerHpBar.getSize().y));
}

void Game::updateWorld()
{
}

void Game::updateCollision()
{
	//left
	if (this->player->getBounds().left < 0.f)
	{
		this->player->setPosition(0.f, this->player->getBounds().top);
	}

	//right
	else if (this->player->getBounds().left + this->player->getBounds().width >= this->window->getSize().x)
	{
		this->player->setPosition(this->window->getSize().x - this->player->getBounds().width, this->player->getBounds().top);
	}

	//top
	if (this->player->getBounds().top < 0.f)
	{
		this->player->setPosition(this->player->getBounds().left, 0.f);
	}

	//bottom
	else if (this->player->getBounds().top + this->player->getBounds().height >= this->window->getSize().y)
	{
		this->player->setPosition(this->player->getBounds().left, this->window->getSize().y - this->player->getBounds().height);
	}
}

void Game::updateBullets()
{
	unsigned counter = 0;
	for (auto* bullet : this->bullets)
	{
		bullet->update();

		if (bullet->getBounds().top + bullet->getBounds().height < 0.f)
		{
			delete this->bullets.at(counter);
			this->bullets.erase(this->bullets.begin() + counter);
			--counter;
		}
		++counter;
	}
}

void Game::updateEnemies()
{
	this->spawnTimer += 0.5f;
	if (this->spawnTimer >= this->spawnTimerMax)
	{
		this->enemies.push_back(new Enemy(rand() % this->window->getSize().x - 20.f, -100.f));
		this->spawnTimer = 0.f;
	}

	unsigned counter = 0;
	for (auto* enemy : this->enemies)
	{
		enemy->update();

		if (enemy->getBounds().top > this->window->getSize().y)
		{
			delete this->enemies.at(counter);
			this->enemies.erase(this->enemies.begin() + counter);
			--counter;
		}
		else if (enemy->getBounds().intersects(this->player->getBounds()))
		{
			this->player->loseHp(this->enemies.at(counter)->getDamage());
			delete this->enemies.at(counter);
			this->enemies.erase(this->enemies.begin() + counter);
			--counter;
		}
		++counter;
	}
}

void Game::updateCombat()
{
	for (int i = 0; i < this->enemies.size(); i++)
	{
		bool enemy_deleted = false;
		for (int j = 0; j < this->bullets.size() && enemy_deleted == false; j++)
		{
			if (this->enemies[i]->getBounds().intersects(this->bullets[j]->getBounds()))
			{
				this->points += this->enemies[i]->getPoints();

				delete this->enemies[i];
				this->enemies.erase(this->enemies.begin() + i);
				delete this->bullets[j];
				this->bullets.erase(this->bullets.begin() + j);

				enemy_deleted = true;
			}
		}
	}
}

void Game::update()
{
	this->updateInput();

	this->player->update();

	this->updateCollision();

	this->updateBullets();

	this->updateEnemies();

	this->updateCombat();

	this->updateGUI();

	this->updateWorld();
}

void Game::renderGUI()
{
	this->window->draw(this->pointText);
	this->window->draw(this->playerHpBarBack);
	this->window->draw(this->playerHpBar);
}

void Game::renderWorld()
{
	this->window->draw(this->worldBackground);
}


void Game::render()
{
	this->window->clear();

	this->renderWorld();

	this->player->render(*this->window);

	for (auto* bullet : this->bullets)
	{
		bullet->render(this->window);
	}
	
	for (auto* enemy : this->enemies)
	{
		enemy->render(this->window);
	}

	this->renderGUI();

	//Game over screen
	if (this->player->getHp() <= 0)
		this->window->draw(gameOverText);
	this->window->display();
}
