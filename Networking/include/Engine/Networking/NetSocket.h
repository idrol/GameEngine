#pragma once

#include <string>
#include <functional>
#include <Engine/Networking/NetworkAPI.h>

#ifdef WIN32
#include <WinSock2.h>

typedef SOCKET net_socket;

#else

// TODO Cross platform implementations
typedef uintptr_t net_socket;

#endif

#include <Engine/Library/Common.h>



namespace Engine::Network {

	enum class NETWORK_API ProtocolType : int
	{ 
		// Guaranteed packet reception on both ends
		TCP = 0, 
		// No confirmation for packet received, use with care
		UDP = 1,
		// Same as TCP, packets are guaranteed to be received
		Reliable = 0, 
		// Same as UDP, no guarantee on package reception
		UnReliable = 1
	};

	class NETWORK_API NetSocket {
	public:
		NetSocket();

		// Create NetSocket out of existing socket
		explicit NetSocket(net_socket socket);

		// Open a new socket connection on port and start listening, using specified protocol
		NetSocket(ProtocolType type, u16 port);
		
		// Open a new socket connection to given IP and port
		NetSocket(ProtocolType type, std::string ip_addr, u16 port);

		// Closes any existing connections and frees resources
		~NetSocket();

		// When a new connection is made, run callback function
		void SetNewConnectionCallback(std::function<void(NetSocket)> cb);

		// When connection is gracefully closed, call this callback
		void SetDisconnectedCallback(std::function<void(NetSocket)> cb);

		// When connection is unexpectedly interrupted, call this callback
		void SetLostConnectionCallback(std::function<void(NetSocket)> cb);

		// Set callback when data is received on this connection
		void SetOnDataReceivedCallback(std::function<void(char* data, int size)> cb);

		// Check if we have an incoming connection in a non-blocking manner
		// New connections are sent to callback function if one is registered
		// If noone exists, the server socket will be closed and replaced with
		// incoming connection, thus making this a client-connection.
		bool Listen();

		// Check socket data received, callback is ran if there is data, otherwise this does nothing
		int Poll();

		// Send data over this connection, current limit of packetsize: 8kb
		int Send(char* data, int length);

		// Is this socket currently open? 
		bool IsOpen() const { return m_is_socket_open; }
		
		// Returns last errormessage generated, 0 if OK
		const char* GetErrorMsg() const { return m_error_str; }

		// Is this a client to server connection? 
		// Otherwise it's just a server port listening for new connections
		bool IsConnection() const { return m_client_socket != 0; }

		// Is this socket a listening socket for new connections?
		bool IsListening() const { return !IsConnection(); }

		// Enable/Disable blocking Listen() and Poll() calls
		void SetBlocking(bool blocking);

		// Returns a copy of the peer connection name, otherwise empty
		std::string GetPeerName() const;

		// Returns port used in this socket connection, short maxvalue if it could not be resolved
		unsigned short GetPort() const;

		// Get the protocol this socket is using
		ProtocolType GetProtocol() const;

		// Returns the underlying peer socket
		net_socket GetPeer() const;

		// Close socket connection
		void Close(bool graceful_close = true);

		// Dispose of current socket
		void Dispose();

		

		// Release global resources
		static void Cleanup();

		bool operator==(const NetSocket& other) const {
			return m_socket == other.m_socket && m_client_socket == other.m_client_socket && m_port == other.m_port;
		}

	private:
		// Initialize network interface globally (windows stuff)
		static void Init();

		bool CheckErrors(int result);

		void UpdateBlocking();

		ProtocolType	m_protocol_type;
		
		net_socket		m_socket;
		net_socket		m_client_socket;
		
		u16				m_port;
		bit				m_is_socket_open;
		const char*		m_error_str;
		static bit		m_subsystem_init;
		bit				m_blocking_calls;

		char*			m_peer_name;

		std::function<void(NetSocket)>   *m_con_callback;
		std::function<void(NetSocket)>   *m_dis_callback;
		std::function<void(NetSocket)>	 *m_lost_con_callback;
		std::function<void(char* data, int size)>   *m_data_callback;

	};

}