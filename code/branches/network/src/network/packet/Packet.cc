
#include <enet/enet.h>

#include "Packet.h"
#include "network/ConnectionManager.h"
#include "network/ClientInformation.h"

#include "Acknowledgement.h"
#include "Chat.h"
#include "ClassID.h"
#include "Gamestate.h"
#include "network/Host.h"
#include "core/CoreIncludes.h"

namespace network{

namespace packet{
  
#define _PACKETID           0
  

  
Packet::Packet(){
  packetDirection_ = ENUM::Outgoing;
  clientID_=0;
  packetContent_=0;
  enetPacket_=0;
}

Packet::Packet(PacketContent *content){
  packetDirection_ = ENUM::Outgoing;
  clientID_=0;
  packetContent_=content;
  enetPacket_=0;
}

Packet::Packet(ENetPacket *packet, ENetPeer *peer){
  packetDirection_ = ENUM::Incoming;
  enetPacket_ = packet;
  clientID_ = ClientInformation::findClient(&peer->address)->getID();
  packetContent_ = createPacketContent(packet);
  enet_packet_destroy(packet);
  delete peer;
}

Packet::~Packet(){
  if(packetContent_)
    delete packetContent_;
  if(enetPacket_){
    enet_packet_destroy(enetPacket_);
  }
}

void Packet::setPacketContent(PacketContent *content){
  packetContent_ = content;
  if(enetPacket_){
    enet_packet_destroy(enetPacket_);
    enetPacket_=0;
  }
}

bool Packet::send(){
  if(packetDirection_ != ENUM::Outgoing && packetDirection_ != ENUM::Bidirectional )
    return false;
  if(!enetPacket_){
    if(!packetContent_)
      return false;
    COUT(3) << "creating packet with data: " << packetContent_->getData() << " size: " << packetContent_->getSize() << " and flags: " << packetContent_->getFlags() << std::endl;
    enetPacket_ = enet_packet_create(packetContent_->getData(), packetContent_->getSize(), packetContent_->getFlags());
  }
  network::Host::addPacket( enetPacket_, clientID_);
  enetPacket_ = 0; // otherwise we have a double free because enet already handles the deallocation of the packet
  return true;
}

PacketContent *Packet::createPacketContent(ENetPacket *packet){
  unsigned char *data = packet->data;
  switch( *(ENUM::Type *)&data[_PACKETID] )
  {
    case ENUM::Acknowledgement:
      packetContent_ = new Acknowledgement( data, clientID_ );
      break;
    case ENUM::Chat:
      packetContent_ = new Chat( data, clientID_ );
      break;
    case ENUM::ClassID:
      packetContent_ = new ClassID( data, clientID_ );
      break;
    case ENUM::Gamestate:
      // TODO: remove brackets
      packetContent_ = new Gamestate( data, true, clientID_ );
      break;
    default:
      assert(0);
  }
  return packetContent_;
}

} // namespace packet

} // namespace network

