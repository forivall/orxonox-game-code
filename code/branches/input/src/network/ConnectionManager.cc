/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
 *                    > www.orxonox.net <
 *
 *
 *   License notice:
 *
 *   This program is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU General Public License
 *   as published by the Free Software Foundation; either version 2
 *   of the License, or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 *   Author:
 *      Oliver Scheuss, (C) 2007
 *   Co-authors:
 *      ...
 *
 */

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

#include <iostream>
// boost.thread library for multithreading support
#include <boost/bind.hpp>

#include "core/CoreIncludes.h"
#include "core/BaseObject.h"
#include "objects/SpaceShip.h"
#include "util/Math.h"
#include "util/Sleep.h"
#include "ClientInformation.h"
#include "ConnectionManager.h"
#include "Synchronisable.h"

namespace std
{
  bool operator< (ENetAddress a, ENetAddress b) {
    if(a.host <= b.host)
      return true;
    else
      return false;
  }
}

namespace network
{
  //boost::thread_group network_threads;
  
  ConnectionManager::ConnectionManager():receiverThread_(0){}
  boost::recursive_mutex ConnectionManager::enet_mutex_;
  
  ConnectionManager::ConnectionManager(ClientInformation *head) : receiverThread_(0) {
    quit=false;
    bindAddress.host = ENET_HOST_ANY;
    bindAddress.port = NETWORK_PORT;
    head_ = head;
  }
  
  ConnectionManager::ConnectionManager(ClientInformation *head, int port){
    quit=false;
    bindAddress.host = ENET_HOST_ANY;
    bindAddress.port = port;
    head_ = head;
  }

  ConnectionManager::ConnectionManager(int port, std::string address, ClientInformation *head) :receiverThread_(0) {
    quit=false;
    enet_address_set_host (& bindAddress, address.c_str());
    bindAddress.port = NETWORK_PORT;
    head_ = head;
  }

  ConnectionManager::ConnectionManager(int port, const char *address, ClientInformation *head) : receiverThread_(0) {
    quit=false;
    enet_address_set_host (& bindAddress, address);
    bindAddress.port = NETWORK_PORT;
    head_ = head;
  }

  /*ENetPacket *ConnectionManager::getPacket(ENetAddress &address) {
    if(!buffer.isEmpty())
      return buffer.pop(address);
    else
      return NULL;
  }*/
/**
This function only pops the first element in PacketBuffer (first in first out)
used by processQueue in Server.cc
*/
  /*ENetPacket *ConnectionManager::getPacket(int &clientID) {
    ENetAddress address;
    ENetPacket *packet=getPacket(address);
    ClientInformation *temp =head_->findClient(&address);
    if(!temp)
      return NULL;
    clientID=temp->getID();
    return packet;
  }*/
  
  ENetEvent *ConnectionManager::getEvent(){
    if(!buffer.isEmpty())
      return buffer.pop();
    else
      return NULL;
  }

  bool ConnectionManager::queueEmpty() {
    return buffer.isEmpty();
  }

  void ConnectionManager::createListener() {
    receiverThread_ = new boost::thread(boost::bind(&ConnectionManager::receiverThread, this));
    //network_threads.create_thread(boost::bind(boost::mem_fn(&ConnectionManager::receiverThread), this));
         //boost::thread thr(boost::bind(boost::mem_fn(&ConnectionManager::receiverThread), this));
    return;
  }

  bool ConnectionManager::quitListener() {
    quit=true;
    //network_threads.join_all();
    receiverThread_->join();
    return true;
  }

  bool ConnectionManager::addPacket(ENetPacket *packet, ENetPeer *peer) {
    ClientInformation *temp = head_->findClient(&(peer->address));
    if(!temp)
      return false;
    boost::recursive_mutex::scoped_lock lock(enet_mutex_);
    if(enet_peer_send(peer, (enet_uint8)temp->getID() , packet)!=0)
      return false;
    return true;
  }

  bool ConnectionManager::addPacket(ENetPacket *packet, int clientID) {
    ClientInformation *temp = head_->findClient(clientID);
    if(!temp){
      COUT(3) << "C.Man: addPacket findClient failed" << std::endl;
      return false;
    }
    boost::recursive_mutex::scoped_lock lock(enet_mutex_);
    if(enet_peer_send(temp->getPeer(), 0, packet)!=0){
      COUT(3) << "C.Man: addPacket enet_peer_send failed" << std::endl;
      return false;
    }
    return true;
  }

  bool ConnectionManager::addPacketAll(ENetPacket *packet) {
    boost::recursive_mutex::scoped_lock lock(enet_mutex_);
    for(ClientInformation *i=head_->next(); i!=0; i=i->next()){
      if(enet_peer_send(i->getPeer(), (enet_uint8)i->getID(), packet)!=0)
        return false;
    }
    return true;
  }

  // we actually dont need that function, because host_service does that for us
  bool ConnectionManager::sendPackets() {
    if(server==NULL)
      return false;
    boost::recursive_mutex::scoped_lock lock(enet_mutex_);
    enet_host_flush(server);
    lock.unlock();
    return true;
  }

  void ConnectionManager::receiverThread() {
    // what about some error-handling here ?
    ENetEvent *event;
    atexit(enet_deinitialize);
    { //scope of the mutex
      boost::recursive_mutex::scoped_lock lock(enet_mutex_);
      enet_initialize();
      server = enet_host_create(&bindAddress, NETWORK_MAX_CONNECTIONS, 0, 0);
      lock.unlock();
    }
    if(server==NULL){
      // add some error handling here ==========================
      quit=true;
      return;
    }

    event = new ENetEvent;
    while(!quit){
      { //mutex scope
        boost::recursive_mutex::scoped_lock lock(enet_mutex_);
        if(enet_host_service(server, event, NETWORK_WAIT_TIMEOUT)<0){
          // we should never reach this point
          quit=true;
          continue;
          // add some error handling here ========================
        }
        lock.unlock();
      }
      switch(event->type){
        // log handling ================
        case ENET_EVENT_TYPE_CONNECT:
          COUT(3) << "adding event_type_connect to queue" << std::endl;
        case ENET_EVENT_TYPE_DISCONNECT:
          //addClient(event);
          //this is a workaround to ensure thread safety
          //COUT(5) << "Con.Man: connection event has occured" << std::endl;
          //break;
        case ENET_EVENT_TYPE_RECEIVE:
          //std::cout << "received data" << std::endl;
          COUT(5) << "Con.Man: receive event has occured" << std::endl;
          // only add, if client has connected yet and not been disconnected
          //if(head_->findClient(&event->peer->address))
            processData(event);
            event = new ENetEvent;
//           else
//             COUT(3) << "received a packet from a client we don't know" << std::endl;
          break;
        //case ENET_EVENT_TYPE_DISCONNECT:
          //clientDisconnect(event->peer);
          //break;
        case ENET_EVENT_TYPE_NONE:
          //receiverThread_->yield();
          usleep(1000);
          break;
      }
//       usleep(100);
      //receiverThread_->yield(); //TODO: find apropriate
    }
    disconnectClients();
    // if we're finishied, destroy server
    {
      boost::recursive_mutex::scoped_lock lock(enet_mutex_);
      enet_host_destroy(server);
      lock.unlock();
    }
  }
  
  //### added some bugfixes here, but we cannot test them because
  //### the server crashes everytime because of some gamestates
  //### (trying to resolve that now)
  void ConnectionManager::disconnectClients() {
    ENetEvent event;
    ClientInformation *temp = head_->next();
    while(temp!=0){
      {
        boost::recursive_mutex::scoped_lock lock(enet_mutex_);
        enet_peer_disconnect(temp->getPeer(), 0);
        lock.unlock();
      }
      temp = temp->next();
    }
    //bugfix: might be the reason why server crashes when clients disconnects
    temp = head_->next();
    boost::recursive_mutex::scoped_lock lock(enet_mutex_);
    while( temp!=0 && enet_host_service(server, &event, NETWORK_WAIT_TIMEOUT) >= 0){
      switch (event.type)
      {
      case ENET_EVENT_TYPE_NONE: break;
      case ENET_EVENT_TYPE_CONNECT: break;
      case ENET_EVENT_TYPE_RECEIVE:
        enet_packet_destroy(event.packet);
        break;
      case ENET_EVENT_TYPE_DISCONNECT:
        COUT(4) << "disconnecting all clients" << std::endl;
        if(head_->findClient(&(event.peer->address)))
          delete head_->findClient(&(event.peer->address));
        //maybe needs bugfix: might also be a reason for the server to crash
        temp = temp->next();
        break;
      }
    }
    return;
  }

  bool ConnectionManager::processData(ENetEvent *event) {
    // just add packet to the buffer
    // this can be extended with some preprocessing
    return buffer.push(event);
  }



  int ConnectionManager::getClientID(ENetPeer peer) {
    return getClientID(peer.address);
  }

  int ConnectionManager::getClientID(ENetAddress address) {
    return head_->findClient(&address)->getID();
  }

  ENetPeer *ConnectionManager::getClientPeer(int clientID) {
    return head_->findClient(clientID)->getPeer();
  }

  void ConnectionManager::syncClassid(int clientID) {
    unsigned int network_id=0, failures=0;
    std::string classname;
    orxonox::Identifier *id;
    std::map<std::string, orxonox::Identifier*>::const_iterator it = orxonox::Factory::getFactoryBegin();
    while(it != orxonox::Factory::getFactoryEnd()){
      id = (*it).second;
      if(id == NULL)
        continue;
      classname = id->getName();
      network_id = id->getNetworkID();
      if(network_id==0)
        COUT(3) << "we got a null class id: " << id->getName() << std::endl;
      COUT(4) << "Con.Man:syncClassid:\tnetwork_id: " << network_id << ", classname: " << classname << std::endl;

      while(!addPacket(packet_gen.clid( (int)network_id, classname ), clientID) && failures < 10){
        failures++;
      }
      ++it;
    }
    //sendPackets();
    COUT(4) << "syncClassid:\tall synchClassID packets have been sent" << std::endl;
  }

  
  
  bool ConnectionManager::removeShip(ClientInformation *client){
    int id=client->getShipID();
    orxonox::Iterator<orxonox::SpaceShip> it;
    for(it = orxonox::ObjectList<orxonox::SpaceShip>::start(); it; ++it){
      if(it->objectID!=id)
        continue;
      delete *it;
    }
    return true;
  }
  
  bool ConnectionManager::sendWelcome(int clientID, int shipID, bool allowed){
    if(addPacket(packet_gen.generateWelcome(clientID, shipID, allowed),clientID)){
      //sendPackets();
      return true;
    }else
      return false;
  }
  
  void ConnectionManager::disconnectClient(ClientInformation *client){
    {
      boost::recursive_mutex::scoped_lock lock(enet_mutex_);
      enet_peer_disconnect(client->getPeer(), 0);
      lock.unlock();
    }
    removeShip(client);
  }
  
  bool ConnectionManager::addFakeConnectRequest(ENetEvent *ev){
    ENetEvent event;
    event.peer=ev->peer;
    event.packet = packet_gen.generateConnectRequest();
    return buffer.push(&event);
  }
  
  

}
