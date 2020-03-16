#include <Engine/Networking/NetSocket.h>

#include <iostream>
#ifdef WIN32
#include <WS2tcpip.h>

#define SOCK_NOTINIT		"Network error: WSAStartup() must be called before creating a socket"
#define SOCK_ENETDOWN		"Network error: Network subsystem on the host machine is down"
#define SOCK_AFNOTSUPPORTED	"Network error: IPv4 is not supported on host machine"
#define SOCK_BLOCKING		"Network error: Windows sockets is busy"
#define SOCK_UNABLETOCREATE	"Network error: Maximum number of sockets on host machine reached"
#define SOCK_INVALID		"Network error: Invalid parameters given to windows socket"
#define SOCK_PROVIDER		"Network error: Unsupported network provider"
#define SOCK_BUF			"Network error: No buffer could be allocated"
#define SOCK_PROCTAB		"Network error: Invalid procedure table from network provider"
#define SOCK_UNSPROTO		"Network error: Unsupported network protocol"
#define SOCK_PROTO			"Network error: Incorrect protocol for socket type"
#define SOCK_PROINIT		"Network error: Network provider failed to initialize"
#define SOCK_TYPE			"Network error: Unsupported socket-type in this address family"
#define SOCK_WSAINIT		"Network error: Network subsystem is not ready"
#define SOCK_WSAVER			"Network error: Unsupported version"
#define SOCK_PROCLIM		"Network error: Number of tasks limit reached by windows sockets"
#define SOCK_EFAULT			"Network error: Invalid WSADATA parameter"

#else

#define SOCK_NOTINIT		"Unsupported platform"
#define SOCK_ENETDOWN		"Unsupported platform"
#define SOCK_AFNOTSUPPORTED "Unsupported platform"
#define SOCK_BLOCKING		"Unsupported platform"
#define SOCK_UNABLETOCREATE "Unsupported platform"
#define SOCK_INVALID		"Unsupported platform"
#define SOCK_PROVIDER		"Unsupported platform"
#define SOCK_BUF			"Unsupported platform"
#define SOCK_PROCTAB		"Unsupported platform"
#define SOCK_UNSPROTO		"Unsupported platform"
#define SOCK_PROTO			"Unsupported platform"
#define SOCK_PROINIT		"Unsupported platform"
#define SOCK_TYPE			"Unsupported platform"
#define SOCK_WSAINIT		"Unsupported platform"
#define SOCK_WSAVER			"Unsupported platform"
#define SOCK_PROCLIM		"Unsupported platform"
#define SOCK_EFAULT			"Unsupported platform"

#endif

using namespace Engine::Network;

bit NetSocket::m_subsystem_init = false;

NetSocket::NetSocket() {
	if (!m_subsystem_init) Init();
	m_socket = 0;
	m_client_socket = 0;
	m_port = 0;
	m_is_socket_open = false;
	m_blocking_calls = false;
	m_error_str = nullptr;
	m_peer_name = nullptr;
	m_con_callback = nullptr;
	m_dis_callback = nullptr;
	m_lost_con_callback = nullptr;
	m_data_callback = nullptr;
	m_protocol_type = ProtocolType::TCP;
}



NetSocket::NetSocket(net_socket socket) : NetSocket() {
	if (!m_subsystem_init) Init();
	// Assuming a client connection was passed
	m_client_socket = socket;
	m_is_socket_open = true; // Perhaps sanity-check this connection

#ifdef WIN32
	struct sockaddr_in peeraddr;
	int size = sizeof(peeraddr);
	auto result = getpeername(m_client_socket, (struct sockaddr*)&peeraddr, &size);
	if (CheckErrors(result)) {
		std::cout << m_error_str << std::endl;
		return;
	}

	char* ip = inet_ntoa(peeraddr.sin_addr);
	m_peer_name = ip;
	m_port = peeraddr.sin_port;
#endif

}

NetSocket::NetSocket(ProtocolType type, u16 port) : NetSocket() {

	m_protocol_type = type;
	m_port = port;
	
#ifdef WIN32
	
	struct addrinfo *result = NULL, *ptr = NULL, hints;
	
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = type == ProtocolType::TCP ? SOCK_STREAM : SOCK_DGRAM;
	hints.ai_protocol = type == ProtocolType::TCP ? IPPROTO::IPPROTO_TCP : IPPROTO::IPPROTO_UDP;
	hints.ai_flags = AI_PASSIVE; // Listening
	
	auto iresult = getaddrinfo(NULL, std::to_string(port).c_str(), &hints, &result);
	if (CheckErrors((int)iresult)) {
		std::cout << "Error getting address info" << std::endl; return;
	}

	// Open up the socket
	m_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (CheckErrors((int)m_socket)) {
		std::cout << "Error opening up socket" << std::endl; return;
	}

	UpdateBlocking();

	// Bind to ip
	iresult = bind(m_socket, result->ai_addr, (int)result->ai_addrlen);
	if (CheckErrors(iresult)) {
		std::cout << "Error binding to port" << std::endl; 
		freeaddrinfo(result);
		Close(false);
		return;
	}
	freeaddrinfo(result);

	auto result2 = listen(m_socket, SOMAXCONN);
	if (CheckErrors((int)result2)) {
		std::cout << "Error listening on port" << std::endl;
		Close(false);
		return;
	}

	m_is_socket_open = true;

	std::cout << "Created listener socket!" << std::endl;
#else
	std::cerr << "Network error: unsupported platform" << std::endl;
#endif
}

NetSocket::NetSocket(ProtocolType type, std::string ip, u16 port) : NetSocket() {
	m_protocol_type = type;
	m_port = port;

	std::cout << "Connecting to " << ip << std::endl;

#ifdef WIN32
	struct addrinfo* result = NULL, * ptr = NULL, hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = type == ProtocolType::TCP ? SOCK_STREAM : SOCK_DGRAM;
	hints.ai_protocol = type == ProtocolType::TCP ? IPPROTO_TCP : IPPROTO_UDP;
	
	auto iresult = getaddrinfo(ip.c_str(), std::to_string(port).c_str(), &hints, &result);
	if (CheckErrors(iresult)) {
		std::cout << "Error getting address info" << std::endl; return;
	}

	ptr = result;
	// Open up the socket
	m_client_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (CheckErrors((int)m_client_socket)) {
		std::cout << "Error opening up socket" << std::endl; freeaddrinfo(result); return;
	}

	std::cout << "Socket opened" << std::endl;

	iresult = connect(m_client_socket, ptr->ai_addr, (int)ptr->ai_addrlen);
	if (CheckErrors((int)iresult)) {
		std::cout << "Error connecting to ip" << std::endl;
		freeaddrinfo(result);
		Close();
		return;
	}
	m_peer_name = _strdup(ip.c_str());

	freeaddrinfo(result);

	if (m_socket == INVALID_SOCKET) {
		std::cout << "Error connecting to server" << std::endl;
		Close();
		return;
	}
	std::cout << "Socket connected" << std::endl;
	m_is_socket_open = true;
#else
	std::cout << "Network error: Unsupported platform" << std::endl;
#endif
}

NetSocket::~NetSocket() {}

void NetSocket::Dispose() {
	if (m_peer_name) free(m_peer_name);
	m_peer_name = nullptr;
	if (m_data_callback != nullptr) delete m_data_callback;
	if (m_con_callback != nullptr) delete m_con_callback;
	if (m_dis_callback != nullptr) delete m_dis_callback;
	if (m_lost_con_callback != nullptr) delete m_lost_con_callback;
}

bool NetSocket::Listen() {
	// Return if server socket isn't open or we have a connection here
	if (!m_is_socket_open || m_client_socket != 0) return false;
#ifdef WIN32

	FD_SET read_set;
	FD_ZERO(&read_set);

	TIMEVAL timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 1;

	// Add server socket to read_set
	FD_SET(m_socket, &read_set);
	if (select(0, &read_set, NULL, NULL, &timeout) == SOCKET_ERROR) {
		m_error_str = "Network error: socket connection check returned with an error!";
		Close(false);
		return false;
	}

	
	// new connection on our socket
	if(FD_ISSET(m_socket, &read_set))
	{

		m_client_socket = accept(m_socket, NULL, NULL);
		if (m_client_socket == INVALID_SOCKET) {
			m_error_str = "Network error: Error accepting connection";
			Close(false);
			return false;
		}

		// Mark socket non-blocking
		unsigned long NonBlock = 1;
		if(!m_blocking_calls) NonBlock = 0;
		if (ioctlsocket(m_client_socket, FIONBIO, &NonBlock) == SOCKET_ERROR) {
			m_error_str = "Network error: Error setting socket to non-blocking state";
			Close(false);
			return false;
		}

		if (m_con_callback != nullptr) {
			(*m_con_callback)(NetSocket(m_client_socket));
			m_client_socket = 0; // Continue to listen for new connections on this socket
			// User must handle the given socket for himself
		}
		return true;
	}

#endif

	return false;
}

int NetSocket::Poll() {
	if(!m_is_socket_open) return 0;
#ifdef WIN32
	FD_SET read_set;
	FD_ZERO(&read_set);

	auto current_socket = m_socket;
	if (m_client_socket != 0 && m_socket == 0)
		current_socket = m_client_socket;

	// Add server socket to read_set
	FD_SET(current_socket, &read_set);
	if (select(0, &read_set, NULL, NULL, NULL) == SOCKET_ERROR) {
		m_error_str = "Network error: socket connection check returned with an error!";
		
		Close(false);
		return -1;
	}

	// Data to read
	if (FD_ISSET(current_socket, &read_set)) {
		std::cout << "Data to read" << std::endl;
		char buffer[8192];
		auto result = recv(current_socket, buffer, 8192, 0);
		if (CheckErrors(result)) {
			std::cout << "Error receiving data" << std::endl;
			std::cout << m_error_str << std::endl;
			Close(false);
			return -1;
		}
		if (result == 0) {
			// Connection closed
			//std::cout << "Connection gracefully closed" << std::endl;
			Close();
		}
		else {
			std::cout << "calling callback with data " << result << std::endl;
			// Received data
			if (m_data_callback != nullptr)
				(*m_data_callback)(&buffer[0], result);
		}
		return result;
	}
	
#endif
	return 0;
}

int NetSocket::Send(char* data, int length) {
#ifdef WIN32
	if (length > 8192) {
		m_error_str = "Network error: too big of a packet, limit 8192";
		std::cout << m_error_str << std::endl;
		Close(false);
		return -1;
	}
	if (m_client_socket == 0) {
		m_error_str = "Network error: No client is connected";
		std::cout << m_error_str << std::endl;
		Close(false);
		return -1;
	}

	auto result = send(m_client_socket, data, length, 0);
	if (CheckErrors(result)) {
		std::cout << "Error sending data" << std::endl;
		std::cout << m_error_str << std::endl;
		Close(false);
		return -1;
	}
	if (result != length) {
		m_error_str = "Network error: couldn't send all the data";
		std::cout << m_error_str << std::endl;
		Close(false);
		return result;
	}
	else
	{
		std::cout << "Successfully sent " << length << " bytes" << std::endl;
	}
	return length;
#else
	std::cout << "Network send error: unsupported platform" << std::endl;
	return 0;
#endif
}

// Statically initialize WinSock
void NetSocket::Init() {
#ifdef WIN32
	if (!m_subsystem_init) {
		WSADATA data;
		auto result = WSAStartup(MAKEWORD(2, 2), &data); // requesting version 2.2

		//switch (result) {
		//case 0: /* OK */ break;
		//case WSASYSNOTREADY: m_error_str = SOCK_WSAINIT; break;
		//case WSAVERNOTSUPPORTED: m_error_str = SOCK_WSAVER; break;
		//case WSAEINPROGRESS: m_error_str = SOCK_BLOCKING; break;
		//case WSAEPROCLIM: m_error_str = SOCK_PROCLIM; break;
		//case WSAEFAULT: m_error_str = SOCK_EFAULT; break;
		//default: m_error_str = "Network error: WSAStartup returned invalid result"; break;
		//}

		m_subsystem_init = result == 0;
		return;
	}
#endif
}

// Statically dispose WinSock
void NetSocket::Cleanup() {
#ifdef WIN32
	if (!m_subsystem_init) return;
	WSACleanup();
#endif
}

// Closes active socket
void NetSocket::Close(bool graceful_close) {
	if(!m_is_socket_open || m_client_socket == 0) return;
	std::cout << "Close socket called" << std::endl;
#ifdef WIN32
	if (graceful_close && m_client_socket != 0 && m_dis_callback != nullptr)
		(*m_dis_callback)(*this);
	else if(!graceful_close && m_client_socket != 0 && m_lost_con_callback != nullptr)
		(*m_lost_con_callback)(*this);

	if(m_is_socket_open)
		closesocket(m_socket);
	if (m_client_socket != 0)
		closesocket(m_client_socket);
#endif
	m_is_socket_open = false;
	m_socket = m_client_socket = 0;
}

void NetSocket::SetNewConnectionCallback(std::function<void(NetSocket)> cb)
{ 
	if (m_con_callback != nullptr) delete m_con_callback;
	m_con_callback = new std::function<void(NetSocket)>();
	*m_con_callback = cb;
}

void NetSocket::SetDisconnectedCallback(std::function<void(NetSocket)> cb)
{
	if (m_dis_callback != nullptr) delete m_dis_callback;
	m_dis_callback = new std::function<void(NetSocket)>();
	*m_dis_callback = cb;
}

void NetSocket::SetLostConnectionCallback(std::function<void(NetSocket)> cb) {
	if(m_lost_con_callback != nullptr) delete m_lost_con_callback;
	m_lost_con_callback = new std::function<void(NetSocket)>();
	*m_lost_con_callback = cb;
}

void NetSocket::SetOnDataReceivedCallback(std::function<void(char* data, int size)> cb)
{ 
	if(m_data_callback != nullptr) delete m_data_callback;
	std::cout << "Setting receive cb for socket " << m_client_socket << std::endl;
	m_data_callback = new std::function<void(char* data, int size)>();
	*m_data_callback = cb;
}

bool NetSocket::CheckErrors(int result) {
#ifdef WIN32
	if (result != SOCKET_ERROR && result != INVALID_SOCKET) return false;
	switch (WSAGetLastError()) {
	case WSANOTINITIALISED:			m_error_str = SOCK_NOTINIT; break;
	case WSAENETDOWN:				m_error_str = SOCK_ENETDOWN; break;
	case WSAEAFNOSUPPORT:			m_error_str = SOCK_AFNOTSUPPORTED; break;
	case WSAEINPROGRESS:			m_error_str = SOCK_BLOCKING; break;
	case WSAEMFILE:					m_error_str = SOCK_UNABLETOCREATE; break;
	case WSAEINVAL:					m_error_str = SOCK_INVALID; break;
	case WSAEINVALIDPROVIDER:		m_error_str = SOCK_PROVIDER; break;
	case WSAEINVALIDPROCTABLE:		m_error_str = SOCK_BUF; break;
	case WSAENOBUFS:				m_error_str = SOCK_PROCTAB; break;
	case WSAEPROTONOSUPPORT:		m_error_str = SOCK_UNSPROTO; break;
	case WSAEPROTOTYPE:				m_error_str = SOCK_PROTO; break;
	case WSAEPROVIDERFAILEDINIT:	m_error_str = SOCK_PROINIT; break;
	case WSAESOCKTNOSUPPORT:		m_error_str = SOCK_TYPE; break;
	case WSAEINTR:					m_error_str = "Network error: The blocking call was cancelled"; break;
	case WSAENOTCONN:				m_error_str = "Network error: Not connected"; break;
	case WSAENETRESET:				m_error_str = "Network error: Connection was broken"; break;
	case WSAENOTSOCK:				m_error_str = "Network error: Descriptor is not a socket"; break;
	case WSAEOPNOTSUPP:				m_error_str = "Network error: OOB was specified but socket is not stream style"; break;
	case WSAEMSGSIZE:				m_error_str = "Network error: message buffer overflow error"; break;
	case WSAESHUTDOWN:				m_error_str = "Network error: Connection shut down"; break;
	case WSAEWOULDBLOCK:			m_error_str = "Network error: Would block"; break;
	case WSAETIMEDOUT:				m_error_str = "Network error: Connection timed out"; break;
	case WSAECONNRESET:				m_error_str = "Network error: Connection reset"; break;
	case WSAECONNABORTED:			m_error_str = "Network error: Connection aborted"; break;
	default:						m_error_str = "Network error: Unknown network error"; break;
	}
	return true;
#else
	return false;
#endif
}

std::string NetSocket::GetPeerName() const {
	if (!m_is_socket_open || m_socket != 0 || m_peer_name == nullptr) return std::string();
	return std::string(m_peer_name);
}

unsigned short NetSocket::GetPort() const {
	return m_port;
}

ProtocolType NetSocket::GetProtocol() const {
	return m_protocol_type;
}

net_socket NetSocket::GetPeer() const {
	return m_client_socket;
}

void NetSocket::SetBlocking(bool blocking) {
	m_blocking_calls = blocking;
	UpdateBlocking();
}

void NetSocket::UpdateBlocking() {
	if (!m_is_socket_open) return;
	
	net_socket con = m_socket;
	if (m_socket == 0) con = m_client_socket;

#ifdef WIN32
	// Mark socket non-blocking
	unsigned long NonBlock = !m_blocking_calls;
	if (ioctlsocket(m_socket, FIONBIO, &NonBlock) == SOCKET_ERROR) {
		m_error_str = "Network error: Error setting socket to non-blocking state";
		std::cout << m_error_str << std::endl;
		Close();
		return;
	}
#endif
}