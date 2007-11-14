//
// Dummy client to test ConnectionManager and PacketBuffer classes
//
// Author: Oliver Scheuss

#include <iostream>
#include <enet/enet.h>

using namespace std;

int main(){
  ENetHost * client;
  ENetAddress address;
  ENetEvent event;
  ENetPeer *peer;

  enet_initialize();
  atexit(enet_deinitialize);
  
  enet_address_set_host(&address, "127.0.0.1");
  address.port = 5555;

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
    puts("Connection to localhost:5555 succeeded.");
  }else{
    enet_peer_reset(peer);
    puts("Connection to localhost:5555 failed.");
  }

  for(int i=0; i<10; i++){
        // weihnachtsmann bringt packete
    ENetPacket *packet = enet_packet_create ("packet1234", strlen("packet1234") + 1, ENET_PACKET_FLAG_RELIABLE);
        // extend the packet and append the string foo to it
        // send packet to peer on channel id 0
    enet_peer_send(peer, 1, packet);
        // keep the timeout very small for low delay
    if(enet_host_service(client, &event, 1)==0){
      cout << "successfully sent: " << event.type << endl;
    }else{
      cout << "failed sending" << endl;
    }
  }

        // now disconnect
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
