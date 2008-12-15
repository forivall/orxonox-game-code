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
#include "Spectator.h"

#include "core/CoreIncludes.h"
#include "core/ConfigValueIncludes.h"
#include "core/Core.h"
#include "objects/worldentities/Model.h"
#include "objects/Scene.h"
#include "objects/infos/PlayerInfo.h"
#include "objects/gametypes/Gametype.h"
#include "tools/BillboardSet.h"
#include "overlays/OverlayText.h"
#include "overlays/OverlayGroup.h"
#include "util/Convert.h"

namespace orxonox
{
    CreateFactory(Spectator);

    Spectator::Spectator(BaseObject* creator) : ControllableEntity(creator)
    {
        RegisterObject(Spectator);

        this->speed_ = 200;

        this->yaw_ = 0;
        this->pitch_ = 0;
        this->roll_ = 0;
        this->setHudTemplate("spectatorhud");

        this->setDestroyWhenPlayerLeft(true);

        this->greetingFlare_ = new BillboardSet();
        this->greetingFlare_->setBillboardSet(this->getScene()->getSceneManager(), "Examples/Flare", ColourValue(1.0, 1.0, 0.8), Vector3(0, 0, 0), 1);
        if (this->greetingFlare_->getBillboardSet())
            this->getNode()->attachObject(this->greetingFlare_->getBillboardSet());
        this->greetingFlare_->setVisible(false);
        this->bGreetingFlareVisible_ = false;
        this->bGreeting_ = false;

        this->setConfigValues();
        this->registerVariables();
    }

    Spectator::~Spectator()
    {
        if (this->isInitialized())
        {
            if (this->greetingFlare_)
            {
                if (this->greetingFlare_->getBillboardSet())
                    this->getNode()->detachObject(this->greetingFlare_->getBillboardSet());
                delete this->greetingFlare_;
            }
        }
    }

    void Spectator::setConfigValues()
    {
        SetConfigValue(speed_, 200.0f);
    }

    void Spectator::registerVariables()
    {
        REGISTERDATA(this->bGreetingFlareVisible_, direction::toclient, new NetworkCallback<Spectator>(this, &Spectator::changedFlareVisibility));
        REGISTERDATA(this->bGreeting_,             direction::toserver, new NetworkCallback<Spectator>(this, &Spectator::changedGreeting));
    }

    void Spectator::changedGreeting()
    {
        this->bGreetingFlareVisible_ = this->bGreeting_;
        this->changedFlareVisibility();
    }

    void Spectator::changedFlareVisibility()
    {
        this->greetingFlare_->setVisible(this->bGreetingFlareVisible_);
    }

    void Spectator::tick(float dt)
    {
        if (this->hasLocalController())
        {
            Vector3 velocity = this->getVelocity();
            velocity.normalise();
            this->setVelocity(velocity * this->speed_);

            if (!this->isInMouseLook())
            {
                this->yaw(Radian(this->yaw_ * this->getMouseLookSpeed()));
                this->pitch(Radian(this->pitch_ * this->getMouseLookSpeed()));
                this->roll(Radian(this->roll_ * this->getMouseLookSpeed()));
            }

            this->yaw_ = this->pitch_ = this->roll_ = 0;
        }

        SUPER(Spectator, tick, dt);

        if (this->hasLocalController())
        {
            this->setVelocity(Vector3::ZERO);
        }
    }

    void Spectator::setPlayer(PlayerInfo* player)
    {
        ControllableEntity::setPlayer(player);

//        this->setObjectMode(direction::toclient);
    }

    void Spectator::startLocalHumanControl()
    {
        ControllableEntity::startLocalHumanControl();
    }

    void Spectator::moveFrontBack(const Vector2& value)
    {
        this->setVelocity(this->getVelocity() + value.y * this->speed_ * WorldEntity::FRONT);
    }

    void Spectator::moveRightLeft(const Vector2& value)
    {
        this->setVelocity(this->getVelocity() + value.y * this->speed_ * WorldEntity::RIGHT);
    }

    void Spectator::moveUpDown(const Vector2& value)
    {
        this->setVelocity(this->getVelocity() + value.y * this->speed_ * WorldEntity::UP);
    }

    void Spectator::rotateYaw(const Vector2& value)
    {
        this->yaw_ = value.y;

        ControllableEntity::rotateYaw(value);
    }

    void Spectator::rotatePitch(const Vector2& value)
    {
        this->pitch_ = value.y;

        ControllableEntity::rotatePitch(value);
    }

    void Spectator::rotateRoll(const Vector2& value)
    {
        this->roll_ = value.y;

        ControllableEntity::rotateRoll(value);
    }

    void Spectator::fire()
    {
        if (this->getPlayer())
            this->getPlayer()->setReadyToSpawn(true);
    }

    void Spectator::greet()
    {
        this->bGreeting_ = !this->bGreeting_;

        if (Core::isMaster())
        {
            this->bGreetingFlareVisible_ = this->bGreeting_;
            this->changedFlareVisibility();
        }
    }
}
