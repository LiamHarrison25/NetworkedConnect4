#pragma once

#include <socklib.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <iostream>
#include <sstream>

const int portNumber = 10346;
const int secondPortNumber = 9000;
const char* const localAddress = "127.0.0.1";

#pragma region SocketErrors

class connectionDisconnect : public std::runtime_error
{
public:
	connectionDisconnect() : std::runtime_error("connection disconnected") {}
};

class connectionTimeout : public std::runtime_error
{
public:
	connectionTimeout() : std::runtime_error("connection time out") {}
};

#pragma endregion

/// <summary>
/// This class is used to provide polymorphism for the different types of connections. (servers and clients)
/// </summary>
class NetworkedUser
{
public:

	NetworkedUser(const char* host, int port)
	{
		hostName = host;
		portNumber = port;
	}

	~NetworkedUser()
	{
		//delete socket;
		socket = nullptr;
		socket2 = nullptr;
	}

	Socket* GetSocket()
	{
		return socket;
	}

	Socket* GetSocket2()
	{
		return socket2;
	}

	void SetSocket2(Socket* sock)
	{
		socket2 = sock;
	}

	// Pure virtual functions
	virtual void RunUser() = 0; // Used for polymorphism for the different run functions on the children
	virtual void SendMessage(std::stringstream& stream, Socket* socketTarget) = 0;
	virtual std::string RecieveMessage(Socket& sock, size_t size = 4096) = 0;

protected:

	const char* hostName;
	int portNumber;
	Socket* socket; 
	Socket* socket2; // Socket 2 is used to store whatever socket the user is attempting to send or recieve data
};

/// <summary>
/// The networked server is used to create a server socket
/// </summary>
class NetworkedServer : public NetworkedUser
{
public:

	NetworkedServer(const char* host, int port) : NetworkedUser(host, port) { }

	void run_server();

	void RunUser();

	void SendMessage(std::stringstream& stream, Socket* socketTarget);
	std::string RecieveMessage(Socket& sock, size_t size = 4096);

};

/// <summary>
/// The networked client is used to create a client socket
/// </summary>
class NetworkedClient : public NetworkedUser
{
public:

	NetworkedClient(const char* host, int port) : NetworkedUser(host, port) {}

	void RunUser();

	void run_client();

	void SendMessage(std::stringstream& stream, Socket* socketTarget);
	std::string RecieveMessage(Socket& sock, size_t size = 4096);

};
