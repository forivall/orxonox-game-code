/*
 *Class generates packets that can be send by enet
 * ->don't read this without the class PacketDecoder, since they belong together
 * 
 * Autor: Dumeni Manatschal
 *  
*/

#include "PacketManager.h"
#include <enet/enet.h>
#include <iostream>
#include <list>
#include <string>
#include <cstring>

using namespace network;

PacketGenerator::PacketGenerator() {}

//following functions create a packet in form of bytestream

ENetPacket* PacketGenerator::acknowledgement( int state, int reliable )
{
    std::cout << "generating new acknowledgement" << std::endl;
	ack* ackreq = new ack;
	ackreq->id = ACK;
	ackreq->a = state;
	
	ENetPacket *packet = enet_packet_create( ackreq , sizeof( *ackreq ), reliable );
	
	return packet;
}
/*### mouseupdates */
ENetPacket* PacketGenerator::mousem( double x, double y, int reliable )
{
	std::cout << "generating new mouse" << std::endl;
	mouse* mousemove = new mouse;
	mousemove->id = MOUSE;
	mousemove->x = x;
	mousemove->y = y;
	
	ENetPacket *packet = enet_packet_create( mousemove , sizeof( *mousemove ), reliable );
	
	return packet;
}
/*### keystrikes updates */
ENetPacket* PacketGenerator::keystrike( char press, int reliable )
{
	std::cout << "generating new keyboard" << std::endl;
	keyboard* key = new keyboard;
	key->id = KEYBOARD;
	key->press = press;
	
	ENetPacket *packet = enet_packet_create( key , sizeof( *key ), reliable );
	
	return packet;
}
/*### chat messages packet */
ENetPacket* PacketGenerator::chatMessage( const char* message, int reliable )
{
	int* trans = new int[sizeof(int) + strlen(message) + 1];
	*trans = CHAT;
	//be carefull here, don't forget to allocate the space before using it ;-)
	memcpy( &trans[1], (const void*)message, strlen( message ) + 1);
	ENetPacket *packet = enet_packet_create( trans , sizeof( int ) + strlen( message ) + 1, reliable );
	
	return packet;
}

/*### gamestate packet */
ENetPacket* PacketGenerator::gstate( GameStateCompressed* states, int reliable )
{
	int* gid = new int; 
        *gid = GAMESTATE; //first assign the correct enet id
	int totalLen = 4*sizeof( int ) + sizeof(bool) + states->compsize; //calculate the total size of the datastream memory
	unsigned char* data = (unsigned char*)malloc( totalLen ); //allocate the memory for datastream
	memcpy( (void*)(data), (const void*)gid, sizeof( int ) ); //this is the enet id
	memcpy( (void*)(data+sizeof(int)), (const void*)&(states->id), sizeof(int) ); //the GameStateCompressed id
	//this is the compressed size of the GameStateCompressed data, place at 3th position of the enet datastream
	memcpy( (void*)(data+2*sizeof(int)), (const void*)&(states->compsize), sizeof(int));
	//this is the uncompressed size of GameStateCompressed data
	memcpy( (void*)(data+3*sizeof(int)), (const void*)&(states->normsize), sizeof(int));
	//since there is a new parameter inside GameStateCompressed, change this function to create packet
	memcpy( (void*)(data+4*sizeof(int)), (const void*)states->diffed, sizeof(bool));
	//place the GameStateCompressed data at the end of the enet datastream
	memcpy( (void*)(data+4*sizeof( int ) + sizeof(bool)), (const void*)states->data, states->compsize );
	//create an enet packet with the generated bytestream
	ENetPacket *packet = enet_packet_create( data , totalLen, reliable );
	
	return packet;
}

ENetPacket* PacketGenerator::clid( int classid, std::string classname, int reliable ){
        unsigned char* data = (unsigned char *)malloc(3*sizeof(int)+classname.length()+1);
        *(int *)data = CLASSID;
        *((int *)data+1) = classname.length()+1;
        *((int *)data+2) = classid;
        memcpy( (void *)(data+3*sizeof(int)), classname.c_str(), classname.length()+1);
        ENetPacket *packet = enet_packet_create( data , 3*sizeof(int)+classname.length()+1, reliable );
        return packet;
}


