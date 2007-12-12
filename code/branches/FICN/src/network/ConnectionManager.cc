//
// C++ Interface: ConnectionManager
//
// Description: The Class ConnectionManager manages the servers conenctions to the clients.
// each connection is provided by a new process. communication between master process and
// connection processes is provided by ...
//
//
// Author:  Oliver Scheuss
//

#include "ConnectionManager.h"

namespace std{
  bool operator< (ENetAddress a, ENetAddress b){
    if(a.host <= b.host)
      return true;
    else
      return false;
  }
}

namespace network{

  boost::thread_group network_threads;
  
  ConnectionManager::ConnectionManager(ClientInformation *head){
    quit=false;
    bindAddress.host = ENET_HOST_ANY;
    bindAddress.port = NETWORK_PORT;
    head_ = head;
  }

  ConnectionManager::ConnectionManager(int port, std::string address, ClientInformation *head){
    quit=false;
    enet_address_set_host (& bindAddress, address.c_str());
    bindAddress.port = NETWORK_PORT;
    head_ = head;
  }

  ConnectionManager::ConnectionManager(int port, const char *address, ClientInformation *head){
    quit=false;
    enet_address_set_host (& bindAddress, address);
    bindAddress.port = NETWORK_PORT;
    head_ = head;
  }

  ENetPacket *ConnectionManager::getPacket(ENetAddress &address){
    if(!buffer.isEmpty())
      return buffer.pop(address);
    else
        return NULL;
  }
  
  ENetPacket *ConnectionManager::getPacket(int &clientID){
    ENetAddress address;
    ENetPacket *packet=getPacket(address);
    ClientInformation *temp =head_->findClient(&address);
    clientID=temp->getID();
    return packet;
  }

  bool ConnectionManager::queueEmpty(){
    return buffer.isEmpty();
  }

  void ConnectionManager::createListener(){
    network_threads.create_thread(boost::bind(boost::mem_fn(&ConnectionManager::receiverThread), this));
//     boost::thread thr(boost::bind(boost::mem_fn(&ConnectionManager::receiverThread), this));
    return;
  }

  bool ConnectionManager::quitListener(){
    quit=true;
    network_threads.join_all();
    return true;
  }

  bool ConnectionManager::addPacket(ENetPacket *packet, ENetPeer *peer){
    if(enet_peer_send(peer, head_->findClient(&(peer->address))->getID() , packet)!=0)
      return false;
    return true;
  }
  
  bool ConnectionManager::addPacket(ENetPacket *packet, int clientID){
    if(enet_peer_send(head_->findClient(clientID)->getPeer(), clientID, packet)!=0)
      return false;
    return true;
  }
  
  bool ConnectionManager::addPacketAll(ENetPacket *packet){
    for(ClientInformation *i=head_->next(); i!=0; i=i->next()){
      if(enet_peer_send(i->getPeer(), i->getID(), packet)!=0)
         return false;
    }
    return true;
  }

  bool ConnectionManager::sendPackets(ENetEvent *event){
    if(server==NULL)
      return false;
    if(enet_host_service(server, event, NETWORK_SEND_WAIT)>=0)
      return true;
    else
      return false;
  }
  
  bool ConnectionManager::sendPackets(){
    ENetEvent event;
    if(server==NULL)
      return false;
    if(enet_host_service(server, &event, NETWORK_SEND_WAIT)>=0)
      return true;
    else
      return false;
  }

  void ConnectionManager::receiverThread(){
    // what about some error-handling here ?
    enet_initialize();
    atexit(enet_deinitialize);
    ENetEvent event;
    server = enet_host_create(&bindAddress, NETWORK_MAX_CONNECTIONS, 0, 0);
    if(server==NULL){
      // add some error handling here ==========================
      quit=true;
      return;
    }

    while(!quit){
      if(enet_host_service(server, &event, NETWORK_WAIT_TIMEOUT)<0){
        // we should never reach this point
        quit=true;
        // add some error handling here ========================
      }
      switch(event.type){
        // log handling ================
        case ENET_EVENT_TYPE_CONNECT:
        addClient(&event);
        break;
      case ENET_EVENT_TYPE_RECEIVE:
        std::cout << "received data" << std::endl;
        processData(&event);
        break;
      case ENET_EVENT_TYPE_DISCONNECT:
        // add some error/log handling here
        clientDisconnect(event.peer);
        break;
      case ENET_EVENT_TYPE_NONE:
        break;
      }
    }
    disconnectClients();
    // if we're finishied, destroy server
    enet_host_destroy(server);
  }
  
  void ConnectionManager::disconnectClients(){
    ENetEvent event;
    ClientInformation *temp = head_->next();
    while(temp!=0){
      enet_peer_disconnect(temp->getPeer(), 0);
      temp = temp->next();
    }
    temp = temp->next();
    while( temp!=0 && enet_host_service(server, &event, NETWORK_WAIT_TIMEOUT) > 0){
      switch (event.type)
      {
        case ENET_EVENT_TYPE_NONE:
        case ENET_EVENT_TYPE_CONNECT:
        case ENET_EVENT_TYPE_RECEIVE:
          enet_packet_destroy(event.packet);
          break;
        case ENET_EVENT_TYPE_DISCONNECT:
          delete head_->findClient(&(event.peer->address));
          temp = temp->next();
          break;
      }
    }
    return;
  }

  bool ConnectionManager::processData(ENetEvent *event){
    // just add packet to the buffer
    // this can be extended with some preprocessing
    return buffer.push(event);
  }

//   bool ConnectionManager::clientDisconnect(ENetPeer *peer){
//     return clientDisconnect(*peer);
//   }
  
  
  
  bool ConnectionManager::clientDisconnect(ENetPeer *peer){
    return head_->removeClient(peer);
    return true;
  }

  bool ConnectionManager::addClient(ENetEvent *event){
    ClientInformation *temp = head_->insertBack(new ClientInformation);
    temp->setID(temp->prev()->getID()+1);
    temp->setPeer(event->peer);
    std::cout << "added client id: " << temp->prev()->getID() << std::endl;
    return true;
  }
  
  int ConnectionManager::getClientID(ENetPeer peer){
    return getClientID(peer.address);
  }
  
  int ConnectionManager::getClientID(ENetAddress address){
    return head_->findClient(&address)->getID();
  }
  
  ENetPeer *ConnectionManager::getClientPeer(int clientID){
    return head_->findClient(clientID)->getPeer();
  }
  
  void ConnectionManager::syncClassid(int clientID){
    int i=0;
    std::string classname;
    bool abort=false;
    orxonox::Identifier *id;
    while(!abort){
      id = ID(i);
      if(id == NULL)
        abort=true;
      else{
        classname = id->getName();
        addPacket(packet_gen.clid( i, classname ),clientID);
      }
      ++i;
    }
    sendPackets();
  }
  
}
