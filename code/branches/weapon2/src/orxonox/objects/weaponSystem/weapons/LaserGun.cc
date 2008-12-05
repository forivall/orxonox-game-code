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


#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "util/Debug.h"

#include "LaserGun.h"


namespace orxonox
{
    CreateFactory(LaserGun);

    LaserGun::LaserGun(BaseObject* creator) : Weapon(creator)
    {
        RegisterObject(LaserGun);

        //set weapon properties here
        //this->setLoadingTime(0.5);

COUT(0) << "LaserGun::LaserGun" << std::endl;
        this->attachNeededMunition(this->getMunitionType());
    }

    LaserGun::~LaserGun()
    {
    }

    void LaserGun::fire()
    {
COUT(0) << "LaserGun::fire" << std::endl;
        if ( this->getBulletReadyToShoot() && this->getMagazineReadyToShoot() )
        {
COUT(0) << "LaserGun::fire - ready to shoot" << std::endl;
            //take munition
            this->setBulletReadyToShoot(false);
            Weapon::bulletTimer();
            this->getAttachedMunition(this->munitionType_)->removeBullets(1,this);

            //create projectile
            BillboardProjectile* projectile = new ParticleProjectile(this);
            projectile->setOrientation(projectile->getOrientation());
            projectile->setPosition(projectile->getWorldPosition());
            projectile->setVelocity(WorldEntity::FRONT * this->speed_);

            //projectile->setColour(this->getProjectileColour());
        }
        else
        {
            //actions, when weapon is not reloaded if there are some
        }
    }

    void LaserGun::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {

    }



}
