#include "Networking.h"

//Metworked user:

void NetworkedUser::RunNetworkedUpdate()
{

}


//Networked server

void NetworkedServer::run_server()
{
	socket.Create(Socket::Family::INET, Socket::Type::STREAM);

	socket.Bind(Address(hostName, portNumber));

	socket.SetNonBlockingMode(true);

	socket.Listen();
}

std::string NetworkedServer::recieve(Socket& sock, size_t size)
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
	//if (!hasConnection)
	//{
	//	try
	//	{
	//		Socket connectionTest = socket.Accept();
	//		connection = connectionTest; //= socket.Accept();
	//	}
	//	catch (std::runtime_error)
	//	{
	//		std::cout << "Could not connect" << std::endl;
	//	}

	//	std::cout << "Connection established" << std::endl;

	//	hasConnection = true;
	//}

	//if (hasConnection)
	//{
	//	try
	//	{
	//		std::string message = recieve(connection);

	//		connection.Send(message.data(), message.size());
	//	}
	//	catch (connectionDisconnect& e)
	//	{
	//		hasConnection = false;
	//		std::cout << "The connection was closed" << std::endl;
	//	}
	//	catch (connectionTimeout& e)
	//	{
	//		hasConnection = false;
	//	}
	//}	
}

void NetworkedServer::RunNetworkedUpdate()
{
	CheckForInput();
}

//Networked Client

void NetworkedClient::run_client()
{
	socket.Create(Socket::Family::INET, Socket::Type::STREAM);

	try
	{
		socket.Connect(Address(hostName, portNumber));
		socket.SetNonBlockingMode(true);
	}
	catch (std::runtime_error)
	{
		std::cout << "Could not make a connection" << std::endl;
	}	
}

std::string NetworkedClient::SendMessage(std::stringstream& stream)
{
	std::string message = stream.str();

	int bytesSent = socket.Send(message.data(), message.size());

	std::string buffer(4096, '\0');

	int bytesRecieved = socket.Recv(buffer.data(), buffer.size());
	buffer.resize(bytesRecieved);

	std::cout << std::endl << buffer << std::endl;

	return buffer;
}

void NetworkedClient::RunNetworkedUpdate()
{
	//Send Message
	std::string message = "HELLO!";

	std::stringstream stream(message);
	SendMessage(stream);

	char* recvMessage;

	socket.Recv(recvMessage, 4098);

	std::cout << recvMessage << std::endl;
}


