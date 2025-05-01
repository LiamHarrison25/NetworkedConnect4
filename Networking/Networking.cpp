#include "Networking.h"

//Networked server
#pragma region NetworkedServer

void NetworkedServer::run_server()
{
	socket = new Socket(Socket::Family::INET, Socket::Type::STREAM);

	socket->Bind(Address(hostName, portNumber));

	socket->Listen();
}

void NetworkedServer::SendMessage(std::stringstream& stream, Socket* socketTarget)
{
	std::string message = stream.str();

	int bytesSent = socketTarget->Send(message.data(), message.size());

}

std::string NetworkedServer::RecieveMessage(Socket& sock, size_t size)
{
	std::string buffer(size, '\0');

	int numBytesRecieved = sock.Recv(buffer.data(), buffer.size());

	if (numBytesRecieved < 0)
	{
		throw connectionTimeout();
	}
	else if (numBytesRecieved == 0)
	{
		throw connectionDisconnect();
	}

	buffer.resize(numBytesRecieved);
	return buffer;
}

void NetworkedServer::RunUser()
{
	run_server();
}

#pragma endregion

//Networked Client

#pragma region NetworkedClient

void NetworkedClient::run_client()
{
	socket = new Socket(Socket::Family::INET, Socket::Type::STREAM);

	try
	{
		socket->Connect(Address(hostName, portNumber));
		socket->SetNonBlockingMode(true);
	}
	catch (std::runtime_error)
	{
		std::cout << "Could not make a connection" << std::endl;
	}
}

void NetworkedClient::SendMessage(std::stringstream& stream, Socket* socketTarget)
{
	std::string message = stream.str();

	int bytesSent = socketTarget->Send(message.data(), message.size());
}


std::string NetworkedClient::RecieveMessage(Socket& sock, size_t size)
{
	std::string buffer(size, '\0');

	int numBytesRecieved = sock.Recv(buffer.data(), buffer.size());

	if (numBytesRecieved < 0)
	{
		throw connectionTimeout();
	}
	else if (numBytesRecieved == 0)
	{
		throw connectionDisconnect();
	}

	buffer.resize(numBytesRecieved);
	return buffer;
}

void NetworkedClient::RunUser()
{
	run_client();
}

#pragma endregion
