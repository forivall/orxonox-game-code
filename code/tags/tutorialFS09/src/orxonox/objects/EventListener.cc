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
#include "EventListener.h"

#include "core/CoreIncludes.h"
#include "core/EventIncludes.h"
#include "core/XMLPort.h"

namespace orxonox
{
    CreateFactory(EventListener);

    EventListener::EventListener(BaseObject* creator) : BaseObject(creator)
    {
        RegisterObject(EventListener);

        this->bActive_ = false;
    }

    EventListener::~EventListener()
    {
    }

    void EventListener::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(EventListener, XMLPort, xmlelement, mode);

        XMLPortParam(EventListener, "event", setEventName, getEventName, xmlelement, mode);
    }

    void EventListener::processEvent(Event& event)
    {
        if (this->bActive_)
        {
            COUT(2) << "Warning: Detected Event loop in EventListener \"" << this->getName() << "\"" << std::endl;
            return;
        }

        this->bActive_ = true;

        this->fireEvent(event.activate_, event.originator_);

        this->bActive_ = false;
    }

    void EventListener::setEventName(const std::string& eventname)
    {
        this->eventName_ = eventname;

        if (this->eventName_ == "")
            return;

        for (ObjectList<BaseObject>::iterator it = ObjectList<BaseObject>::begin(); it != ObjectList<BaseObject>::end(); ++it)
            if (it->getName() == this->eventName_)
                this->registerAsListener(*it);
    }

    void EventListener::loadedNewXMLName(BaseObject* object)
    {
        if (this->eventName_ == "")
            return;

        if (object->getName() == this->eventName_)
            this->registerAsListener(object);
    }

    void EventListener::registerAsListener(BaseObject* object)
    {
        object->registerEventListener(this, "");
    }
}