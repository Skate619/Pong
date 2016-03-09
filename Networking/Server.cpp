#include "Server.h"

Server::Server()
{
	player1Score = 0;
	player2Score = 0;
	playersActive = false;
	puck.puckRect.setSize(sf::Vector2f(10, 10));
	puck.velocity.x = 3.0f;
	puck.velocity.y = 3.0f;
	std::cout << "Your public ip: " << publicIp << "\n";
	std::cout << "Your local ip:  " << localIp << "\n";
	std::cout << "Enter a port to use: ";
	std::cin >> port;
	serverListen();
}

void Server::serverListen()
{
	listener.listen(port);
	selector.add(listener);
	float x, y, xSize, ySize;
	int xWin, yWin;
	sf::RectangleShape playerRect;
	while (!playersActive)
	{
		if (selector.wait())
		{
			if (selector.isReady(listener))
			{
				sf::TcpSocket *socket = new sf::TcpSocket;
				listener.accept(*socket);
				std::cout << "\nConnection from: " << socket->getRemoteAddress();
				sockets.push_back(socket);
				selector.add(*socket);
			}

			for (int i = 0; i < sockets.size(); ++i)
			{
				if (selector.isReady(*sockets[i]))
				{
					sockets[i]->receive(packet);

					if (players.size() < sockets.size())
					{
						if (packet >> myString >> x >> y >> xSize >> ySize >> xWin >> yWin)
						{
							winX = xWin;
							winY = yWin;
							puck.position.x = xWin / 2 - puck.puckRect.getGlobalBounds().width;
							puck.position.y = yWin / 2;
							puck.puckRect.setPosition(puck.position);
							std::cout << "\nPlayer connected: " << myString;
							if (player1Name.empty())
								player1Name = myString;
							else if (player2Name.empty())
								player2Name = myString;
							playerId newPlayer;
							newPlayer.position.x = x;
							newPlayer.position.y = y;
							newPlayer.playerRect.setFillColor(sf::Color(255, 255, 255));
							newPlayer.playerRect.setSize(sf::Vector2f(xSize, ySize));
							newPlayer.playerID = players.size() + 1;
							newPlayer.playerRect.setPosition(newPlayer.position);

							sendPacket << newPlayer.playerID;
							for (int j = 0; j < sockets.size(); ++j)
								sockets[j]->send(sendPacket);
							sendPacket.clear();

							if (newPlayer.playerID == 2)
							{
								newPlayer.position.x = xWin - (xSize * 2);
								newPlayer.playerRect.setPosition(newPlayer.position);
								sendPacket << players[0].position.x << players[0].position.y;
								for (int j = 0; j < sockets.size(); ++j)
									sockets[j]->send(sendPacket);
								sendPacket.clear();
							}
							players.push_back(newPlayer);

							if (newPlayer.playerID == 2)
							{
								sendPacket << player2Name << player1Name;
								for (int j = 0; j < sockets.size(); ++j)
									sockets[j]->send(sendPacket);
								sendPacket.clear();
								playersActive = true;
							}

							sendPacket << newPlayer.playerID << newPlayer.position.x << newPlayer.position.y;
							for (int j = 0; j < sockets.size(); ++j)
								sockets[j]->send(sendPacket);
							sendPacket.clear();

							myString.clear();
							packet.clear();
						}
					}
				}
			}
		}
	}
	serverLoop();
	for (std::vector<sf::TcpSocket*>::iterator it = sockets.begin(); it != sockets.end(); ++it)
		delete *it;
}

void Server::serverLoop()
{
	previous = 0;
	current = 0;
	bool puckVel = false;
	sockets[0]->setBlocking(false);
	sockets[1]->setBlocking(false);
	sf::Clock clock;
	while (playersActive)
	{
		if (clock.getElapsedTime().asSeconds() > 1 / SERVER_FRAMERATE)
		{
			float elapsedTime = clock.getElapsedTime().asSeconds();
			puck.move(elapsedTime);
			for (int i = 0; i < players.size(); ++i)
			{
				current = i;
				if (puck.collision(players[i].playerRect) && previous != current)
				{
					previous = i;
					puck.velocity.x = puck.velocity.x * -1;
					if (puck.velocity.x < 0)
						puck.velocity.x -= 0.5f;
					else
						puck.velocity.x += 0.5f;
				}
			}
			if (puck.bounce(arenaTop, winY))
				puck.velocity.y = puck.velocity.y * -1;
			if (puck.offscreen(winX) == 1)
			{
				// add score to player 1 and reset
				player1Score += 1;
				puck.position.x = winX / 2 - puck.puckRect.getGlobalBounds().width;
				puck.position.y = winY / 2;
				puck.puckRect.setPosition(puck.position);

				puck.velocity.y = 3.0f;
				if (puck.velocity.x < 0)
				{
					if (puck.velocity.x / 2.0f > -3.0f)
						puck.velocity.x = -3.0f;
					else
						puck.velocity.x = puck.velocity.x / 2.0f;
				}
				else
				{
					if (puck.velocity.x / 2.0f < 3.0f)
						puck.velocity.x = 3.0f;
					else
						puck.velocity.x = puck.velocity.x / 2.0f;
				}
				for (int i = 0; i < sockets.size(); ++i)
				{
					sendPacket << 4 << player1Score << player2Score;
					sockets[i]->send(sendPacket);
					sendPacket.clear();
				}
			}
			if (puck.offscreen(winX) == 2)
			{
				// add score to player 2 and reset
				player2Score += 1;
				puck.position.x = winX / 2 - puck.puckRect.getGlobalBounds().width;
				puck.position.y = winY / 2;
				puck.puckRect.setPosition(puck.position);

				puck.velocity.y = 3.0f;
				if (puck.velocity.x < 0)
				{
					if (puck.velocity.x / 2.0f < -3.0f)
						puck.velocity.x = -3.0f;
					else
						puck.velocity.x = puck.velocity.x / 2.0f;
				}
				else
				{
					if (puck.velocity.x / 2.0f < 3.0f)
						puck.velocity.x = 3.0f;
					else
						puck.velocity.x = puck.velocity.x / 2.0f;
				}
				for (int i = 0; i < sockets.size(); ++i)
				{
					sendPacket << 4 << player1Score << player2Score;
					sockets[i]->send(sendPacket);
					sendPacket.clear();
				}
			}
			for (int i = 0; i < sockets.size(); ++i)
			{
				sendPacket << puck.id << puck.position.x << puck.position.y;
				sockets[i]->send(sendPacket);
				sendPacket.clear();
			}
			clock.restart();
		}

		for (int i = 0; i < sockets.size(); ++i)
		{
			sockets[i]->receive(packet);
			if (packet >> myString >> id)
			{
				if (myString == W)
				{
					players[i].position.y -= 5;
					players[i].playerRect.setPosition(players[i].position);
				}
				else if (myString == S)
				{
					players[i].position.y += 5;
					players[i].playerRect.setPosition(players[i].position);

				}
				sendPacket << id << players[i].position.x << players[i].position.y;
				for (int j = 0; j < sockets.size(); ++j)
					sockets[j]->send(sendPacket);
				sendPacket.clear();
				myString.clear();
			}
			packet.clear();
		}

		if (player1Score >= 7 || player2Score >= 7)
		{
			playersActive = false;
			if (player1Score >= 7)
			{
				sendPacket << 5 << 1.0f << 2.0f;
				for (int i = 0; i < sockets.size(); ++i)
					sockets[i]->send(sendPacket);
			}
			else if (player2Score >= 7)
			{
				sendPacket << 5 << 2.0f << 1.0f;
				for (int i = 0; i < sockets.size(); ++i)
					sockets[i]->send(sendPacket);
			}
			sendPacket.clear();
			restart();
		}
	}
}

void Server::restart()
{
	// listen for player1 and player2 to continue then call to loop
	bool p1Resume = false;
	bool p2Resume = false;
	int p1 = 1;
	int p2 = 2;
	bool resume = false;
	while (!resume)
	{
		packet.clear();
		if (p1Resume == true && p2Resume == true)
		{
			resume = true;
		}

		for (int i = 0; i < sockets.size(); ++i)
		{
			sockets[i]->receive(packet);
			if (packet >> myString >> id)
			{
				std::cout << "\nId: " << id << " String: " << myString;
				if (id == p1 && myString == "continue")
					p1Resume = true;
				else if (id == p2 && myString == "continue")
					p2Resume = true;
			}
			packet.clear();
		}
	}
	player1Score = 0;
	player2Score = 0;
	
	playersActive = true;
	serverLoop();
}