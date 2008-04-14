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

#include <enet/enet.h>

#include "NetworkPrereqs.h"

#define GAMESTATEID_INITIAL -1

namespace network
{
  /**
  * This class implements a list for client informations
  * @author Oliver Scheuss
  */
  class ClientInformation{
  public:
    ClientInformation();
    ClientInformation(bool head);
    //   ClientInformation(ClientInformation *prev, ClientInformation *next);
    //   ClientInformation(ClientInformation *prev);
    ~ClientInformation();
    ClientInformation *next();
    ClientInformation *prev();
    bool setNext(ClientInformation *next);
    bool setPrev(ClientInformation *prev);
    ClientInformation *insertAfter(ClientInformation *ins);
    ClientInformation *insertBefore(ClientInformation *ins);
    ClientInformation *insertBack(ClientInformation *ins);
    void setID(int clientID);
    void setPeer(ENetPeer *peer);
    void setGamestateID(int id);
    int getID();
    ENetPeer *getPeer();
    int getGamestateID();
    bool removeClient(int clientID);
    bool removeClient(ENetPeer *peer);
    ClientInformation *findClient(int clientID, bool look_backwards=false);
    ClientInformation *findClient(ENetAddress *address, bool look_backwards=false);

    void setSynched(bool s);
    bool getSynched();

    bool head;

  private:
    ClientInformation *preve;
    ClientInformation *nexte;
    //actual information:
    ENetPeer *peer_;
    int clientID_;
    int gamestateID_;
    bool synched_;
  };

}

#endif /* _ClientInformation_H__ */
