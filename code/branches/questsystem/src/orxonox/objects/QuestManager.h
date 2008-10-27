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

#ifndef _QuestManager_H__
#define _QuestManager_H__

#include <map>
#include <string>

#include "core/BaseObject.h"
#include "Quest.h"
#include "QuestHint.h"

namespace orxonox {

    /**
    @brief
        Manages quests, by making them globally accessable.
    @author
        Damian 'Mozork' Frick
    */
    class QuestManager : public BaseObject
    {
    
	public:
	    QuestManager();
	    ~QuestManager();
	    
	    static bool registerQuest(Quest* quest); //!< Registers a quest in the QuestManager.
	    static bool registerHint(QuestHint* quest); //!< Registers a QuestHint in the QuestManager.
	    
	    static Quest* findQuest(const std::string & questId); //!< Returns the quest with the input id.
	    static QuestHint* findHint(const std::string & hintId); //!< Returns the QuestHint with the input id.
	    	    
	private:
	    static std::map<std::string, Quest*> questMap_;
	    static std::map<std::string, QuestHint*> hintMap_;
    
    };

}

#endif /* _QuestManager_H__ */
