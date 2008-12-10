

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
 *      Oliver Scheuss, (C) 2008
 *   Co-authors:
 *      ...
 *
 */


#include "Welcome.h"
#include "network/Host.h"
#include "network/synchronisable/Synchronisable.h"
#include "core/CoreIncludes.h"
#include <assert.h>

namespace orxonox {
namespace packet {

#define PACKET_FLAGS_CLASSID  ENET_PACKET_FLAG_RELIABLE
#define _PACKETID             0
#define _CLIENTID             _PACKETID + sizeof(ENUM::Type)
#define _SHIPID               _CLIENTID + sizeof(uint32_t)
#define _ENDIANTEST           _SHIPID + sizeof(uint32_t)
  
  Welcome::Welcome( unsigned int clientID, unsigned int shipID )
 : Packet()
{
  flags_ = flags_ | PACKET_FLAGS_CLASSID;
  assert(getSize());
  data_=new uint8_t[ getSize() ];
  assert(data_);
  *(packet::ENUM::Type *)(data_ + _PACKETID ) = packet::ENUM::Welcome;
  *(uint32_t *)(data_ + _CLIENTID ) = clientID;
  *(uint32_t *)(data_ + _SHIPID ) = shipID;
  *(uint32_t *)(data_ + _ENDIANTEST ) = 0xFEDC4321;
}

Welcome::Welcome( uint8_t* data, unsigned int clientID )
  : Packet(data, clientID)
{
}

Welcome::~Welcome()
{
}

uint8_t *Welcome::getData(){
  return data_;
}

unsigned int Welcome::getSize() const{
  return sizeof(packet::ENUM::Type) + 3*sizeof(uint32_t);
}

bool Welcome::process(){
  unsigned int shipID, clientID;
  clientID = *(uint32_t *)&data_[ _CLIENTID ];
  shipID = *(uint32_t *)&data_[ _SHIPID ];
  assert(*(uint32_t *)(data_ + _ENDIANTEST ) == 0xFEDC4321);
  Host::setClientID(clientID);
  Host::setShipID(shipID);
  COUT(3) << "Welcome set clientId: " << clientID << " shipID: " << shipID << std::endl;
  Synchronisable::setClient(true);
  delete this;
  return true;
}


} //namespace packet
}//namespace orxonox
