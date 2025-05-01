#include "Networking.h"

//Metworked user:

void NetworkedUser::RunNetworkedUpdate()
{

}


//Networked server

void NetworkedServer::run_server()
{
	socket = new Socket(Socket::Family::INET, Socket::Type::STREAM);

	//socket->Create();

	socket->Bind(Address(hostName, portNumber));

	socket->Listen();
}

void NetworkedServer::SendMessage(std::stringstream& stream, Socket* socketTarget)
{
	std::string message = stream.str();

	int bytesSent = socketTarget->Send(message.data(), message.size());

	//std::string buffer(4096, '\0');

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

void NetworkedServer::CheckForInput()
{

}

void NetworkedServer::RunNetworkedUpdate()
{
	CheckForInput();
}

void NetworkedServer::RunUser()
{
	run_server();
}

//Networked Client

void NetworkedClient::run_client()
{
	socket = new Socket(Socket::Family::INET, Socket::Type::STREAM);
	//socket->Create(Socket::Family::INET, Socket::Type::STREAM);

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

void NetworkedClient::RunNetworkedUpdate()
{
	//Send Message
}

void NetworkedClient::RunUser()
{
	run_client();
}

