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
 *      Martin Polak
 *   Co-authors:
 *      ...
 *
 */

#include "OrxonoxStableHeaders.h"

#include <vector>

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "util/Debug.h"

#include "WeaponSystem.h"

/* WEAPONSYSTEM
 * creates the WeaponSystem and the ability to use weapons and munition
 * loads the weapon the whole weaponSystem setting from an xml file
 *
 */

namespace orxonox
{
    WeaponSystem::WeaponSystem()
    {
        RegisterObject(WeaponSystem);
    }

    WeaponSystem::~WeaponSystem()
    {
    }

    //creates empty weaponSet
    void WeaponSystem::attachWeaponSet(WeaponSet *wSet)
    {
        this->weaponSets_.push_back(wSet);
        wSet->setParentWeaponSystem(this);
    }

    //the first weaponSet is at n=0
    void WeaponSystem::setActiveWeaponSet(int n)
    {
        if ( (int) weaponSets_.size() <= n )
            n=0;
        this->activeWeaponSet_ = this->weaponSets_[n];
    }

    //n is the n'th weaponSet, starting with zero
    //Spaceship.cc only needs to have the keybinding to a specific Set-number n
    void WeaponSystem::fire(int n)
    {
        if (n>=0)
            this->weaponSets_[n]->fire();
    }

    WeaponSet * getWeaponSetPointer(int n)
    {
        return this->weaponSets_[n];
    }

    void WeaponSystem::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {

    }

}
