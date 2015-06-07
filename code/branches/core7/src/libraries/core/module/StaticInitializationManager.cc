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

#include "StaticInitializationManager.h"

#include "StaticInitializationHandler.h"

namespace orxonox
{
    StaticInitializationManager* StaticInitializationManager::singletonPtr_s = 0;

    void StaticInitializationManager::addHandler(StaticInitializationHandler* handler)
    {
        handler->setupHandler();
        this->handlers_.push_back(handler);
    }

    void StaticInitializationManager::removeHandler(StaticInitializationHandler* handler)
    {
        this->handlers_.remove(handler);
        handler->shutdownHandler();
    }

    void StaticInitializationManager::loadModule(ModuleInstance* module)
    {
        // attention: loading a module may add new handlers to the list
        for (std::list<StaticInitializationHandler*>::iterator it = this->handlers_.begin(); it != this->handlers_.end(); ++it)
            (*it)->loadModule(module);
    }

    void StaticInitializationManager::unloadModule(ModuleInstance* module)
    {
        // unload in reversed order
        // attention: unloading a module may remove handlers from the list
        for (std::list<StaticInitializationHandler*>::reverse_iterator it = this->handlers_.rbegin(); it != this->handlers_.rend(); )
            (*(it++))->unloadModule(module);
    }
}
