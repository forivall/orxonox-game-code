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
 *      Benjamin Knecht
 *   Co-authors:
 *      ...
 *
 */

#include "Script.h"

#include "core/CoreIncludes.h"
#include "core/LuaState.h"
#include "core/XMLPort.h"

namespace orxonox
{
    CreateFactory(Script);

    Script::Script(BaseObject* creator) : BaseObject(creator)
    {
        RegisterObject(Script);

        // Get a new LuaState
        luaState_ = new LuaState();
    }

    Script::~Script()
    {
        if (this->isInitialized())
            delete luaState_;
    }

    void Script::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        BaseObject::XMLPort(xmlelement, mode);

        XMLPortParam(Script, "code", setCode, getCode, xmlelement, mode);
    }

    void Script::execute()
    {
        luaState_->doString(code_);
    }
}