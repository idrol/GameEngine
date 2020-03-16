#pragma once
#if 0

#include <iostream>
#include <Engine/Library/Common.h>
#include <Engine/Networking/NetSocket.h>
#include <Engine/Networking/NetworkAPI.h>



namespace Engine::Network {

typedef net_socket NetClient;
typedef net_socket NetHost;
template<typename T>
class NetChannel;

	class NETWORK_API NetSocketComparator {
		NetSocket client;
	public:
		NetSocketComparator(NetSocket th) :client(th) {}
		bool operator()(NetSocket other) const {
			return client.GetPeerName() == other.GetPeerName() && client.GetPort() == other.GetPort();
		}
	};
	class NETWORK_API NetSocketChannelComparator {
		NetSocket client;
	public:
		NetSocketChannelComparator(NetSocket sock) : client(sock) {}
		template<typename T>
		bool operator()(NetChannel<T> ch) const {
			return ch.GetClient() == client.GetPeer() && ch.GetPort() == client.GetPort();
		}
	};

#define NET_BUFFER_SIZE		512
#define NET_RECONNECT_TRIES 5

	template<typename T>
	class NETWORK_API NetChannel {
	public:

		NetChannel(ProtocolType netType = ProtocolType::Reliable, 
				std::function<void(NetChannel<T>)> OnClientDisconnect = nullptr,
				std::function<void(NetChannel<T>)> OnLostConnection = nullptr,
				std::function<void(T)> OnData = nullptr) {
			m_protocol = netType;
			m_buffer = nullptr;
			m_socket = NetSocket();
			m_socket.SetBlocking(false);
			m_packet_waiting_count = 0;
			m_discon_cb = OnClientDisconnect;
			m_lost_con_cb = OnLostConnection;
			m_data_cb = OnData;
			m_buf_front = m_buf_back = 0;
		}

		NetChannel(NetSocket socket,
			std::function<void(NetChannel<T>)> OnClientDisconnect = nullptr,
			std::function<void(NetChannel<T>)> OnLostConnection = nullptr,
			std::function<void(T)> OnData = nullptr) : NetChannel(socket.GetProtocol(), OnClientDisconnect, OnLostConnection, OnData)
		{
			m_buffer = nullptr;
			m_protocol = socket.GetProtocol();

			m_socket = socket;
		}
		
		// Send a packet along the "tube"
		template<typename T>
		bool Send(T packet) {
			if(!IsConnected()) return false;

			auto count = m_socket.Send((char*)(&packet), sizeof(T));

			return count == sizeof(T);
		}

		// Receive a packet if one is waiting
		template<typename T>
		bool Receive(T& packet) {
			m_socket.Poll();
			if (m_buf_front <= m_buf_back) return false;
			if (m_buffer == nullptr) m_buffer = new T[NET_BUFFER_SIZE];

			packet = m_buffer[m_buf_back++];
			if (m_buf_back >= NET_BUFFER_SIZE)
				m_buf_back = 0;
			
			return true;
		}

		// Connect to a host ip
		void Connect(string public_ip, u16 port) {
			if(m_socket.IsOpen() || m_socket.IsConnection()) m_socket.Close();
			m_socket.Dispose();
			if(m_buffer == nullptr) m_buffer = new T[NET_BUFFER_SIZE];
			m_socket = NetSocket(m_protocol, public_ip, port);
			m_socket.SetDisconnectedCallback(std::bind(&NetChannel::ClosedConnection, this, std::placeholders::_1));
			m_socket.SetLostConnectionCallback(std::bind(&NetChannel::LostConnection, this, std::placeholders::_1));
			m_socket.SetOnDataReceivedCallback(std::bind(&NetChannel::DataReceived, this, std::placeholders::_1, std::placeholders::_2));
			m_packet_waiting_count = 0;
			m_buf_front = m_buf_back = 0;
		}

		// Reconnect to client/server if connection was lost
		bool Reconnect() {
			if(!m_socket.IsConnection()) return false;
			if(m_socket.IsOpen()) return true;
			// Get the ip and port before we dispose of the socket just to be sure
			auto port = m_socket.GetPort();
			auto ip = m_socket.GetPeer();
			if (m_buffer == nullptr) m_buffer = new T[NET_BUFFER_SIZE];
			m_socket.Dispose();
			m_socket = NetSocket(m_protocol, ip, port);
			m_socket.SetDisconnectedCallback(std::bind(&NetChannel::ClosedConnection, this, std::placeholders::_1));
			m_socket.SetLostConnectionCallback(std::bind(&NetChannel::LostConnection, this, std::placeholders::_1));
			m_socket.SetOnDataReceivedCallback(std::bind(&NetChannel::DataReceived, this, std::placeholders::_1, std::placeholders::_2));
			return m_socket.IsOpen();
		}

		// Close active connection gracefully
		void Disconnect() {
			if(!m_socket.IsOpen()) return;
			m_socket.Close();
			m_socket.Dispose();
			delete m_buffer;
			m_buffer = nullptr;
		}

		// Is this channel open?
		bool IsConnected() const {
			// In this case we don't include m_is_host since this channel can be a connection where this end is
			// declared as 'host', this is not reflected in the connection properties tho as listening sockets
			// make new socket connections upon connecting to a client
			return m_socket.IsOpen() && m_socket.IsConnection();
		}

		// Poll socket for incoming packets and connections
		void Poll() {
			m_socket.Poll();
		}

		NetClient GetClient() const {
			if(IsConnected()) return (NetClient)m_socket.GetPeer();
			return 0;
		}

		// Returns the ping time between host-client
		int GetPing() const {
			return -1;
		}

		// Get active connection port
		int GetPort() const {
			if(!m_socket.IsOpen()) return 0;
			return m_socket.GetPort();
		}

		// Get connected peer name
		std::string GetPeer() const {
			if(!m_socket.IsOpen()) return "";
			return m_socket.GetPeerName();
		}


		template<typename T>
		bool operator==(const NetChannel<T>& a) const {
			return a.GetClient() == GetClient() && a.GetClient() != 0 && a.GetPort() == GetPort();
		}


	private:

		template<typename T>
		void ClosedConnection(NetSocket sock) {
			if (m_discon_cb == nullptr) return;
			m_discon_cb(NetChannel<T>(sock, 
								std::bind(&NetChannel::ClosedConnection, this, std::placeholders::_1), 
								std::bind(&NetChannel::LostConnection, this, std::placeholders::_1),
								std::bind(&NetChannel::DataReceived, this, std::placeholders::_1, std::placeholders::_2)));
		}

		template<typename T>
		void LostConnection(NetSocket sock) {
			if (m_lost_con_cb == nullptr) return;
			m_lost_con_cb(NetChannel<T>(sock, 
								std::bind(&NetChannel::ClosedConnection, this, std::placeholders::_1), 
								std::bind(&NetChannel::LostConnection, this, std::placeholders::_1),
								std::bind(&NetChannel::DataReceived, this, std::placeholders::_1, std::placeholders::_2)));
		}

		void DataReceived(char* ptr, int len) {
			
			// Copy data to buffer
			if (len < sizeof(T)) {
				std::cerr << "ERROR! Packet received didn't match expected datatype length, packet is discarded" << std::endl;
				free(ptr);
				return;
			}
			if (m_buffer == nullptr) m_buffer = new T[NET_BUFFER_SIZE];
			// To be sure, copy only one instance
			auto buf_addr = m_buf_front;
			memcpy(&m_buffer[m_buf_front++], ptr, min(len, sizeof(T)));

			// Push front pointer one step
			if(m_buf_front >= NET_BUFFER_SIZE) m_buf_front = 0;
			if (m_buf_front >= m_buf_back) {
				std::cerr << "Warning! Packets are lost to buffer overflow" << std::endl;
				m_buf_back++;
				if(m_buf_back >= NET_BUFFER_SIZE) m_buf_back = 0;
			}

			// call callback
			if (m_data_cb == nullptr || m_buffer == nullptr) return;
			m_data_cb(m_buffer[buf_addr]);
		}

		T				*m_buffer;
		u32				m_packet_waiting_count;
		u32				m_buf_front, m_buf_back;
		std::function<void(NetChannel)> m_lost_con_cb;
		std::function<void(NetChannel)> m_discon_cb;
		std::function<void(T)> m_data_cb;
		ProtocolType	m_protocol;
		NetSocket		m_socket;
	};
}

#endif