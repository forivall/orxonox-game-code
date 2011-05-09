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

//o
// Dummy client to test ConnectionManager and PacketBuffer classes
//
// Author: Oliver Scheuss

#include <iostream>
#include <string>
#include <enet/enet.h>

#include "util/Sleep.h"
#include "PacketManager.h"
#include "PacketTypes.h"

using namespace std;

int main(){
  ENetHost * client;
  ENetAddress address;
  ENetEvent event;
  ENetPeer *peer;
  network::PacketGenerator pck;

  enet_initialize();
  atexit(enet_deinitialize);

  cout << "Enter address of the server xxx.xxx.xxx.xxx (enter for localhost)" << endl;
  string str;
  getline(cin, str);
  cout << "You entered: " << str << endl;
  if(str.compare("")==0)
    str="127.0.0.1";

  enet_address_set_host(&address, str.c_str());
  address.port = 55556;

  // create client object
  client = enet_host_create(NULL, 2, 0, 0);

  if(client==NULL){
    fprintf(stderr, "An error occured");
    exit(EXIT_FAILURE);
  }
  // connect peer
  peer = enet_host_connect(client, &address, 2);
  if(peer==NULL){
    fprintf(stderr, "Peer establishing error");
    exit(EXIT_FAILURE);
  }
  // wait 5 seconds for the connection attempt to succeed
  if(enet_host_service(client, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT){
    cout << "Connection to " << str << " succeeded." << endl;
    //puts("Connection to localhost:5555 succeeded.");
  }else{
    enet_peer_reset(peer);
    cout << "Connection to " << str << " failed." << endl;
    //puts("Connection to localhost:5555 failed.");
    exit(EXIT_FAILURE);
  }

  for(int i=0; i<10; i++){
    // weihnachtsmann bringt packete
    //ENetPacket *packet = enet_packet_create ("packet1234", strlen("packet1234") + 1, ENET_PACKET_FLAG_RELIABLE);
    // extend the packet and append the string foo to it
    // send packet to peer on channel id 0
    enet_peer_send(peer, 1, pck.chatMessage("test2"));
    // keep the timeout very small for low delay
    if(enet_host_service(client, &event, 1)==0){
      cout << "successfully sent: " << event.type << endl;
    }else{
      cout << "failed sending" << endl;
    }
    usleep(1000000);
  }

  // now disconnect
  //   enet_peer_disconnect (peer);
  enet_peer_disconnect (peer, 0);
  // 3 seconds timeout
  while(enet_host_service(client, &event, 3000) > 0){
    switch (event.type)
    {
    case ENET_EVENT_TYPE_RECEIVE:
      enet_packet_destroy(event.packet);
      break;
    case ENET_EVENT_TYPE_DISCONNECT:
      puts("Disconnection succeeded.");
      return 0;
    }
  }
  // if disconnect failed
  enet_peer_reset(peer);



  return 0;
}