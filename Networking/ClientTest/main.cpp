
#include <Engine/Network.h>
#include <iostream>

using namespace Engine::Network;
using namespace std;

struct MyPacket {
	int number;
	std::string message;
};

int main() {

	auto sock = NetSocket(ProtocolType::TCP, "127.0.0.1",1337);
	
	//NetBufferedConnection connection = NetBufferedConnection(sock);
	
	cout << "Client connecting" << endl;

	MyPacket packet = MyPacket { 42, std::string("Hello sailor!") };

	if(sock.IsOpen())
	{
		auto i = 0;
		while (i++ < 10) {
			cout << "Sending packet...";

			cout << endl;
			Sleep(1000);
		}
		sock.Close();
	}
	else {
		cout << "Error connecting" << endl;
	}
	NetSocket::Cleanup();
	return 0;
}