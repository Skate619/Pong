#ifndef GAME_H
#define GAME_H
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <string>
#include <sstream>
#include "Menu.h"
#include "Player.h"
#include "Server.h"

const float GAME_FRAMERATE = 60.0f;

class Game
{
public:
	Game(sf::RenderWindow* app);
	Game();
	sf::RenderWindow* win;
	void init();
	void serverInit();
	void build();
	void draw();
	void update();
	void play();
	void userInput();
	void mainMenu(sf::RenderWindow* app);

protected:
	sf::Clock clock;
	sf::Clock puckClock;
private:
	bool menuState;
	bool Up, Enter, Down;
	char choice = ' ';
	int port;
	float puckWidth, puckHeight, winWidth, winHeight;
	Menu* menu;
	Player* player;
	Server* server;

	sf::Font square;
};
#endif