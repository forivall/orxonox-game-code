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

#ifndef _Client_H__
#define _Client_H__

#include "NetworkPrereqs.h"

#include <string>

#include "Host.h"
#include "packet/Chat.h"
#include "ClientConnection.h"
#include "PacketManager.h"
#include "GameStateClient.h"
//#include "NetworkFrameListener.h"



namespace network
{
  /**
  network::Client *client;
  * The network/Client class
  * This class implements all necessary function for the network communication
  * It is the root class of the network module
  *
  */
  class _NetworkExport Client : PacketDecoder, public Host{
  public:
    Client();
    Client(std::string address, int port);
    Client(const char *address, int port);
    ~Client();
    
    bool establishConnection();
    bool closeConnection();
    bool queuePacket(ENetPacket *packet, int clientID);
    bool processChat(packet::Chat *message, unsigned int clientID);
    bool sendChat(packet::Chat *chat);
    
//    static void Chat( std::string message );
    
    int shipID(){return shipID_;}
    int playerID(){return clientID_;}

    void tick(float time);

  private:
    
    ClientConnection client_connection;
    PacketGenerator pck_gen;
    GameStateClient gamestate;
    bool isConnected;
    bool isSynched_;

    bool sendChat( std::string message );
    
    // implement data processing functions of PacketDecoder
    void processGamestate( GameStateCompressed *data, int clientID);
    void processClassid(classid *clid);
//     void processChat( chat *data, int clientId );
    bool processWelcome( welcome *w );
    int clientID_;     // this is the id the server gave to us
    int shipID_;
    bool gameStateFailure_;
  };


}

#endif /* _Client_H__ */
