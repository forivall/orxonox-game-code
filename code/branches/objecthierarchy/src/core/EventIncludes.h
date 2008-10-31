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

#ifndef _EventIncludes_H__
#define _EventIncludes_H__

#include "CorePrereqs.h"
#include "Executor.h"

#define SetEvent(classname, eventname, functionname, event) \
    SetEventGeneric(eventcontainer##classname##functionname, classname, eventname, orxonox::createExecutor(orxonox::createFunctor(&classname::functionname), std::string( #classname ) + "::" + #functionname), event, BaseObject)

#define SetEventTemplate(classname, eventname, functionname, event, ...) \
    SetEventGeneric(eventcontainer##classname##functionname, classname, eventname, orxonox::createExecutor(orxonox::createFunctor<classname, __VA_ARGS__ >(&classname::functionname), std::string( #classname ) + "::" + #functionname), event, BaseObject)

#define SetSubclassEvent(classname, eventname, functionname, event, subclassname) \
    SetEventGeneric(eventcontainer##classname##functionname, classname, eventname, orxonox::createExecutor(orxonox::createFunctor(&classname::functionname), std::string( #classname ) + "::" + #functionname), event, subclassname)

#define SetSubclassEventTemplate(classname, eventname, functionname, event, subclassname, ...) \
    SetEventGeneric(eventcontainer##classname##functionname, classname, eventname, orxonox::createExecutor(orxonox::createFunctor<classname, __VA_ARGS__ >(&classname::functionname), std::string( #classname ) + "::" + #functionname), event, subclassname)

#define SetEventGeneric(containername, classname, eventname, executor, event, subclassname) \
    orxonox::EventContainer* containername = this->getEventContainer(eventname); \
    if (!containername) \
    { \
        containername = new orxonox::ClassEventContainer<classname>(std::string(eventname), executor, orxonox::ClassIdentifier<subclassname>::getIdentifier()); \
        this->addEventContainer(eventname, containername); \
    } \
    event.castedOriginator_ = dynamic_cast<subclassname*>(event.originator_); \
    containername->process(this, event)

#endif /* _EventIncludes_H__ */
