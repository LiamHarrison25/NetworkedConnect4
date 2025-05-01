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

	virtual void RunUser() = 0;

	virtual void RunNetworkedUpdate();

	virtual void SendMessage(std::stringstream& stream, Socket* socketTarget) = 0;
	virtual std::string RecieveMessage(Socket& sock, size_t size = 4096) = 0;

protected:

	const char* hostName;
	int portNumber;
	Socket* socket;
	Socket* socket2;

};

class NetworkedServer : public NetworkedUser
{
public:

	NetworkedServer(const char* host, int port) : NetworkedUser(host, port) { hasConnection = false; }

	void RunNetworkedUpdate();

	void run_server();

	void CheckForInput();

	void RunUser();

private:

	Socket connection;
	bool hasConnection;

	void SendMessage(std::stringstream& stream, Socket* socketTarget);
	std::string RecieveMessage(Socket& sock, size_t size = 4096);
};

class NetworkedClient : public NetworkedUser
{
public:

	NetworkedClient(const char* host, int port) : NetworkedUser(host, port) {}

	void RunNetworkedUpdate();

	void RunUser();

	void run_client();
	void SendMessage(std::stringstream& stream, Socket* socketTarget);
	std::string RecieveMessage(Socket& sock, size_t size = 4096);

private:


};
