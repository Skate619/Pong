#include "Player.h"

Player::Player(sf::RenderWindow* app)
{
	leftScore << 0;
	rightScore << 0;
	puckRect.setSize(sf::Vector2f(10, 10));
	names = true;
	win = app;
	square.loadFromFile("Square.ttf");
}

bool Player::checkCollision(std::string side)
{
	if (side == "top")
	{
		if (playerRect.getGlobalBounds().top - arenaTop <= 5)
		{
			return true;
		}
		else
			return false;
	}
	else if (side == "bottom")
	{
		if ((playerRect.getGlobalBounds().top + rectHeight) + 5 >= win->getSize().y)
		{
			return true;
		}
		else
			return false;
	}
}

void Player::playerInput()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		if (!checkCollision("top"))
		{
			std::string keyString = "W";
			playerPacket << keyString << id;
			playerSocket.send(playerPacket);
			playerPacket.clear();
			keyString.clear();
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		if (!checkCollision("bottom"))
		{
			std::string keyString = "S";
			playerPacket << keyString << id;
			playerSocket.send(playerPacket);
			playerPacket.clear();
			keyString.clear();
		}
	}
}

void Player::setupConnection()
{
	setup = true;
	address = false;
	portNum = false;
	name = false;
	Enter = true;
	ipString.clear();
	myString.clear();
	port = 0;

	while (setup)
	{
		win->clear(sf::Color::Black);
		win->setView(win->getDefaultView());
		sf::Event Event;

		while (win->pollEvent(Event))
		{
			if (Event.type == sf::Event::Closed)
				win->close();

			if (Event.type == sf::Event::TextEntered)
			{
				if (Event.text.unicode < 128 && Event.text.unicode != 32 && Event.text.unicode != 13 && Event.text.unicode != 27)
				{
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace))
					{
						if (myString.size() > 0)
							myString.erase(myString.size() - 1, 2);
					}

					else
					{
						myString = myString + static_cast<char>(Event.text.unicode);
					}
				}
			}
		}

		ipAddress.setFont(square);
		ipAddress.setString(myString);
		width = ipAddress.getGlobalBounds().width;
		ipAddress.setColor(sf::Color(255, 255, 255));
		ipAddress.setPosition((win->getSize().x - width) / 2, win->getSize().y / 2);
		ipAddress.setCharacterSize(20);

		help.setFont(square);
		if (name == false)
			help.setString("Enter player name");
		else if (address == false)
			help.setString("Enter Ip Address");
		else if (portNum == false)
			help.setString("Enter Port");

		width = help.getGlobalBounds().width;
		help.setColor(sf::Color(255, 255, 255));
		help.setPosition((win->getSize().x - width) / 2, win->getSize().y / 4);
		help.setCharacterSize(20);
		win->draw(ipAddress);
		win->draw(help);
		win->display();

		if (!(sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) && Enter == true)
			Enter = false;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && Enter == false)
		{
			Enter = true;
			if (name == false)
			{
				playerName = myString;
				playerPacket << playerName;
				name = true;
				myString.clear();
			}
			else if (address == false)
			{
				ipString = myString;
				address = true;
				myString.clear();
			}
			else if (portNum == false)
			{
				std::istringstream(myString) >> port;
				portNum = true;
				myString.clear();
			}

			if (address == true && portNum == true)
				setup = false;
		}
	}
	connect();
}

void Player::connect()
{
	sf::IpAddress ip(ipString);

	sf::Socket::Status status = playerSocket.connect(ip, port, sf::seconds(5));

	if (status != sf::Socket::Done)
	{
		error();
	}
	else
	{
		opponentRect.setFillColor(sf::Color(255, 255, 255));
		opponentRect.setSize(sf::Vector2f(10, 40));
		opponentRect.setPosition(sf::Vector2f(-50, -50)); // set offscreen until joins

		playerRect.setFillColor(sf::Color(255, 255, 255));
		playerRect.setSize(sf::Vector2f(10, 40));
		playerRect.setPosition(sf::Vector2f(0 + playerRect.getSize().x, (win->getSize().y - playerRect.getSize().y) / 2));
		rectHeight = playerRect.getGlobalBounds().height;
		playerPacket << playerRect.getPosition().x << playerRect.getPosition().y << playerRect.getSize().x << playerRect.getSize().y;

		playerPacket << win->getSize().x << win->getSize().y;

		std::cout << "Connected to: " << playerSocket.getRemoteAddress();
		playerSocket.send(playerPacket);
		playerPacket.clear();

		playerSocket.receive(playerPacket);
		if (playerPacket >> playerId)
		{
			id = playerId;
		}
		playerPacket.clear();

		playerSocket.receive(playerPacket);
		if (playerPacket >> playerX >> playerY)
		{
			if (playerId == 2)
			{
				playerRect.setPosition(sf::Vector2f(playerX, playerY));
				opponentRect.setPosition(sf::Vector2f(10, playerY));
			}
		}

		getNames();
	}
}

void Player::getNames()
{
	win->clear(sf::Color(0, 0, 0));
	sf::String waitString = "Please Wait";
	sf::Text waitText;
	waitText.setFont(square);
	waitText.setString(waitString);
	waitText.setPosition((win->getSize().x / 2) - (waitText.getGlobalBounds().width / 2), (win->getSize().y / 2) - waitText.getGlobalBounds().height);
	waitText.setColor(sf::Color(255, 255, 255));
	win->draw(waitText);
	win->setView(win->getDefaultView());
	win->display();

	playerSocket.setBlocking(false);
	playerSocket.receive(playerPacket);

	if (playerPacket >> newOpponentName >> newPlayerName)
	{
		opponentName = newOpponentName;
		playerName = newPlayerName;
	}
	if (playerName != "" && opponentName != "")
	{
		names = false;
		sf::Vector2f position(win->getSize().x / 2 - puckRect.getGlobalBounds().width, win->getSize().y / 2);
		puckRect.setPosition(position);
	}
	playerPacket.clear();

	leftPlayer.setString(playerName);
	rightPlayer.setString(opponentName);

	leftPlayer.setFont(square);
	leftPlayer.setColor(sf::Color(255, 255, 255));
	leftPlayer.setPosition(sf::Vector2f(10, 10));
	leftPlayer.setCharacterSize(40);

	rightPlayer.setFont(square);
	rightPlayer.setColor(sf::Color(255, 255, 255));
	rightPlayer.setCharacterSize(40);

	arenaTop = leftPlayer.getGlobalBounds().top + leftPlayer.getGlobalBounds().height;
}

void Player::error()
{
	bool error = true;
	myString.clear();
	ipAddress.setString(myString);
	while (error)
	{
		win->clear(sf::Color::Black);
		win->setView(win->getDefaultView());

		sf::Event Event;
		while (win->pollEvent(Event))
		{
			if (Event.type == sf::Event::Closed)
				win->close();

			if (!(sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) && Enter == true)
				Enter = false;

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && Enter == false)
			{
				Enter = true;
				error = false;
			}
		}

		help.setString("Error connecting to: " + ipString + "\nPress enter to try again");
		help.setFont(square);
		int width = help.getGlobalBounds().width;
		help.setColor(sf::Color(255, 255, 255));
		help.setPosition((win->getSize().x - width) / 2, win->getSize().y / 4);
		help.setCharacterSize(20);
		win->draw(ipAddress);
		win->draw(help);
		win->display();
	}
	setupConnection();
}

void Player::draw()
{
	if (names)
		getNames();
	else
	{
		rightPlayer.setPosition(sf::Vector2f((win->getSize().x - rightPlayer.getGlobalBounds().width) - 10, 10));

		playerSocket.setBlocking(false);
		playerSocket.receive(playerPacket);
		if (playerPacket >> playerId >> playerX >> playerY)
		{
			if (playerId == id)
			{
				playerRect.setPosition(sf::Vector2f(playerX, playerY));
			}
			else if (playerId == 3)
			{
				puckRect.setPosition(sf::Vector2f(playerX, playerY));
			}
			else if (playerId == 4)
			{
				leftScore.str("");
				rightScore.str("");
				leftScore << playerX;
				rightScore << playerY;
				
			}
			else if (playerId == 5)
			{
				reset(playerX);
				leftScore.str("");
				rightScore.str("");
				rightScore << 0;
				leftScore << 0;
				rightScoreText.setString(rightScore.str());
				leftScoreText.setString(leftScore.str());
			}
			else
			{
				opponentRect.setPosition(sf::Vector2f(playerX, playerY));
			}
		}
		playerPacket.clear();

		leftScoreText.setFont(square);
		leftScoreText.setString(leftScore.str());
		leftScoreText.setColor(sf::Color(255, 255, 255));
		leftScoreText.setPosition(sf::Vector2f((win->getSize().x / 8) * 3, 10));
		leftScoreText.setCharacterSize(40);

		rightScoreText.setFont(square);
		rightScoreText.setString(rightScore.str());
		rightScoreText.setColor(sf::Color(255, 255, 255));
		rightScoreText.setPosition(sf::Vector2f((win->getSize().x / 8) * 5, 10));
		rightScoreText.setCharacterSize(40);

		win->draw(leftPlayer);
		win->draw(rightPlayer);
		win->draw(leftScoreText);
		win->draw(rightScoreText);
		win->draw(puckRect);
		win->draw(playerRect);
		win->draw(opponentRect);
	}
}

void Player::reset(float player)
{
	bool resume = false;
	sf::Event Event;
	while (!resume)
	{
		win->clear(sf::Color::Black);
		win->setView(win->getDefaultView());
		int playerNum = player;
		sf::String playerWin = "Player: " + std::to_string(playerNum) + " wins!";
		std::string tempString = "Press [Enter] to continue";
		winText.setString(playerWin);
		winText.setFont(square);
		int width = winText.getGlobalBounds().width;
		winText.setColor(sf::Color(255, 255, 255));
		winText.setPosition((win->getSize().x - width) / 2, win->getSize().y / 4);
		winText.setCharacterSize(20);

		help.setString(tempString);
		help.setFont(square);
		width = help.getGlobalBounds().width;
		help.setColor(sf::Color(255, 255, 255));
		help.setPosition((win->getSize().x - width) / 2, (win->getSize().y / 4) * 2);
		help.setCharacterSize(20);

		win->draw(help);
		win->draw(winText);
		win->display();

		while (win->pollEvent(Event))
		{
			if (Event.type == sf::Event::Closed)
				win->close();

			if (!(sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) && Enter == true)
				Enter = false;

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && Enter == false)
			{
				playerPacket.clear();
				std::string newString = "continue";
				Enter = true;
				resume = true;
				playerPacket << newString << id;
				playerSocket.send(playerPacket);
				playerPacket.clear();
			}
		}
	}
}

std::string Player::getPlayerName()
{
	return(playerName);
}

bool Player::isActive()
{
	if (!names)
		return true;
	else
		return false;
}

sf::RectangleShape Player::getPlayerRect()
{
	return playerRect;
}

sf::RectangleShape Player::getOpponentRect()
{
	return opponentRect;
}

int Player::getArena()
{
	return arenaTop;
}