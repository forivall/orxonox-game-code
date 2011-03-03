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
 *      Oli Scheuss
 *   Co-authors:
 *      Damian 'Mozork' Frick
 *
 */

#include "AutonomousDroneController.h"

#include "worldentities/AutonomousDrone.h"
#include "util/Math.h"

namespace orxonox
{

    /**
    @brief
        Constructor.
    @param creator
        The creator of this object.
    */
    AutonomousDroneController::AutonomousDroneController(BaseObject* creator) : Controller(creator)
    {
        //TODO: Place your code here:
        // Make sure to register the object in the factory.
        // Do some kind of initialisation.

        // This checks that our creator really is a drone
        // and saves the pointer to the drone for the controlling commands
        AutonomousDrone* drone = dynamic_cast<AutonomousDrone*>(creator);
        assert(drone != NULL);
        this->setControllableEntity(drone);
    }

    /**
    @brief
        Destructor.
    */
    AutonomousDroneController::~AutonomousDroneController()
    {

    }

    /**
    @brief
        The controlling happens here. This method defines what the controller has to do each tick.
    @param dt
        The duration of the tick.
    */
    void AutonomousDroneController::tick(float dt)
    {
        AutonomousDrone *myDrone = static_cast<AutonomousDrone*>(this->getControllableEntity());
        //TODO: Place your code here:
        // Steering commands
        // You can use the following commands for steering
        // - moveFrontBack, moveRightLeft, moveUpDown
        // - rotatePitch, rotateYaw, rotateRoll
        // Apply the to myDrone (e.g. myDrone->rotateYaw(..) )

    }
}
