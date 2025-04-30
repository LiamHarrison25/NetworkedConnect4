#pragma once

#include <socklib.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <iostream>
#include <sstream>

const int portNumber = 10346;
const char* const localAddress = "127.0.0.1";

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

class NetworkedUser
{
public:

	NetworkedUser(const char* host, int port)
	{
		hostName = host;
		portNumber = port;
	}

	Socket* GetSocket()
	{
		return &socket;
	}

	virtual void RunNetworkedUpdate();

protected:

	const char* hostName;
	int portNumber;
	Socket socket;

};

class NetworkedServer : public NetworkedUser
{
public:

	NetworkedServer(const char* host, int port) : NetworkedUser(host, port) { hasConnection = false; }

	void RunNetworkedUpdate();

	void run_server();

	void CheckForInput();

private:

	Socket connection;
	bool hasConnection;

	std::string recieve(Socket& sock, size_t size = 4096);
};

class NetworkedClient : public NetworkedUser
{
public:

	NetworkedClient(const char* host, int port) : NetworkedUser(host, port) {}

	void RunNetworkedUpdate();

	void run_client();
	std::string SendMessage(std::stringstream& stream);

private:


};
