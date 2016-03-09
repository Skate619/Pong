#ifndef SERVER_H
#define SERVER_H
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>
#include "Player.h"

const float SERVER_FRAMERATE = 60.0f;
const int MAX_PLAYERS = 2;

struct playerId
{
	sf::Vector2f position;
	sf::RectangleShape playerRect;
	int playerID, collision;
};

const float MAX_PUCK_SPEED = 10.0f;

struct Puck
{
	sf::Vector2f position;
	sf::Vector2f velocity;
	sf::RectangleShape puckRect;
	int id = 3;

	void move(float elapsedTime)
	{
		int speed = pow(velocity.x, 2) + pow(velocity.y, 2);
		double max_vel = pow(MAX_PUCK_SPEED, 2);
		sf::Vector2f tempvel;
		tempvel = sf::Vector2f(max_vel / speed, max_vel / speed);
		if (speed > max_vel || speed < -max_vel)
		{
			velocity.x *= tempvel.x;
			velocity.y *= tempvel.y;
		}
		position.x += velocity.x;
		position.y += velocity.y;
		puckRect.setPosition(position);
	}

	bool collision(sf::RectangleShape targetRect)
	{
		if (puckRect.getGlobalBounds().intersects(targetRect.getGlobalBounds()))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool bounce(float top, float bottom)
	{
		// a function to check if puck has collided with top or bottom of screen
		if (puckRect.getGlobalBounds().top - 5 <= top)
			return true;
		else if ((puckRect.getGlobalBounds().top + puckRect.getGlobalBounds().height) + 5 >= bottom)
			return true;
		else
			return false;
	}

	int offscreen(float winRight)
	{
		float right = puckRect.getGlobalBounds().left + puckRect.getGlobalBounds().width;
		if (puckRect.getGlobalBounds().left < -10)
		{
			// add score to player 2 and reset the puck position
			return 2;
		}
		else if (right > winRight + 10)
		{
			// add score to player 1 and reset puck position
			return 1;
		}
		else
			return 0;
	}
};

class Server
{
public:
	Server();
	void serverUpdate();
	void serverLoop();
	void serverListen();
	void restart();
protected:

private:
	Puck puck;
	sf::IpAddress publicIp = sf::IpAddress::getPublicAddress();
	sf::IpAddress localIp = sf::IpAddress::getLocalAddress();
	sf::SocketSelector selector;
	sf::TcpListener listener;
	sf::Packet packet, sendPacket;

	std::string publicString = publicIp.toString();
	std::string localString = localIp.toString();
	std::string packetString, myString, player1Name, player2Name;
	std::string W = "W";
	std::string S = "S";
	std::string puckString = "puck";

	std::vector<playerId> players;
	std::vector<sf::TcpSocket*> sockets;
	float player1Score, player2Score;
	int port, id, winX, winY;
	int arenaTop = 51;
	bool playersActive;
	int previous, current;
};
#endif