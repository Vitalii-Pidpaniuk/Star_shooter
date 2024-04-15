#pragma once
#include <map>
#include <string>
#include <sstream>
#include <vector>
#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"

using namespace std;

class Game
{
private:
	//Window
	sf::RenderWindow* window;

	//Resources
	map<string, sf::Texture*> textures;
	vector<Bullet*> bullets;
	
	//GUI
	sf::Font font;
	sf::Text pointText;

	sf::Text gameOverText;

	//World
	sf::Texture worldBackgroundTex;
	sf::Sprite worldBackground;

	//Systems
	unsigned points;

	//Player
	Player* player;

	//PlayerGUI
	sf::RectangleShape playerHpBar;
	sf::RectangleShape playerHpBarBack;

	//Enemy
	float spawnTimer;
	float spawnTimerMax;

	vector<Enemy*> enemies;

	void initWindow();
	void initTextures();
	void initGUI();
	void initWorld();
	void initSystems();

	void initPlayer();
	void initEnemies();

public:
	Game();
	virtual ~Game();

	void run();


	void updatePollEvents();
	void updateInput();
	void updateGUI();
	void updateWorld();
	void updateCollision();
	void updateBullets();
	void updateEnemies();
	void updateCombat();
	void update();
	void renderGUI();
	void renderWorld();
	void render();
};

