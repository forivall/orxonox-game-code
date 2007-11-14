#ifndef PACKETMANAGER_H_
#define PACKETMANAGER_H_

#include <enet/enet.h>

namespace network
{
enum packet_id {
	ACK,
	MOUSE,
	KEYBOARD,
	CHAT
};

class PacketGenerator
{
public:
	PacketGenerator();
	ENetPacket* acknowledgement( int state, int reliable = ENET_PACKET_FLAG_RELIABLE );
	ENetPacket* mousem( double x, double y, int reliable = ENET_PACKET_FLAG_RELIABLE );
	ENetPacket* keystrike( char press, int reliable = ENET_PACKET_FLAG_RELIABLE );
	ENetPacket* chatMessage( const char* message, int reliable = ENET_PACKET_FLAG_RELIABLE );
private:
	struct ack {
		int id;
		int a;
	};

	struct mouse {
		int id;
		double x;
		double y;
	};

	struct keyboard {
		int id;
		char press;
	};	
};

class PacketDecoder
{
public:
	PacketDecoder();
	bool elaborate( ENetPacket* packet, int clientId );
private:
	struct ack {
		int id;
		int a;
	};

	struct mouse {
		int id;
		double x;
		double y;
	};

	struct keyboard {
		int id;
		char press;
	};
	struct chat {
		int id;
		const char* message;
	};
	
	void acknowledgement( ENetPacket* packet );
	void mousem( ENetPacket* packet );
	void keystrike( ENetPacket* packet );
	void chatMessage( ENetPacket* packet );
	
	//print functions
	void printAck( ack* data );
	void printMouse( mouse* data );
	void printKey( keyboard* data );
	void printChat( chat* data );
	void printPeer( ENetPeer* peer );
};
}

#endif /*PACKETMANAGER_H_*/
