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
 *      ... *
 */

#include "OrxonoxStableHeaders.h"

#include <vector>

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "util/Debug.h"

#include "WeaponSet.h"

namespace orxonox
{
    WeaponSet::WeaponSet()
    {
        RegisterObject(WeaponSet);
    }

    WeaponSet::~WeaponSet()
    {
    }

    //Vorwärtsdeklaration
    WeaponSystem * parentWeaponSystem_;

    void WeaponSet::attachWeaponSlot(WeaponSlot *wSlot)
    {
        this->weaponSlots_.push_back(wSlot);
    }

    void WeaponSet::fire()
    {
        for (int i=0; i < (int) this->weaponSlots_.size(); i++)
        {
            this->weaponSlots_[i]->fire();
        }
    }

    void WeaponSet::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {

    }

}
