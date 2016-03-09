#include "Game.h"
#include <iostream>

int main()
{
	bool s = false;
	bool c = false;
	char connectionType;
	unsigned int width = 1280;
	unsigned int height = 720;

	std::cout << "(s)erver or (c)lient: ";
	std::cin >> connectionType;

	if (connectionType == 'c')
		c = true;
	else if (connectionType == 's')
		s = true;

	if (s == false && c == true)
	{
		sf::RenderWindow App(sf::VideoMode(width, height), "Networking");
		Game myGame(&App);
		myGame.init();
		while (App.isOpen())
		{
			myGame.play();
		}
	}
	else if (s == true && c == false)
	{
		Game myGame;
		myGame.serverInit();
	}
	return EXIT_SUCCESS;
}