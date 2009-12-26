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
 *      Daniel 'Huty' Haggenmueller
 *   Co-authors:
 *      ...
 *
 */

/**
    @file
    @brief Definition of PickupSpawner.
*/

#ifndef _PickupSpawner_H__
#define _PickupSpawner_H__

#include "pickup/PickupPrereqs.h"

#include <string>
#include "tools/Timer.h"
#include "tools/interfaces/Tickable.h"
#include "worldentities/StaticEntity.h"
#include "interfaces/Pickupable.h"

namespace orxonox
{
    /**
        @brief PickupSpawner.
        @author Daniel 'Huty' Haggenmueller
    */
    class _OrxonoxExport PickupSpawner : public StaticEntity, public Tickable
    {
        public:
            //TODO: Add limit of items spawned here.
            PickupSpawner(BaseObject* creator);
            PickupSpawner(BaseObject* creator, Pickupable* pickup, float triggerDistance, float respawnTime, int maxSpawnedItems);
            virtual ~PickupSpawner();

            //virtual void changedActivity();                                 //!< Invoked when activity has changed (set visibilty).
            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);  //!< Method for creating a PickupSpawner through XML.
            virtual void tick(float dt);

            /**
                @brief Get the distance in which to trigger.
                @return Returns the distance in which this gets triggered.
            */
            inline float getTriggerDistance() const
                { return this->triggerDistance_; }
            /**
                @brief Set the distance in which to trigger.
                @param value The new distance in which to trigger.
            */
            inline void setTriggerDistance(float value)
                { this->triggerDistance_ = value; }

            /**
                @brief Get the time to respawn.
                @returns Returns the time after which this gets re-actived.
            */
            inline float getRespawnTime() const
                { return this->respawnTime_; }
            /**
                @brief Set the time to respawn.
                @param time New time after which this gets re-actived.
            */
            inline void setRespawnTime(float time)
                { this->respawnTime_ = time; }

            /**
                @brief Get the maximum number of items that will be spawned by this PickupSpawner.
                @return Returns the maximum number of items spawned by this PickupSpawner.
            */
            inline int getMaxSpawnedItems(void)
                { return this->maxSpawnedItems_; }
            void setMaxSpawnedItems(int items);

        protected:
            virtual Pickupable* getPickup(void);
            
            void setPickupable(Pickupable* pickup);
            const Pickupable* getPickupable(void);
            
            void decrementSpawnsRemaining(void);
            
            Pickupable* pickup_; //!< The pickup to be spawned.

        private:
            void initialize(void);
            
            void trigger(Pawn* pawn);                                       //!< Method called when a Pawn is close enough.
            void respawnTimerCallback();                                    //!< Method called when the timer runs out.

            int maxSpawnedItems_;                   //!< Maximum number of items spawned by this PickupSpawner.
            int spawnsRemaining_;                   //!< Number of items that can be spawned by this PickupSpawner until it selfdestructs.

            float triggerDistance_;                 //!< Distance in which this gets triggered.

            /* Pickup animation */
            float tickSum_;                         //!< Adds up tick to use in sine movement
            static const float bounceSpeed_s;       //!< Speed of pickup to bounce up and down
            static const float bounceDistance_s;    //!< Distance the pickup bounces up and down
            static const float rotationSpeed_s;     //!< Rotation speed of pickup

            float respawnTime_;                     //!< Time after which this gets re-actived.
            Timer respawnTimer_;                    //!< Timer used for re-activating.

            static const int INF = -1;             //!< Constant for infinity.
    };
}

#endif /* _PickupSpawner_H__ */
