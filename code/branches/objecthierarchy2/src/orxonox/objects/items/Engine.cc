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
#include "Engine.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "objects/worldentities/pawns/SpaceShip.h"

namespace orxonox
{
    CreateFactory(Engine);

    Engine::Engine(BaseObject* creator) : Item(creator)
    {
        RegisterObject(Engine);

        this->ship_ = 0;
        this->shipID_ = OBJECTID_UNKNOWN;

        this->boostFactor_ = 1.5;
        this->speedFactor_ = 1.0;

        this->maxSpeedFront_ = 0.0;
        this->maxSpeedBack_ = 0.0;
        this->maxSpeedLeftRight_ = 0.0;
        this->maxSpeedUpDown_ = 0.0;

        this->accelerationFront_ = 0.0;
        this->accelerationBrake_ = 0.0;
        this->accelerationBack_ = 0.0;
        this->accelerationLeftRight_ = 0.0;
        this->accelerationUpDown_ = 0.0;

        this->registerVariables();
    }

    Engine::~Engine()
    {
    }

    void Engine::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(Engine, XMLPort, xmlelement, mode);

        XMLPortParam(Engine, "boostfactor", setBoostFactor, getBoostFactor, xmlelement, mode);

        XMLPortParam(Engine, "speedfront",     setMaxSpeedFront,     setMaxSpeedFront,     xmlelement, mode);
        XMLPortParam(Engine, "speedback",      setMaxSpeedBack,      setMaxSpeedBack,      xmlelement, mode);
        XMLPortParam(Engine, "speedleftright", setMaxSpeedLeftRight, setMaxSpeedLeftRight, xmlelement, mode);
        XMLPortParam(Engine, "speedupdown",    setMaxSpeedUpDown,    setMaxSpeedUpDown,    xmlelement, mode);

        XMLPortParam(Engine, "accelerationfront",     setAccelerationFront,     setAccelerationFront,     xmlelement, mode);
        XMLPortParam(Engine, "accelerationbrake",     setAccelerationBrake,     setAccelerationBrake,     xmlelement, mode);
        XMLPortParam(Engine, "accelerationback",      setAccelerationBack,      setAccelerationBack,      xmlelement, mode);
        XMLPortParam(Engine, "accelerationleftright", setAccelerationLeftRight, setAccelerationLeftRight, xmlelement, mode);
        XMLPortParam(Engine, "accelerationupdown",    setAccelerationUpDown,    setAccelerationUpDown,    xmlelement, mode);
    }

    void Engine::registerVariables()
    {
        REGISTERDATA(this->shipID_, direction::toclient, new NetworkCallback<Engine>(this, &Engine::networkcallback_shipID));

        REGISTERDATA(this->speedFactor_, direction::toclient);
        REGISTERDATA(this->boostFactor_, direction::toclient);

        REGISTERDATA(this->maxSpeedFront_,     direction::toclient);
        REGISTERDATA(this->maxSpeedBack_,      direction::toclient);
        REGISTERDATA(this->maxSpeedLeftRight_, direction::toclient);
        REGISTERDATA(this->maxSpeedUpDown_,    direction::toclient);

        REGISTERDATA(this->accelerationFront_,     direction::toclient);
        REGISTERDATA(this->accelerationBrake_,     direction::toclient);
        REGISTERDATA(this->accelerationBack_,      direction::toclient);
        REGISTERDATA(this->accelerationLeftRight_, direction::toclient);
        REGISTERDATA(this->accelerationUpDown_,    direction::toclient);
    }

    void Engine::networkcallback_shipID()
    {
        this->ship_ = 0;

        if (this->shipID_ != OBJECTID_UNKNOWN)
        {
            Synchronisable* object = Synchronisable::getSynchronisable(this->shipID_);
            if (object)
                this->ship_ = dynamic_cast<SpaceShip*>(object);
        }
    }

    void Engine::tick(float dt)
    {
        if (!this->ship_)
        {
            if (this->shipID_)
            {
                this->networkcallback_shipID();

                if (!this->ship_)
                    return;
            }
            else
                return;
        }

        if (!this->isActive())
            return;

        const Vector3& direction = this->getDirection();
        Vector3 velocity = this->ship_->getVelocity();
        Vector3 acceleration = Vector3::ZERO;

        float factor = 1.0f / this->speedFactor_;
        velocity *= factor;

        if (direction.z < 0)
        {
            if (this->maxSpeedFront_ != 0)
            {
                float boostfactor = (this->ship_->getBoost() ? this->boostFactor_ : 1.0f);
                acceleration.z = direction.z * this->accelerationFront_ * boostfactor * clamp((this->maxSpeedFront_ - -velocity.z/boostfactor) / this->maxSpeedFront_, 0.0f, 1.0f);
            }
        }
        else if (direction.z > 0)
        {
            if (velocity.z < 0)
                acceleration.z = direction.z * this->accelerationBrake_;
            else if (this->maxSpeedBack_ != 0)
                acceleration.z = direction.z * this->accelerationBack_ * clamp((this->maxSpeedBack_ - velocity.z) / this->maxSpeedBack_, 0.0f, 1.0f);
        }

        if (this->maxSpeedLeftRight_ != 0)
        {
            if (direction.x < 0)
                acceleration.x = direction.x * this->accelerationLeftRight_ * clamp((this->maxSpeedLeftRight_ - -velocity.x) / this->maxSpeedLeftRight_, 0.0f, 1.0f);
            else if (direction.x > 0)
                acceleration.x = direction.x * this->accelerationLeftRight_ * clamp((this->maxSpeedLeftRight_ - velocity.x) / this->maxSpeedLeftRight_, 0.0f, 1.0f);
        }

        if (this->maxSpeedUpDown_ != 0)
        {
            if (direction.y < 0)
                acceleration.y = direction.y * this->accelerationUpDown_ * clamp((this->maxSpeedUpDown_ - -velocity.y) / this->maxSpeedUpDown_, 0.0f, 1.0f);
            else if (direction.y > 0)
                acceleration.y = direction.y * this->accelerationUpDown_ * clamp((this->maxSpeedUpDown_ - velocity.y) / this->maxSpeedUpDown_, 0.0f, 1.0f);
        }

        this->ship_->setAcceleration(acceleration);

        this->ship_->setBoost(false);
        this->ship_->setSteeringDirection(Vector3::ZERO);
    }

    void Engine::addToSpaceShip(SpaceShip* ship)
    {
        this->ship_ = ship;
        if (ship)
            this->shipID_ = ship->getObjectID();
    }

    const Vector3& Engine::getDirection() const
    {
        if (this->ship_)
            return this->ship_->getSteeringDirection();
        else
            return Vector3::ZERO;
    }
}
