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
    @brief Definition of the FailQuest class.
*/

#ifndef _FailQuest_H__
#define _FailQuest_H__

#include "OrxonoxPrereqs.h"

#include "core/XMLPort.h"
#include "ChangeQuestStatus.h"

namespace orxonox
{
    /**
    @brief
        Fails a quest (with a specified id) for the player invoking the QuestEffect.
        
        Creating a FailQuest through XML goes as follows:
        
        <FailQuest questId="id" />  //Where id is a GUID, see http://en.wikipedia.org/wiki/Globally_Unique_Identifier#Basic_structure for more information, and identifies the Quest that should be failed.
    @author
        Damian 'Mozork' Frick
    */
    class _OrxonoxExport FailQuest : public ChangeQuestStatus
    {
        public:
            FailQuest(BaseObject* creator);
            virtual ~FailQuest();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode); //!< Method for creating a FailQuest object through XML.

            virtual bool invoke(PlayerInfo* player); //!< Invokes the QuestEffect.

    };

}

#endif /* _FailQuest_H__ */
