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
 *      Reto Grieder (physics)
 *   Co-authors:
 *      ...
 *
 */

#include "OrxonoxStableHeaders.h"
#include "WorldEntity.h"

#include <cassert>
#include <OgreSceneManager.h>
#include "BulletDynamics/Dynamics/btRigidBody.h"

#include "util/Exception.h"
#include "util/Convert.h"
#include "core/CoreIncludes.h"
#include "core/XMLPort.h"

#include "objects/Scene.h"
#include "objects/collisionshapes/CompoundCollisionShape.h"

namespace orxonox
{
    const Vector3 WorldEntity::FRONT = Vector3::NEGATIVE_UNIT_Z;
    const Vector3 WorldEntity::BACK  = Vector3::UNIT_Z;
    const Vector3 WorldEntity::LEFT  = Vector3::NEGATIVE_UNIT_X;
    const Vector3 WorldEntity::RIGHT = Vector3::UNIT_X;
    const Vector3 WorldEntity::DOWN  = Vector3::NEGATIVE_UNIT_Y;
    const Vector3 WorldEntity::UP    = Vector3::UNIT_Y;

    WorldEntity::WorldEntity(BaseObject* creator) : BaseObject(creator), network::Synchronisable(creator)
    {
        RegisterObject(WorldEntity);

        assert(this->getScene());
        assert(this->getScene()->getRootSceneNode());

        this->node_ = this->getScene()->getRootSceneNode()->createChildSceneNode();

        this->parent_ = 0;
        this->parentID_ = (unsigned int)-1;

        this->node_->setPosition(Vector3::ZERO);
        this->node_->setOrientation(Quaternion::IDENTITY);

        // Default behaviour does not include physics
        this->physicalBody_ = 0;
        this->bPhysicsActive_ = false;
        this->collisionShape_ = new CompoundCollisionShape(this);
        this->mass_ = 0;
        this->childrenMass_ = 0;
        this->collisionType_ = None;
        this->collisionTypeSynchronised_ = None;

        this->registerVariables();
    }

    WorldEntity::~WorldEntity()
    {
        if (this->isInitialized())
        {
            this->node_->detachAllObjects();
            if (this->getScene()->getSceneManager())
                this->getScene()->getSceneManager()->destroySceneNode(this->node_->getName());

            // TODO: Detach from parent and detach all children.

            if (this->physicalBody_)
            {
                this->deactivatePhysics();
                delete this->physicalBody_;
            }
            delete this->collisionShape_;
        }
    }

    void WorldEntity::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(WorldEntity, XMLPort, xmlelement, mode);

        XMLPortParamTemplate(WorldEntity, "position", setPosition, getPosition, xmlelement, mode, const Vector3&);
        XMLPortParamTemplate(WorldEntity, "orientation", setOrientation, getOrientation, xmlelement, mode, const Quaternion&);
        XMLPortParamLoadOnly(WorldEntity, "lookat", lookAt_xmlport, xmlelement, mode);
        XMLPortParamLoadOnly(WorldEntity, "direction", setDirection_xmlport, xmlelement, mode);
        XMLPortParamLoadOnly(WorldEntity, "yaw", yaw_xmlport, xmlelement, mode);
        XMLPortParamLoadOnly(WorldEntity, "pitch", pitch_xmlport, xmlelement, mode);
        XMLPortParamLoadOnly(WorldEntity, "roll", roll_xmlport, xmlelement, mode);
        XMLPortParamTemplate(WorldEntity, "scale3D", setScale3D, getScale3D, xmlelement, mode, const Vector3&);
        XMLPortParam(WorldEntity, "scale", setScale, getScale, xmlelement, mode);

        // Physics
        XMLPortParam(WorldEntity, "collisionType", setCollisionTypeStr, getCollisionTypeStr, xmlelement, mode);
        XMLPortParam(WorldEntity, "mass", setMass, getMass, xmlelement, mode);

        // Other attached WorldEntities
        XMLPortObject(WorldEntity, WorldEntity, "attached", attach, getAttachedObject, xmlelement, mode);
        // Attached collision shapes
        XMLPortObject(WorldEntity, CollisionShape, "collisionShapes", attachCollisionShape, getAttachedCollisionShape, xmlelement, mode);
    }

    void WorldEntity::registerVariables()
    {
        REGISTERDATA(this->bActive_,     network::direction::toclient, new network::NetworkCallback<WorldEntity>(this, &WorldEntity::changedActivity));
        REGISTERDATA(this->bVisible_,    network::direction::toclient, new network::NetworkCallback<WorldEntity>(this, &WorldEntity::changedVisibility));

        REGISTERDATA(this->getScale3D(), network::direction::toclient, new network::NetworkCallback<WorldEntity>(this, &WorldEntity::scaleChanged));

        REGISTERDATA((int&)this->collisionTypeSynchronised_,
                                         network::direction::toclient, new network::NetworkCallback<WorldEntity>(this, &WorldEntity::collisionTypeChanged));
        REGISTERDATA(this->mass_,        network::direction::toclient, new network::NetworkCallback<WorldEntity>(this, &WorldEntity::massChanged));
        REGISTERDATA(this->bPhysicsActiveSynchronised_,
                                         network::direction::toclient, new network::NetworkCallback<WorldEntity>(this, &WorldEntity::physicsActivityChanged));

        REGISTERDATA(this->parentID_,    network::direction::toclient, new network::NetworkCallback<WorldEntity>(this, &WorldEntity::updateParent));
    }

    void WorldEntity::updateParent()
    {
        WorldEntity* parent = dynamic_cast<WorldEntity*>(Synchronisable::getSynchronisable(this->parentID_));
        if (parent)
            this->attachToParent(parent);
    }

    void WorldEntity::collisionTypeChanged()
    {
        if (this->collisionTypeSynchronised_ != Dynamic &&
            this->collisionTypeSynchronised_ != Kinematic &&
            this->collisionTypeSynchronised_ != Static &&
            this->collisionTypeSynchronised_ != None)
        {
            CCOUT(1) << "Error when collsion Type was received over network. Unknown enum value:" << this->collisionTypeSynchronised_ << std::endl;
        }
        else if (this->collisionTypeSynchronised_ != collisionType_)
        {
            if (this->parent_)
                CCOUT(2) << "Warning: Network connection tried to set the collision type of an attached WE. Ignoring." << std::endl;
            else
                this->setCollisionType(this->collisionTypeSynchronised_);
        }
    }

    void WorldEntity::massChanged()
    {
        this->setMass(this->mass_);
    }

    void WorldEntity::physicsActivityChanged()
    {
        if (this->bPhysicsActiveSynchronised_)
            this->activatePhysics();
        else
            this->deactivatePhysics();
    }

    void WorldEntity::attach(WorldEntity* object)
    {
        // check first whether attaching is even allowed
        if (object->hasPhysics())
        {
            if (!this->hasPhysics())
                ThrowException(PhysicsViolation, "Cannot attach a physical object to a non physical one.");
            else if (object->isDynamic())
                ThrowException(PhysicsViolation, "Cannot attach a dynamic object to a WorldEntity.");
            else if (object->isKinematic() && this->isDynamic())
                ThrowException(PhysicsViolation, "Cannot attach a kinematic object to a dynamic one.");
            else if (object->isKinematic())
                ThrowException(NotImplemented, "Cannot attach a kinematic object to a static or kinematic one: Not yet implemented.");
            else
            {
                object->deactivatePhysics();
            }
        }

        if (object->getParent())
            object->detachFromParent();
        else
        {
            Ogre::Node* parent = object->node_->getParent();
            if (parent)
                parent->removeChild(object->node_);
        }

        this->node_->addChild(object->node_);
        this->children_.insert(object);
        object->parent_ = this;
        object->parentID_ = this->getObjectID();

        // collision shapes
        this->attachCollisionShape(object->getCollisionShape());
        // mass
        this->childrenMass_ += object->getMass();
        recalculatePhysicsProps();
    }

    void WorldEntity::detach(WorldEntity* object)
    {
        // collision shapes
        this->detachCollisionShape(object->getCollisionShape());
        // mass
        if (object->getMass() > 0.0f)
        {
            this->childrenMass_ -= object->getMass();
            recalculatePhysicsProps();
        }

        this->node_->removeChild(object->node_);
        this->children_.erase(object);
        object->parent_ = 0;
        object->parentID_ = (unsigned int)-1;
//        this->getScene()->getRootSceneNode()->addChild(object->node_);

        // Note: It is possible that the object has physics but was disabled when attaching
        object->activatePhysics();
    }

    WorldEntity* WorldEntity::getAttachedObject(unsigned int index) const
    {
        unsigned int i = 0;
        for (std::set<WorldEntity*>::const_iterator it = this->children_.begin(); it != this->children_.end(); ++it)
        {
            if (i == index)
                return (*it);
            ++i;
        }
        return 0;
    }

    void WorldEntity::attachCollisionShape(CollisionShape* shape)
    {
        this->collisionShape_->addChildShape(shape);
        // Note: this->collisionShape_ already notifies us of any changes.
    }

    void WorldEntity::detachCollisionShape(CollisionShape* shape)
    {
        this->collisionShape_->removeChildShape(shape);
        // Note: this->collisionShape_ already notifies us of any changes.
    }

    CollisionShape* WorldEntity::getAttachedCollisionShape(unsigned int index) const
    {
        return this->collisionShape_->getChildShape(index);
    }

    void WorldEntity::activatePhysics()
    {
        if (this->isActive() && this->hasPhysics() && !this->isPhysicsActive() && !this->parent_)
        {
            this->getScene()->addRigidBody(this->physicalBody_);
            this->bPhysicsActive_ = true;
        }
    }

    void WorldEntity::deactivatePhysics()
    {
        if (this->isPhysicsActive())
        {
            this->getScene()->removeRigidBody(this->physicalBody_);
            this->bPhysicsActive_ = false;
        }
    }

    bool WorldEntity::addedToPhysicalWorld() const
    {
        return this->physicalBody_ && this->physicalBody_->isInWorld();
    }

    void WorldEntity::setScale3D(const Vector3& scale)
    {
        if (this->hasPhysics())
            ThrowException(NotImplemented, "Cannot set the scale of a physical object: Not yet implemented.");

        this->node_->setScale(scale);
    }

    void WorldEntity::scale3D(const Vector3& scale)
    {
        if (this->hasPhysics())
            ThrowException(NotImplemented, "Cannot set the scale of a physical object: Not yet implemented.");

        this->node_->scale(scale);
    }

    void WorldEntity::setMass(float mass)
    {
        this->mass_ = mass;
        recalculatePhysicsProps();
    }

    void WorldEntity::setCollisionType(CollisionType type)
    {
        // If we are already attached to a parent, this would be a bad idea..
        if (this->parent_)
            ThrowException(PhysicsViolation, "Cannot set the collision type of a WorldEntity with a parent");
        else if (this->addedToPhysicalWorld())
            ThrowException(PhysicsViolation, "Warning: Cannot set the collision type at run time.");

        // Check for type legality. Could be StaticEntity or MobileEntity
        if (!this->isCollisionTypeLegal(type))
            return; // exception gets issued anyway

        // Check whether we have to create or destroy.
        if (type != None && this->collisionType_ == None)
        {
            // Check whether there was some scaling applied.
            if (!this->node_->getScale().positionEquals(Vector3(1, 1, 1), 0.001))
                ThrowException(NotImplemented, "Cannot create a physical body if there is scaling applied to the node: Not yet implemented.");

            // Create new rigid body
            btCollisionShape* temp = 0;
            btRigidBody::btRigidBodyConstructionInfo bodyConstructionInfo(0, this, this->collisionShape_->getCollisionShape());
            bodyConstructionInfo.m_restitution = 1;
            this->physicalBody_ = new btRigidBody(bodyConstructionInfo);
            this->physicalBody_->setUserPointer(this);
            this->physicalBody_->setActivationState(DISABLE_DEACTIVATION);
        }
        else if (type == None && this->collisionType_ != None)
        {
            // Destroy rigid body
            assert(this->physicalBody_);
            deactivatePhysics();
            delete this->physicalBody_;
            this->physicalBody_ = 0;
            this->collisionType_ = None;
            this->collisionTypeSynchronised_ = None;
            return;
        }

        // Change type
        switch (type)
        {
        case Dynamic:
            this->physicalBody_->setCollisionFlags(this->physicalBody_->getCollisionFlags() & !(btCollisionObject::CF_STATIC_OBJECT | btCollisionObject::CF_KINEMATIC_OBJECT));
            break;
        case Kinematic:
            this->physicalBody_->setCollisionFlags(this->physicalBody_->getCollisionFlags() & !btCollisionObject::CF_STATIC_OBJECT | btCollisionObject::CF_KINEMATIC_OBJECT);
            break;
        case Static:
            this->physicalBody_->setCollisionFlags(this->physicalBody_->getCollisionFlags() & !btCollisionObject::CF_KINEMATIC_OBJECT | btCollisionObject::CF_STATIC_OBJECT);
            break;
        case None:
            return; // this->collisionType_ was None too
        }

        // Only sets this->collisionShape_
        // However the assertion is to ensure that the internal bullet setting is right
        updateCollisionType();
        assert(this->collisionType_ == type);

        // update mass and inertia tensor
        recalculatePhysicsProps();
        activatePhysics();
    }

    void WorldEntity::setCollisionTypeStr(const std::string& typeStr)
    {
        std::string typeStrLower = getLowercase(typeStr);
        CollisionType type;
        if (typeStrLower == "dynamic")
            type = Dynamic;
        else if (typeStrLower == "static")
            type = Static;
        else if (typeStrLower == "kinematic")
            type = Kinematic;
        else if (typeStrLower == "none")
            type = None;
        else
            ThrowException(ParseError, std::string("Attempting to set an unknown collision type: '") + typeStr + "'.");
        this->setCollisionType(type);
    }

    std::string WorldEntity::getCollisionTypeStr() const
    {
        switch (this->getCollisionType())
        {
            case Dynamic:
                return "dynamic";
            case Kinematic:
                return "kinematic";
            case Static:
                return "static";
            case None:
                return "none";
            default:
                assert(false);
                return "";
        }
    }

    void WorldEntity::updateCollisionType()
    {
        if (!this->physicalBody_)
            this->collisionType_ = None;
        else if (this->physicalBody_->isKinematicObject())
            this->collisionType_ = Kinematic;
        else if (this->physicalBody_->isStaticObject())
            this->collisionType_ = Static;
        else
            this->collisionType_ = Dynamic;
        this->collisionTypeSynchronised_ = this->collisionType_;
    }

    void WorldEntity::notifyChildMassChanged() // Called by a child WE
    {
        // Note: CollisionShape changes of a child get handled over the internal CompoundCollisionShape already
        // Recalculate mass
        this->childrenMass_ = 0.0f;
        for (std::set<WorldEntity*>::const_iterator it = this->children_.begin(); it != this->children_.end(); ++it)
            this->childrenMass_ += (*it)->getMass();
        recalculatePhysicsProps();
        // Notify parent WE
        if (this->parent_)
            parent_->notifyChildMassChanged();
    }

    void WorldEntity::notifyCollisionShapeChanged() // called by this->collisionShape_
    {
        if (hasPhysics())
        {
            // Bullet doesn't like sudden changes of the collision shape, so we remove and add it again
            if (this->addedToPhysicalWorld())
            {
                this->deactivatePhysics();
                this->physicalBody_->setCollisionShape(this->collisionShape_->getCollisionShape());
                this->activatePhysics();
            }
            else
                this->physicalBody_->setCollisionShape(this->collisionShape_->getCollisionShape());
        }
        recalculatePhysicsProps();
    }

    void WorldEntity::recalculatePhysicsProps()
    {
        if (this->hasPhysics())
        {
            if (this->isStatic())
            {
                // Just set everything to zero
                this->physicalBody_->setMassProps(0.0f, btVector3(0, 0, 0));
            }
            else if ((this->mass_ + this->childrenMass_) == 0.0f)
            {
                // Use default values to avoid very large or very small values
                CCOUT(4) << "Warning: Setting the internal physical mass to 1.0 because mass_ is 0.0." << std::endl;
                this->physicalBody_->setMassProps(1.0f, this->collisionShape_->getLocalInertia(1.0f));
            }
            else
            {
                float mass = this->mass_ + this->childrenMass_;
                this->physicalBody_->setMassProps(mass, this->collisionShape_->getLocalInertia(mass));
            }
        }
    }
}
