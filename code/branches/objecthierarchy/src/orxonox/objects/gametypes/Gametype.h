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
 *      Fabian 'x3n' Landau
 *   Co-authors:
 *      ...
 *
 */

#ifndef _Gametype_H__
#define _Gametype_H__

#include "OrxonoxPrereqs.h"

#include <map>

#include "core/BaseObject.h"
#include "core/Identifier.h"
#include "network/ClientConnectionListener.h"
#include "objects/worldentities/ControllableEntity.h"

namespace orxonox
{
    class _OrxonoxExport Gametype : public BaseObject, public network::ClientConnectionListener
    {
        friend class PlayerInfo;

        public:
            Gametype();
            virtual ~Gametype() {}

            static Gametype* getCurrentGametype();
            static void listPlayers();

            inline const std::map<unsigned int, PlayerInfo*>& getClients() const
                { return this->clients_; }
            inline const std::set<PlayerInfo*>& getPlayers() const
                { return this->players_; }
            static PlayerInfo* getClient(unsigned int clientID);

        protected:
            virtual void clientConnected(unsigned int clientID);
            virtual void clientDisconnected(unsigned int clientID);

            virtual void playerJoined(PlayerInfo* player);
            virtual void playerLeft(PlayerInfo* player);

            virtual void playerChangedName(PlayerInfo* player);

        private:
            void addPlayer(PlayerInfo* player);
            void removePlayer(PlayerInfo* player);

            std::set<PlayerInfo*> players_;
            std::map<unsigned int, PlayerInfo*> clients_;
            SubclassIdentifier<ControllableEntity> defaultPawn_;
    };
}

#endif /* _Gametype_H__ */
