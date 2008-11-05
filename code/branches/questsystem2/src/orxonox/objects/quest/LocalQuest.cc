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
 *      Damian 'Mozork' Frick
 *   Co-authors:
 *      ...
 *
 */

/**
    @file LocalQuest.cc
    @brief
	Implementation of the LocalQuest class.
*/

#include "OrxonoxStableHeaders.h"
#include "LocalQuest.h"

#include "core/CoreIncludes.h"
#include "util/Exception.h"

#include "orxonox/objects/worldentities/ControllableEntity.h"
#include "QuestEffect.h"

namespace orxonox {

    CreateFactory(LocalQuest);

    /**
    @brief
        Constructor. Initializes the object.
    */
    LocalQuest::LocalQuest(BaseObject* creator) : Quest(creator)
    {
        this->initialize();
    }
    
    /**
    @brief
        Initializes the object.
    */
    void LocalQuest::initialize(void)
    {
        RegisterObject(LocalQuest);
    }

    /**
    @brief
        Destructor.
    */
    LocalQuest::~LocalQuest()
    {

    }

    /**
    @brief
        Method for creating a LocalQuest object through XML.
    */
    void LocalQuest::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(LocalQuest, XMLPort, xmlelement, mode);

        COUT(3) << "New LocalQuest {" << this->getId() << "} created." << std::endl;
    }

    /**
    @brief
        Fails the quest for a given player.
        Invokes all the failEffects on the player.
    @param player
        The player.
    @return
        Returns true if the quest could be failed, false if not.
    */
    bool LocalQuest::fail(ControllableEntity* player)
    {
        if(this->isFailable(player)) //!< Checks whether the quest can be failed.
        {
            this->setStatus(player, questStatus::failed);
            QuestEffect::invokeEffects(player, this->failEffects_); //!< Invoke the failEffects.
            return true;
        }
        
        COUT(2) << "A non-failable quest was trying to be failed." << std::endl;
        return false;
    }

    /**
    @brief
        Completes the quest for a given player.
	Invokes all the completeEffects on the player.
    @param player
        The player.
    @return
        Returns true if the quest could be completed, false if not.
    */
    bool LocalQuest::complete(ControllableEntity* player)
    {
        if(this->isCompletable(player)) //!< Checks whether the quest can be completed.
        {
            this->setStatus(player, questStatus::completed);
            QuestEffect::invokeEffects(player, this->completeEffects_); //!< Invoke the completeEffects.
            return true;
        }
        
        COUT(2) << "A non-completable quest was trying to be completed." << std::endl;
        return false;
    }

    /**
    @brief
        Checks whether the quest can be started.
    @param player
        The player for whom is to be checked.
    @return
        Returns true if the quest can be started, false if not.
    @throws
        Throws an exception if isInactive(ControllableEntity*) throws one.
    */
    bool LocalQuest::isStartable(const ControllableEntity* player) const
    {
        return this->isInactive(player);
    }

    /**
    @brief
        Checks whether the quest can be failed.
    @param player
        The player for whom is to be checked.
    @return
        Returns true if the quest can be failed, false if not.
    @throws
        Throws an exception if isActive(ControllableEntity*) throws one.
    */
    bool LocalQuest::isFailable(const ControllableEntity* player) const
    {
        return this->isActive(player);
    }

    /**
    @brief
        Checks whether the quest can be completed.
    @param player
        The player for whom is to be checked.
    @return
        Returns true if the quest can be completed, false if not.
    @throws
        Throws an exception if isInactive(ControllableEntity*) throws one.
    */
    bool LocalQuest::isCompletable(const ControllableEntity* player) const
    {
        return this->isActive(player);
    }

    /**
    @brief
        Returns the status of the quest for a specific player.
    @param player
        The player.
    @return
        Returns the status of the quest for the input player.
    @throws
        Throws an Exception if player is NULL.
    */
    questStatus::Enum LocalQuest::getStatus(const ControllableEntity* player) const
    {
        if(player == NULL) //!< No player has no defined status.
        {
            ThrowException(Argument, "The input ControllableEntity* is NULL.");
        }

        std::map<ControllableEntity*, questStatus::Enum>::const_iterator it = this->playerStatus_.find((ControllableEntity*)(void*)player); //Thx. to x3n for the (ControllableEntity*)(void*) 'hack'.
        if (it != this->playerStatus_.end()) //!< If there is a player in the map.
        {
            return it->second;
        }
        
        return questStatus::inactive; //!< If the player is not yet in the map, that means the status of the quest form him is 'inactive'.
    }

    /**
    @brief
        Sets the status for a specific player.
        But be careful wit this one, the status will just be set without checking for its validity. You have to know what you're doing. Really!
    @param player
        The player the status should be set for.
    @param status
        The status to be set.
    @return
        Returns false if player is NULL.
    */
    bool LocalQuest::setStatus(ControllableEntity* player, const questStatus::Enum & status)
    {
        if(player == NULL) //!< We can't set a status for no player.
        {
            return false;
        }
        
        this->playerStatus_[player] = status;
        return true;
    }

}
