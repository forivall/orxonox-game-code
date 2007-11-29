#include <string>
#include <iostream>
#include <enet/enet.h>
#include <boost/thread/thread.hpp>
#include "PacketBuffer.h"
#include "PacketBuffer.cc"

using namespace network;

// workaround for usleep(int) under windows
#ifdef WIN32
#include "winbase.h"
#endif


void write(PacketBuffer *test){
  ENetEvent event;
  ENetPacket *packet;
  if(test->isEmpty())
    std::cout << "empty buffer" << std::endl;
  for(int i=0; i<10; i++){
    std::string temp = "packet ";
    packet = enet_packet_create("packet", strlen("packet ")+1,
ENET_PACKET_FLAG_RELIABLE);
    std::cout << i << ": pushing " << packet->data << std::endl;
    event.packet=packet;
    test->push(&event);
    if(i==5)
// under windows, use Sleep(milliseconds) instead of usleep(microseconds)
#ifdef WIN32
      Sleep(200);
#else
      usleep(200000);
#endif
  }
  test->setClosed(true);
  return;
}

void read(PacketBuffer *test){
  //test->print();
  // exit if the queue is closed and empty
  while(!test->isClosed() || !test->isEmpty()){
    // only pop if the queue isn't empty
    while(!test->isEmpty()){
      std::cout << "We popped the value " << test->pop()->data << std::endl;
    }
  }
  return;
}


int main(int argc, char **argv[]){
  PacketBuffer test = PacketBuffer();
  boost::thread thrd1(boost::bind(&write, &test));
  boost::thread thrd2(boost::bind(&read, &test));

  thrd1.join();
  thrd2.join();

  return 0;
}

