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

#include "OrxonoxStableHeaders.h"
#include "PlayerManager.h"

#include "LevelManager.h"
#include "core/Core.h"
#include "core/CoreIncludes.h"
#include "objects/infos/Level.h"
#include "objects/infos/HumanPlayer.h"

namespace orxonox
{
    PlayerManager::PlayerManager()
    {
        RegisterRootObject(PlayerManager);

        this->getConnectedClients();
    }

    PlayerManager::~PlayerManager()
    {
    }

    PlayerManager& PlayerManager::getInstance()
    {
        static PlayerManager instance;
        return instance;
    }

    void PlayerManager::clientConnected(unsigned int clientID)
    {
        if (Core::isMaster())
        {
            COUT(3) << "client connected" << std::endl;

            // create new HumanPlayer instance
            HumanPlayer* player = new HumanPlayer(0);
            player->setClientID(clientID);

            // add to clients-map
            assert(!this->clients_[clientID]);
            this->clients_[clientID] = player;

            if (LevelManager::getInstancePtr() && LevelManager::getInstance().getActiveLevel())
                LevelManager::getInstance().getActiveLevel()->playerEntered(player);
        }
    }

    void PlayerManager::clientDisconnected(unsigned int clientID)
    {
        if (Core::isMaster())
        {
            COUT(3) << "client disconnected" << std::endl;

            // remove from clients-map
            PlayerInfo* player = this->clients_[clientID];
            this->clients_.erase(clientID);

            if (LevelManager::getInstancePtr() && LevelManager::getInstance().getActiveLevel())
                LevelManager::getInstance().getActiveLevel()->playerLeft(player);

            // delete PlayerInfo instance
            if (player)
                delete player;
        }
    }


    PlayerInfo* PlayerManager::getClient(unsigned int clientID) const
    {
        if (Core::isMaster())
        {
            std::map<unsigned int, PlayerInfo*>::const_iterator it = this->clients_.find(clientID);
            if (it != this->clients_.end())
                return it->second;
        }
        else
        {
            for (ObjectList<PlayerInfo>::iterator it = ObjectList<PlayerInfo>::begin(); it != ObjectList<PlayerInfo>::end(); ++it)
                if (it->getClientID() == clientID)
                    return (*it);
        }
        return 0;
    }
}
