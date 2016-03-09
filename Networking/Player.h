#ifndef PLAYER_H
#define PLAYER_H
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <sstream>
class Player
{
public:
	Player(sf::RenderWindow* app);
	void playerInput();
	void setupConnection();
	void connect();
	void error();
	void draw();
	void getNames();
	void reset(float player);
	bool checkCollision(std::string side);
	bool isActive();
	int getArena();
	std::string getPlayerName();
	sf::RectangleShape getPlayerRect();
	sf::RectangleShape getOpponentRect();
	sf::TcpListener playerListener;
	sf::TcpSocket playerSocket;
	sf::Packet playerPacket;
	sf::IpAddress ip;

private:
	sf::RenderWindow* win;
	sf::Font square;
	sf::Text ipAddress, help, leftPlayer, rightPlayer, leftScoreText, rightScoreText, winText;

	sf::RectangleShape playerRect;
	sf::RectangleShape opponentRect;
	sf::RectangleShape puckRect;

	std::stringstream leftScore;
	std::stringstream rightScore;

	bool Enter, setup, address, portNum, name, names;
	bool name1, name2;

	int port, id, playerId, opponentId;
	float width, playerX, playerY, puckX, puckY, opponentX, opponentY;
	float arenaTop, rectHeight;

	std::string myString, ipString, playerName, opponentName;
	std::string newPlayerName, newOpponentName;
	std::string packetString = "move";
};
#endif