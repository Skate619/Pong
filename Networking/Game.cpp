#include "Game.h"
std::vector<std::string> menuSize;
std::vector<Player*> players;

Game::Game(sf::RenderWindow* app) : win(app)
{

}

Game::Game()
{

}

void Game::init()
{
	square.loadFromFile("Square.ttf");
	Up = false;
	Down = false;
	Enter = false;
	std::string myMenu1 = "Menu";
	std::string myMenu2 = "Play";
	std::string myMenu3 = "Scores";
	std::string myMenu4 = "Exit";
	menuSize.push_back(myMenu1);
	menuSize.push_back(myMenu2);
	menuSize.push_back(myMenu3);
	menuSize.push_back(myMenu4);
	menuState = true;
	menu = new Menu(static_cast<sf::Vector2f>(win->getSize()), menuSize, win);
	winWidth = win->getSize().x;
	winHeight = win->getSize().y;
	return;
}

void Game::build()
{
	player = new Player(win);
	player->setupConnection();
}

void Game::mainMenu(sf::RenderWindow* app)
{
	win = app;
	win->clear(sf::Color::Black);
	win->setView(win->getDefaultView());
	menu->draw();
	win->display();

	sf::Event Event;

	while (win->pollEvent(Event))
	{
		if (Event.type == sf::Event::Closed)
		{
			win->close();
		}
	}

	menu->move();

	if (!(sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) && Enter == true)
		Enter = false;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && Enter == false)
	{
		Enter = true;
		menu->select();
		if (!menu->getMenuState())
		{
			build();
		}
	}
	return;
}

void Game::userInput()
{
	sf::Event Event;
	while (win->pollEvent(Event))
	{
		if (Event.type == sf::Event::Closed)
		{
			win->close();
		}
	}
	return;
}

void Game::update()
{
	if (player->isActive())
	{
		if (clock.getElapsedTime().asSeconds() > 1 / GAME_FRAMERATE)
		{
			player->playerInput();
			clock.restart();
		}
	}
}

void Game::draw()
{
	win->clear(sf::Color(0, 0, 0));
	int arenaTop = player->getArena();
	int dashLength = 20; // Length of the dashes
	arenaTop = arenaTop + (dashLength / 2);
	int dashes = ((win->getSize().y - arenaTop) + dashLength * 2) / dashLength;
	sf::VertexArray vertLines(sf::Lines, dashes); // A vertex array of lines to draw
	for (int i = 0; i < dashes; ++i)
	{
		vertLines[i].position = sf::Vector2f(win->getSize().x / 2, (arenaTop + (i * dashLength) - (dashLength / 2)));
	}

	sf::VertexArray horizLines(sf::Lines, 2); // A vertex array of lines to draw
	horizLines[0].position.x = 0;
	horizLines[0].position.y = 51;
	horizLines[1].position.x = win->getSize().x;
	horizLines[1].position.y = 51;

	win->draw(vertLines);
	win->draw(horizLines);
	player->draw();
	return;
}

void Game::play()
{
	if (menu->getMenuState())
	{
		mainMenu(win);
	}
	else
	{
		draw();
		userInput();
		update();
		win->display();
	}
	return;
}

void Game::serverInit()
{
	server = new Server();
}