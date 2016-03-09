#include "Menu.h"

Menu::Menu(sf::Vector2f winSize, std::vector<std::string> menuSize, sf::RenderWindow* app)
{
	win = app;
	MENU_ITEMS = menuSize.size();
	if (!font.loadFromFile("Square.ttf"))
	{
		std::cout << "Cannot find square.ttf";
	}

	for (int i = 0; i < menuSize.size(); ++i)
	{
		menu[i].setFont(font);
		menu[i].setString(menuSize[i]);
		menu[i].setPosition(sf::Vector2f((winSize.x - menu[i].getGlobalBounds().width) / 2, winSize.y / (menuSize.size() + 1) * (i + 1)));
		menu[i].setColor(sf::Color(255, 255, 255));
	}

	if (menuSize.size() >= 2)
	{
		menu[0].setColor(sf::Color(255, 255, 0));
		menu[1].setColor(sf::Color(255, 0, 0));
	}
	up = false;
	down = false;
	selectedIndex = 1;
}

void Menu::move()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !up)
	{
		menu[selectedIndex].setColor(sf::Color(255, 255, 255));
		selectedIndex--;
		if (selectedIndex < 1)
			selectedIndex = MENU_ITEMS - 1;
		menu[selectedIndex].setColor(sf::Color(255, 0, 0));
		up = true;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && !down)
	{
		menu[selectedIndex].setColor(sf::Color(255, 255, 255));
		selectedIndex++;
		if (selectedIndex > MENU_ITEMS - 1)
			selectedIndex = 1;
		menu[selectedIndex].setColor(sf::Color(255, 0, 0));
		down = true;
	}

	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		up = false;
	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		down = false;
}

void Menu::select()
{
	if (selectedIndex == 1)
	{
		menuState = false;
	}
	else if (selectedIndex == 2)
	{
		menuState = false;
	}
	else if (selectedIndex == 3)
		win->close();
}

void Menu::draw()
{
	for (int i = 0; i < MENU_ITEMS; ++i)
	{
		if (menu[i].getString() != "")
			win->draw(menu[i]);
	}
}

bool Menu::getMenuState()
{
	return menuState;
}