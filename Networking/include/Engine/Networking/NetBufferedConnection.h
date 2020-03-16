#pragma once

#if false
#include <Engine/Networking/NetworkAPI.h>
#include <Engine/Networking/NetSocket.h>
#include <Engine/Library/Common.h>
#include <queue>

namespace Engine::Network {
#define NET_BUFFERED_SIZE 8096
#define NET_MAX_BUFFERED_PACKET_COUNT 128

	typedef struct Packet {
		u16 size;
		char* data;
	};

	class NETWORK_API NetBufferedConnection {
	public:
		NetBufferedConnection(NetSocket socket) {
			//m_packets = Queue<Packet>();
			m_socket = socket;
			//std::cout << "Initialized buffered connection object on socket " << socket.GetPeer() << std::endl;
			m_socket.SetOnDataReceivedCallback(std::bind(&NetBufferedConnection::OnReceive, this, std::placeholders::_1, std::placeholders::_2));
		}

		~NetBufferedConnection() {
		}

		bool ReceivePacket(Packet& packet) {
			// Check next packet for correct length for given type
			// If size doesn't match then return false
			if(m_packets.size() <= 0) return false;
			packet = m_packets.back();
			m_packets.pop();
			return true;
			//return m_packets(packet);
		}

		void Poll() {
			assert(m_socket.IsOpen());

			//std::cout << "Polling client socket" <<std::endl;
			m_socket.Poll();
		}

		bool SendPacket(Packet packet) {
			return m_socket.Send(&packet.data[0], packet.size);
		}

		bool operator==(const NetSocket& other) const {
			return other == m_socket;
		}
		
	private:
	
		void OnReceive(char* d, int le) {
			if (le <= 0) return;
			
			m_packets.push(Packet { (u16)le, d });
		}

		NetSocket m_socket;
		std::queue<Packet> m_packets;
		
	};


}
#endif