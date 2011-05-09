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
#include "ControllableEntity.h"

#include "core/CoreIncludes.h"
#include "core/Core.h"
#include "core/XMLPort.h"
#include "core/Template.h"

#include "objects/infos/PlayerInfo.h"
#include "objects/worldentities/Camera.h"
#include "objects/worldentities/CameraPosition.h"
#include "overlays/OverlayGroup.h"

namespace orxonox
{
    CreateFactory(ControllableEntity);

    ControllableEntity::ControllableEntity(BaseObject* creator) : WorldEntity(creator)
    {
        RegisterObject(ControllableEntity);

        this->bControlled_ = false;
        this->server_overwrite_ = 0;
        this->client_overwrite_ = 0;
        this->player_ = 0;
        this->playerID_ = OBJECTID_UNKNOWN;
        this->hud_ = 0;
        this->camera_ = 0;
        this->bDestroyWhenPlayerLeft_ = false;

        this->velocity_ = Vector3::ZERO;
        this->acceleration_ = Vector3::ZERO;

        this->server_position_ = Vector3::ZERO;
        this->client_position_ = Vector3::ZERO;
        this->server_velocity_ = Vector3::ZERO;
        this->client_velocity_ = Vector3::ZERO;
        this->server_orientation_ = Quaternion::IDENTITY;
        this->client_orientation_ = Quaternion::IDENTITY;

        this->registerVariables();
    }

    ControllableEntity::~ControllableEntity()
    {
        if (this->isInitialized())
        {
            if (this->bControlled_)
                this->stopLocalControl();

            if (this->hud_)
                delete this->hud_;

            if (this->camera_)
                delete this->camera_;

            if (this->getPlayer() && this->getPlayer()->getControllableEntity() == this)
                this->getPlayer()->stopControl(this, false);
        }
    }

    void ControllableEntity::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(ControllableEntity, XMLPort, xmlelement, mode);

        XMLPortParam(ControllableEntity, "hudtemplate", setHudTemplate, getHudTemplate, xmlelement, mode);
        XMLPortParam(ControllableEntity, "camerapositiontemplate", setCameraPositionTemplate, getCameraPositionTemkplate, xmlelement, mode);

        XMLPortObject(ControllableEntity, CameraPosition, "camerapositions", addCameraPosition, getCameraPosition, xmlelement, mode);
    }

    void ControllableEntity::addCameraPosition(CameraPosition* position)
    {
        this->attach(position);
        this->cameraPositions_.push_back(position);
    }

    CameraPosition* ControllableEntity::getCameraPosition(unsigned int index) const
    {
        unsigned int i = 0;
        for (std::list<CameraPosition*>::const_iterator it = this->cameraPositions_.begin(); it != this->cameraPositions_.end(); ++it)
        {
            if (i == index)
                return (*it);
            ++i;
        }
        return 0;
    }

    void ControllableEntity::switchCamera()
    {
        if (this->camera_)
        {
            if (this->camera_->getParent() == this && this->cameraPositions_.size() > 0)
            {
                this->cameraPositions_.front()->attachCamera(this->camera_);
            }
            else if (this->cameraPositions_.size() > 0)
            {
                for (std::list<CameraPosition*>::const_iterator it = this->cameraPositions_.begin(); it != this->cameraPositions_.end(); ++it)
                {
                    if ((*it) == this->camera_->getParent())
                    {
                        ++it;
                        if (it != this->cameraPositions_.end())
                            (*it)->attachCamera(this->camera_);
                        else
                            (*this->cameraPositions_.begin())->attachCamera(this->camera_);
                        break;
                    }
                }
            }
            else
            {
                this->attach(this->camera_);
            }
        }
    }

    void ControllableEntity::setPlayer(PlayerInfo* player)
    {
        if (!player)
        {
            this->removePlayer();
            return;
        }

        this->player_ = player;
        this->playerID_ = player->getObjectID();
        this->bControlled_ = (player->isLocalPlayer() && player->isHumanPlayer());
        if (this->bControlled_)
        {
            this->startLocalControl();

            if (!Core::isMaster())
            {
                this->client_overwrite_ = this->server_overwrite_;
COUT(0) << "CE: bidirectional synchronization" << std::endl;
                this->setObjectMode(direction::bidirectional);
            }
        }
    }

    void ControllableEntity::removePlayer()
    {
        if (this->bControlled_)
            this->stopLocalControl();

        this->player_ = 0;
        this->playerID_ = OBJECTID_UNKNOWN;
        this->bControlled_ = false;
        this->setObjectMode(direction::toclient);

        if (this->bDestroyWhenPlayerLeft_)
            delete this;
    }

    void ControllableEntity::networkcallback_changedplayerID()
    {
        // just do this in case the entity wasn't yet synchronized when the corresponding PlayerInfo got our objectID
        if (this->playerID_ != OBJECTID_UNKNOWN)
        {
            this->player_ = dynamic_cast<PlayerInfo*>(Synchronisable::getSynchronisable(this->playerID_));
            if (this->player_ && (this->player_->getControllableEntity() != this))
                this->player_->startControl(this);
        }
    }

    void ControllableEntity::startLocalControl()
    {
//        std::cout << this->getObjectID() << " ###### start local control" << std::endl;
        this->camera_ = new Camera(this);
        this->camera_->requestFocus();
        if (this->cameraPositionTemplate_ != "")
            this->addTemplate(this->cameraPositionTemplate_);
        if (this->cameraPositions_.size() > 0)
            this->cameraPositions_.front()->attachCamera(this->camera_);
        else
            this->attach(this->camera_);

        if (this->hudtemplate_ != "")
        {
            this->hud_ = new OverlayGroup(this);
            this->hud_->addTemplate(this->hudtemplate_);
        }
    }

    void ControllableEntity::stopLocalControl()
    {
//        std::cout << "###### stop local control" << std::endl;
        this->camera_->detachFromParent();
        delete this->camera_;
        this->camera_ = 0;

        delete this->hud_;
        this->hud_ = 0;
    }

    void ControllableEntity::tick(float dt)
    {
        if (this->isActive())
        {
            this->velocity_ += (dt * this->acceleration_);
            this->node_->translate(dt * this->velocity_, Ogre::Node::TS_LOCAL);

            if (Core::isMaster())
            {
                this->server_velocity_ = this->velocity_;
                this->server_position_ = this->node_->getPosition();
            }
            else if (this->bControlled_)
            {
//                COUT(2) << "setting client position" << endl;
                this->client_velocity_ = this->velocity_;
                this->client_position_ = this->node_->getPosition();
            }
        }
    }

    void ControllableEntity::registerVariables()
    {
        REGISTERSTRING(this->cameraPositionTemplate_, direction::toclient);

        REGISTERDATA(this->client_overwrite_,   direction::toserver);
        
        REGISTERDATA(this->server_position_,    direction::toclient, new NetworkCallback<ControllableEntity>(this, &ControllableEntity::processServerPosition));
        REGISTERDATA(this->server_velocity_,    direction::toclient, new NetworkCallback<ControllableEntity>(this, &ControllableEntity::processServerVelocity));
        REGISTERDATA(this->server_orientation_, direction::toclient, new NetworkCallback<ControllableEntity>(this, &ControllableEntity::processServerOrientation));
        REGISTERDATA(this->server_overwrite_,   direction::toclient, new NetworkCallback<ControllableEntity>(this, &ControllableEntity::processOverwrite));

        REGISTERDATA(this->client_position_,    direction::toserver, new NetworkCallback<ControllableEntity>(this, &ControllableEntity::processClientPosition));
        REGISTERDATA(this->client_velocity_,    direction::toserver, new NetworkCallback<ControllableEntity>(this, &ControllableEntity::processClientVelocity));
        REGISTERDATA(this->client_orientation_, direction::toserver, new NetworkCallback<ControllableEntity>(this, &ControllableEntity::processClientOrientation));


        REGISTERDATA(this->playerID_, direction::toclient, new NetworkCallback<ControllableEntity>(this, &ControllableEntity::networkcallback_changedplayerID));
    }

    void ControllableEntity::processServerPosition()
    {
        if (!this->bControlled_)
            this->node_->setPosition(this->server_position_);
    }

    void ControllableEntity::processServerVelocity()
    {
        if (!this->bControlled_)
            this->velocity_ = this->server_velocity_;
    }

    void ControllableEntity::processServerOrientation()
    {
        if (!this->bControlled_)
            this->node_->setOrientation(this->server_orientation_);
    }

    void ControllableEntity::processOverwrite()
    {
        if (this->bControlled_)
        {
            this->setPosition(this->server_position_);
            this->setVelocity(this->server_velocity_);
            this->setOrientation(this->server_orientation_);

            this->client_overwrite_ = this->server_overwrite_;
        }
    }

    void ControllableEntity::processClientPosition()
    {
        if (this->server_overwrite_ == this->client_overwrite_)
        {
//            COUT(2) << "callback: setting client position" << endl;
            this->node_->setPosition(this->client_position_);
            this->server_position_ = this->client_position_;
        }
//        else
//          COUT(2) << "callback: not setting client position" << endl;
    }

    void ControllableEntity::processClientVelocity()
    {
        if (this->server_overwrite_ == this->client_overwrite_)
        {
            this->velocity_ = this->client_velocity_;
            this->server_velocity_ = this->client_velocity_;
        }
    }

    void ControllableEntity::processClientOrientation()
    {
        if (this->server_overwrite_ == this->client_overwrite_)
        {
            this->node_->setOrientation(this->client_orientation_);
            this->server_orientation_ = this->client_orientation_;
        }
    }


    void ControllableEntity::setPosition(const Vector3& position)
    {
        if (Core::isMaster())
        {
            this->node_->setPosition(position);
            this->server_position_ = position;
            ++this->server_overwrite_;
        }
        else if (this->bControlled_)
        {
            this->node_->setPosition(position);
            this->client_position_ = position;
        }
    }

    void ControllableEntity::setVelocity(const Vector3& velocity)
    {
        if (Core::isMaster())
        {
            this->velocity_ = velocity;
            this->server_velocity_ = velocity;
            ++this->server_overwrite_;
        }
        else if (this->bControlled_)
        {
            this->velocity_ = velocity;
            this->client_velocity_ = velocity;
        }
    }

    void ControllableEntity::translate(const Vector3& distance, Ogre::Node::TransformSpace relativeTo)
    {
        if (Core::isMaster())
        {
            this->node_->translate(distance, relativeTo);
            this->server_position_ = this->node_->getPosition();
            ++this->server_overwrite_;
        }
        else if (this->bControlled_)
        {
            this->node_->translate(distance, relativeTo);
            this->client_position_ = this->node_->getPosition();
        }
    }

    void ControllableEntity::setOrientation(const Quaternion& orientation)
    {
        if (Core::isMaster())
        {
            this->node_->setOrientation(orientation);
            this->server_orientation_ = orientation;
            ++this->server_overwrite_;
        }
        else if (this->bControlled_)
        {
            this->node_->setOrientation(orientation);
            this->client_orientation_ = orientation;
        }
    }

    void ControllableEntity::rotate(const Quaternion& rotation, Ogre::Node::TransformSpace relativeTo)
    {
        if (Core::isMaster())
        {
            this->node_->rotate(rotation, relativeTo);
            this->server_orientation_ = this->node_->getOrientation();
            ++this->server_overwrite_;
        }
        else if (this->bControlled_)
        {
            this->node_->rotate(rotation, relativeTo);
            this->client_orientation_ = this->node_->getOrientation();
        }
    }

    void ControllableEntity::yaw(const Degree& angle, Ogre::Node::TransformSpace relativeTo)
    {
        if (Core::isMaster())
        {
            this->node_->yaw(angle, relativeTo);
            this->server_orientation_ = this->node_->getOrientation();
            ++this->server_overwrite_;
        }
        else if (this->bControlled_)
        {
            this->node_->yaw(angle, relativeTo);
            this->client_orientation_ = this->node_->getOrientation();
        }
    }

    void ControllableEntity::pitch(const Degree& angle, Ogre::Node::TransformSpace relativeTo)
    {
        if (Core::isMaster())
        {
            this->node_->pitch(angle, relativeTo);
            this->server_orientation_ = this->node_->getOrientation();
            ++this->server_overwrite_;
        }
        else if (this->bControlled_)
        {
            this->node_->pitch(angle, relativeTo);
            this->client_orientation_ = this->node_->getOrientation();
        }
    }

    void ControllableEntity::roll(const Degree& angle, Ogre::Node::TransformSpace relativeTo)
    {
        if (Core::isMaster())
        {
            this->node_->roll(angle, relativeTo);
            this->server_orientation_ = this->node_->getOrientation();
            ++this->server_overwrite_;
        }
        else if (this->bControlled_)
        {
            this->node_->roll(angle, relativeTo);
            this->client_orientation_ = this->node_->getOrientation();
        }
    }

    void ControllableEntity::lookAt(const Vector3& target, Ogre::Node::TransformSpace relativeTo, const Vector3& localDirectionVector)
    {
        if (Core::isMaster())
        {
            this->node_->lookAt(target, relativeTo, localDirectionVector);
            this->server_orientation_ = this->node_->getOrientation();
            ++this->server_overwrite_;
        }
        else if (this->bControlled_)
        {
            this->node_->lookAt(target, relativeTo, localDirectionVector);
            this->client_orientation_ = this->node_->getOrientation();
        }
    }

    void ControllableEntity::setDirection(const Vector3& direction, Ogre::Node::TransformSpace relativeTo, const Vector3& localDirectionVector)
    {
        if (Core::isMaster())
        {
            this->node_->setDirection(direction, relativeTo, localDirectionVector);
            this->server_orientation_ = this->node_->getOrientation();
            ++this->server_overwrite_;
        }
        else if (this->bControlled_)
        {
            this->node_->setDirection(direction, relativeTo, localDirectionVector);
            this->client_orientation_ = this->node_->getOrientation();
        }
    }
}