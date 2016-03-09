#ifndef MENU_H
#define MENU_H
#include <fstream>
#include <iostream>
#include <SFML/Graphics.hpp>

const int MAX_ITEMS = 20;

class Menu
{
public:
	Menu(sf::Vector2f winSize, std::vector<std::string> menuSize, sf::RenderWindow* app);
	void draw();
	void move();
	void select();
	void setMenuState(int state);
	bool getMenuState();
	bool menuState;
private:
	int selectedIndex;
	int MENU_ITEMS;
	bool up, down;
	sf::RenderWindow* win;
	sf::Font font;
	sf::Text menu[MAX_ITEMS];
};
#endif