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
 *      ...
 *   Co-authors:
 *      ...
 *
 */

//
// C++ Interface: ClientInformation
//
// Description:
//
//
// Author:  <>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef _ClientInformation_H__
#define _ClientInformation_H__

#include "NetworkPrereqs.h"

#include <enet/enet.h>
#include <boost/thread/recursive_mutex.hpp>

#define GAMESTATEID_INITIAL -1
#define CLIENTID_UNKNOWN -2

// WATCH OUT: THE CLIENTINFORMATION LIST IS NOT THREADSAFE ANYMORE

namespace network
{
  /**
  * This class implements a list for client informations
  * @author Oliver Scheuss
  */
  class ClientInformation{
  public:
    ClientInformation();
    //   ClientInformation(ClientInformation *prev, ClientInformation *next);
    //   ClientInformation(ClientInformation *prev);
    ~ClientInformation();
    ClientInformation *next();
    ClientInformation *prev();
    static ClientInformation *insertBack(ClientInformation *ins);
    
    // set functions
    void setID(int clientID);
    bool setPeer(ENetPeer *peer);
    bool setGamestateID(int id);
    bool setPartialGamestateID(int id);
    inline void setShipID(unsigned int id){ShipID_=id;}
    
    // get functions
    inline unsigned int getShipID(){return ShipID_;}
    int getID();
    int getGamestateID();
    int getPartialGamestateID();
    ENetPeer *getPeer();
    
    int getFailures();
    void addFailure();
    void resetFailures();
    enet_uint32 getRTT();
    enet_uint32 getPacketLoss();
    
    static bool removeClient(int clientID);
    static bool removeClient(ENetPeer *peer);
    static ClientInformation *findClient(int clientID, bool look_backwards=false);
    static ClientInformation *findClient(ENetAddress *address, bool look_backwards=false);
    static ClientInformation *getBegin(){return head_;}

    bool setSynched(bool s);
    bool getSynched();


  private:
    static ClientInformation *head_;
    
    bool setNext(ClientInformation *next);
    bool setPrev(ClientInformation *prev);
    ClientInformation *insertAfter(ClientInformation *ins);
    ClientInformation *insertBefore(ClientInformation *ins);
    
    ClientInformation *preve;
    ClientInformation *nexte;
    //actual information:
    ENetPeer *peer_;
    int clientID_;
    int gamestateID_;
    int partialGamestateID_;
    unsigned int ShipID_;   // this is the unique objectID
    bool synched_;
    unsigned short failures_;
    
  };

}

#endif /* _ClientInformation_H__ */