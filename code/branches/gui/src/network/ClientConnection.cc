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
// C++ Interface: ClientConnection
//
// Description: The Class ClientConnection manages the servers conenctions to the clients.
// each connection is provided by a new process. communication between master process and
// connection processes is provided by ...
//
//
// Author:  Oliver Scheuss
//

#include "ClientConnection.h"

#include <iostream>
// boost.thread library for multithreading support
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>

#include "util/Sleep.h"
#include "core/Debug.h"

namespace network
{
  //static boost::thread_group network_threads;

  boost::recursive_mutex ClientConnection::enet_mutex_;

  ClientConnection::ClientConnection(int port, std::string address) {
    quit=false;
    server=NULL;
    enet_address_set_host(&serverAddress, address.c_str());
    serverAddress.port = port;
    established=false;
  }

  ClientConnection::ClientConnection(int port, const char *address) {
    quit=false;
    server=NULL;
    enet_address_set_host(&serverAddress, address);
    serverAddress.port = port;
    established=false;
  }

  bool ClientConnection::waitEstablished(int milisec) {
    for(int i=0; i<=milisec && !established; i++)
      usleep(1000);

    return established;
  }


  /*ENetPacket *ClientConnection::getPacket(ENetAddress &address) {
    if(!buffer.isEmpty()) {
      //std::cout << "###BUFFER IS NOT EMPTY###" << std::endl;
      return buffer.pop(address);
    }
    else{
      return NULL;
    }
  }

  ENetPacket *ClientConnection::getPacket() {
    ENetAddress address; //sems that address is not needed
    return getPacket(address);
  }*/
  
  ENetEvent *ClientConnection::getEvent(){
    if(!buffer.isEmpty())
      return buffer.pop();
    else
      return NULL;
  }

  bool ClientConnection::queueEmpty() {
    return buffer.isEmpty();
  }

  bool ClientConnection::createConnection() {
    receiverThread_ = new boost::thread(boost::bind(&ClientConnection::receiverThread, this));
    //network_threads.create_thread(boost::bind(boost::mem_fn(&ClientConnection::receiverThread), this));
    // wait 10 seconds for the connection to be established
    return waitEstablished(NETWORK_CLIENT_CONNECT_TIMEOUT);
  }

  bool ClientConnection::closeConnection() {
    quit=true;
    //network_threads.join_all();
    receiverThread_->join();
    established=false;
    return true;
  }


  bool ClientConnection::addPacket(ENetPacket *packet) {
    if(server==NULL)
      return false;
    if(packet==NULL){
      COUT(3) << "Cl.con: addpacket: invalid packet" << std::endl;
      return false;
    }
    boost::recursive_mutex::scoped_lock lock(enet_mutex_);
    if(enet_peer_send(server, 0, packet)<0)
      return false;
    else
      return true;
  }

  bool ClientConnection::sendPackets() {
    if(server==NULL)
      return false;
    boost::recursive_mutex::scoped_lock lock(enet_mutex_);
    enet_host_flush(client);
    lock.unlock();
    return true;
  }

  void ClientConnection::receiverThread() {
    // what about some error-handling here ?
    atexit(enet_deinitialize);
    ENetEvent *event;
    {
      boost::recursive_mutex::scoped_lock lock(enet_mutex_);
      enet_initialize();
      client = enet_host_create(NULL, NETWORK_CLIENT_MAX_CONNECTIONS, 0, 0);
      lock.unlock();
    }
    if(client==NULL) {
      COUT(2) << "ClientConnection: could not create client host" << std::endl;
      // add some error handling here ==========================
      quit=true;
    }
    //connect to the server
    if(!establishConnection()){
      COUT(2) << "clientConn: receiver thread: could not establishConnection" << std::endl;
      quit=true;
      return;
    }
    event = new ENetEvent;
    //main loop
    while(!quit){
      //std::cout << "connection loop" << std::endl;
      {
        boost::recursive_mutex::scoped_lock lock(enet_mutex_);
        if(enet_host_service(client, event, NETWORK_CLIENT_WAIT_TIME)<0){
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
        break;
      case ENET_EVENT_TYPE_RECEIVE:
        COUT(5) << "Cl.Con: receiver-Thread while loop: got new packet" << std::endl;
        if ( !processData(event) ) COUT(2) << "Current packet was not pushed to packetBuffer -> ev ongoing SegFault" << std::endl;
        COUT(5) << "Cl.Con: processed Data in receiver-thread while loop" << std::endl;
        event = new ENetEvent;
        break;
      case ENET_EVENT_TYPE_DISCONNECT:
        quit=true;
        // server closed the connection
        return;
        break;
      case ENET_EVENT_TYPE_NONE:
        //receiverThread_->yield();
        usleep(1000);
        break;
      }
    }
    // now disconnect

    if(!disconnectConnection())
      // if disconnecting failed destroy conn.
      boost::recursive_mutex::scoped_lock lock(enet_mutex_);
      enet_peer_reset(server);
    return;
  }

  bool ClientConnection::disconnectConnection() {
    ENetEvent event;
    boost::recursive_mutex::scoped_lock lock(enet_mutex_);
    enet_peer_disconnect(server, 0);
    while(enet_host_service(client, &event, NETWORK_CLIENT_WAIT_TIME) > 0){
      switch (event.type)
      {
      case ENET_EVENT_TYPE_NONE:
      case ENET_EVENT_TYPE_CONNECT:
      case ENET_EVENT_TYPE_RECEIVE:
        enet_packet_destroy(event.packet);
        break;
      case ENET_EVENT_TYPE_DISCONNECT:
        return true;
      }
    }
    enet_peer_reset(server);
    return false;
  }

  bool ClientConnection::establishConnection() {
    ENetEvent event;
    // connect to peer (server is type ENetPeer*)
    boost::recursive_mutex::scoped_lock lock(enet_mutex_);
    server = enet_host_connect(client, &serverAddress, NETWORK_CLIENT_CHANNELS);
    if(server==NULL) {
      COUT(2) << "ClientConnection: server == NULL" << std::endl;
      // error handling
      return false;
    }
    // handshake
    while(enet_host_service(client, &event, NETWORK_CLIENT_WAIT_TIME)>=0 && !quit){
      if( event.type == ENET_EVENT_TYPE_CONNECT ){
        established=true;
        return true;
      }
    }
    COUT(2) << "ClientConnection: enet_host_service < 0 or event.type != ENET_EVENT_TYPE_CONNECT # EVENT:" << event.type << std::endl;
    return false;
  }

  bool ClientConnection::processData(ENetEvent *event) {
    COUT(5) << "Cl.Con: got packet, pushing to queue" << std::endl;
    // just add packet to the buffer
    // this can be extended with some preprocessing
    return buffer.push(event);
  }

}
