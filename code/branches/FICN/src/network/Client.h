//
// C++ Interface: Client
//
// Description:
//
//
// Author:  Oliver Scheuss, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef NETWORK_CLIENT_H
#define NETWORK_CLIENT_H

#include <string>

#include "ClientConnection.h"
#include "PacketManager.h"
#include "GameStateClient.h"
#include "orxonox/core/IdentifierIncludes.h"


namespace network{

/**
 * The network/Client class
 * This class implements all necessary function for the network communication
 * It is the root class of the network module
 *
 */
class Client : PacketDecoder{
  public:
  Client();
  Client(std::string address, int port);
  Client(const char *address, int port);

  bool establishConnection();
  bool closeConnection();

  bool sendMouse(double x, double y);
  bool sendKeyboard(char key_code);

  bool addMouse(double x, double y);
  bool addKeyboard(char key_code);

  bool sendPackets();
  
  void update();

  private:
  ClientConnection client_connection;
  PacketGenerator pck_gen;
  GameStateClient gamestate;
  bool isConnected;
  
  // implement data processing functions of PacketDecoder
  void processGamestate( GameStateCompressed *data);
  void processClassid(classid *clid);

};

}

#endif
