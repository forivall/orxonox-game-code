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
 *      Val Mikos
 *   Co-authors:
 *      ...
 *
 */

#ifndef _TeamBaseMatchBase_H__
#define _TeamBaseMatchBase_H__

#include "Pawn.h"

namespace orxonox
{


    namespace BaseState
    {
        enum Enum
        {
            uncontrolled,
            controlTeam1,
            controlTeam2,
        };
    }


    class _OrxonoxExport TeamBaseMatchBase : public Pawn
    {
        public:
            TeamBaseMatchBase(BaseObject* creator);

            // if class closes, close everything
            virtual ~TeamBaseMatchBase() {}



            // Set the state of a base to whatever the argument of the function is
            void setState(BaseState::Enum state)
            {
                this->state_ = state;
                this->changeTeamColour();
            }


            // Get the state of a base as a return value
            BaseState::Enum getState() const
            {
                return this->state_;
            }


        protected:
            void changeTeamColour();

            BaseState::Enum state_;
    };
}

#endif /* _TeamBaseMatchBase_H__ */
