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

#ifndef _EventTarget_H__
#define _EventTarget_H__

#include "objects/ObjectsPrereqs.h"

#include "core/BaseObject.h"
#include "core/XMLNameListener.h"

namespace orxonox
{
    class _ObjectsExport EventTarget : public BaseObject, public XMLNameListener
    {
        public:
            EventTarget(BaseObject* creator);
            virtual ~EventTarget();
            
            virtual void processEvent(Event& event);

            virtual void changedName();

        private:
            virtual void loadedNewXMLName(BaseObject* object);
            void addEventTarget(BaseObject* object);
    };
}

#endif /* _EventTarget_H__ */
