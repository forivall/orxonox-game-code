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
    @file
    @brief Definition of the NotificationManager class.
*/

#ifndef _NotificationManager_H__
#define _NotificationManager_H__

#include "notifications/NotificationsPrereqs.h"

#include <ctime>
#include <map>
#include <string>
#include <vector>

#include "util/Singleton.h"
#include "core/OrxonoxClass.h"

namespace orxonox // tolua_export
{ // tolua_export

    /**
    @brief
        The Singleton NotificationManager functions as a gateway between Notifications and NotificationListeners.
        It receives, organizes Notifications and the redistributes them to the specific NotificationListeners.
    @author
        Damian 'Mozork' Frick
    */
    class _NotificationsExport NotificationManager  // tolua_export
        : public Singleton<NotificationManager>, public OrxonoxClass
    { // tolua_export
            friend class Singleton<NotificationManager>;
        public:
            NotificationManager();
            virtual ~NotificationManager();

            virtual void preDestroy(void);

            static NotificationManager& getInstance() { return Singleton<NotificationManager>::getInstance(); } // tolua_export

            static const std::string ALL; //!< Static string to indicate a sender that sends to all NotificationListeners.
            static const std::string NONE; //!< Static string to indicare a sender that sends to no specific NotificationListener.

            bool registerNotification(Notification* notification); //!< Registers a Notification within the NotificationManager.
            void unregisterNotification(Notification* notification, NotificationListener* listener); //!< Unregisters a Notification within the NotificationManager.
            bool registerListener(NotificationListener* listener); //!< Registers a NotificationListener within the NotificationManager.
            void unregisterListener(NotificationListener* listener); //!< Unregisters a NotificationListener withing the NotificationManager.

            bool getNotifications(NotificationListener* listener, std::multimap<std::time_t,Notification*>* map, const std::time_t & timeFrameStart, const std::time_t & timeFrameEnd); //!< Returns the Notifications for a specific NotificationListener in a specified timeframe.

            /**
            @brief Fetches the Notifications for a specific NotificationListener starting at a specified timespan before now.
            @param listener The NotificationListener the Notifications are fetched for.
            @param map A multimap, in which the notifications are stored.
            @param timeDelay The timespan.
            @return Returns true if successful.
            */
            bool getNotifications(NotificationListener* listener, std::multimap<std::time_t,Notification*>* map, int timeDelay)
                { return this->getNotifications(listener, map, std::time(0)-timeDelay, std::time(0)); }

            void enterEditMode(void);

            void createQueue(const std::string& name, const std::string& targets, unsigned int size, unsigned int displayTime); // tolua_export

        private:
            static NotificationManager* singletonPtr_s;

            std::vector<NotificationQueue*> queues_; //!< The list of NotificationQueues created by the NotificationManager.

            int highestIndex_; //!< This variable holds the highest index (resp. key) in notificationLists_s, to secure that no key appears twice.

            std::multimap<std::time_t,Notification*> allNotificationsList_; //!< Container where all notifications are stored.
            std::map<NotificationListener*,int> listenerList_; //!< Container where all NotificationListeners are stored with a number as identifier.
            std::map<int,std::multimap<std::time_t,Notification*>*> notificationLists_; //!< Container where all Notifications, for each identifier (associated with a NotificationListener), are stored.
            std::map<Notification*, unsigned int> listenerCounter_; //!< A container to store the number of NotificationListeners a Notification is registered with.

            bool removeNotification(Notification* notification, std::multimap<std::time_t, Notification*>& map); //!< Helper method that removes an input notification form an input map.

    }; // tolua_export

} // tolua_export

#endif /* _NotificationManager_H__ */
