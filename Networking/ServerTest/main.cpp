
#include <Engine/Network.h>
#include <iostream>
#include <vector>

using namespace Engine::Network;
using namespace std;

struct MyPacket {
	int number;
	std::string message;
};

void onClientDisconnect(NetSocket client);
void onClientLostConnection(NetSocket client);

std::vector<NetSocket> clients;

void onServerReceiveData(char* data, int len) {
	MyPacket *pack = (MyPacket*)(data);
	cout << "Received packet with message: " << pack->message << endl;
}

void onClientConnect(NetSocket client) {
	
	client.SetLostConnectionCallback(onClientLostConnection);
	client.SetDisconnectedCallback(onClientDisconnect);

	clients.push_back(client);

	cout << "New client connected: " << client.GetPeer() << endl;
}

void onClientDisconnect(NetSocket client) {
	cout << "Client disconnected" << endl;

	auto it = std::find(clients.begin(), clients.end(), client);
	assert(it != clients.end());
	if(it != clients.end())
		clients.erase(it);

	cout << endl;
}

void onClientLostConnection(NetSocket client) {

	cout << "Client lost connection " << endl;

	auto it = std::find(clients.begin(), clients.end(), client);
	assert(it != clients.end());
	if (it != clients.end())
		clients.erase(it);

	cout << endl;
}





/*

Server socket is only used for receiving incoming connections
that are fed into callback function. Therefore if you want to send
data to clients you need to use the client connection object passed
as a parameter to said callback.

*/

int main() {
	cout << "Starting server" << endl;

	auto hostSocket = NetSocket(ProtocolType::Reliable, 1337);
	hostSocket.SetNewConnectionCallback(onClientConnect);
	hostSocket.SetDisconnectedCallback(onClientDisconnect);
	hostSocket.SetLostConnectionCallback(onClientLostConnection);

	cout << "Listening on port " << hostSocket.GetPort() << endl;
	while (true) {
		hostSocket.Listen();
		
		for (auto& c : clients) {
			c.Poll();
			
		}

		Sleep(100);
	}
	cout << "Shutting down server" << endl;
	
	NetSocket::Cleanup();
	return 0;
}