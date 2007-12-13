/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
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
 *      ...
 *   Co-authors:
 *      ...
 *
 */

#include "ParticleInterface.h"
#include <Ogre.h>
#include <OIS/OIS.h>
#include <CEGUI/CEGUI.h>
#include <CEGUIRenderer.h>


using namespace Ogre;

namespace particle {

	ParticleInterface::~ParticleInterface(void)
	{
		sceneManager_->destroyParticleSystem(particleSystem_);

		delete sceneNode_;
		delete particleSystem_;
		delete sceneManager_;
	}

	ParticleInterface::ParticleInterface( SceneManager *sceneManager, String name, String templateName )
	{
		sceneManager_ = sceneManager;
		particleSystem_ = sceneManager->createParticleSystem(name, templateName);

		//Variabeln einlesen, Emitter1_ ist Referenz-Emitter
		velocity_ = particleSystem_->getSpeedFactor();
		colour_ = particleSystem_->getEmitter(0)->getColour();
		rate_ = particleSystem_->getEmitter(0)->getEmissionRate();
		distance_ = particleSystem_->getEmitter(0)->getTimeToLive();

		//Anzahl der Emitter
		numberOfEmitters_ = particleSystem_->getNumEmitters();
		standardizeEmitters();
	}

	void ParticleInterface::standardizeEmitters(void)
	{
		//Abgleichen der anderen Emitter an die Variabeln
		for (int i=1; i<numberOfEmitters_; i++) {
			particleSystem_->getEmitter(i)->setColour( colour_ );
			particleSystem_->getEmitter(i)->setTimeToLive( distance_ );
			particleSystem_->getEmitter(i)->setEmissionRate( rate_ );
		}

	}

	void ParticleInterface::addToSceneNode( SceneNode* sceneNode )
	{
		sceneNode_ = sceneNode;
		sceneNode_->attachObject(particleSystem_);
	}

	void ParticleInterface::dettachFromSceneNode ( void )
	{
		sceneNode_->detachObject(particleSystem_);
		sceneNode_ = NULL;
	}

	Real ParticleInterface::getVelocity()
	{
		return velocity_;
	}

	void ParticleInterface::setVelocity(Real v)
	{
		velocity_ = v;
		//partikel anpassen
		particleSystem_->setSpeedFactor(v);
	}

	int ParticleInterface::getRate()
	{
		return rate_;
	}
	void ParticleInterface::setRate(int r)
	{
		rate_ = r;
		//partikel anpassen
		for (int i=0; i<numberOfEmitters_; i++) {
			particleSystem_->getEmitter(i)->setEmissionRate(rate_);
		}
	}

	Real ParticleInterface::getDistance()
	{
		return distance_;
	}

	void ParticleInterface::setDistance(Real d)
	{
		distance_ = d;
		//partikel anpassen
		for (int i=0; i<numberOfEmitters_; i++) {
			particleSystem_->getEmitter(i)->setTimeToLive(distance_);
		}
	}
	ColourValue ParticleInterface::getColour()
	{
		return colour_;
	}

	void ParticleInterface::setColour(ColourValue colour)
	{
		colour_ = colour;
		//partikel anpassen
		for (int i=0; i<numberOfEmitters_; i++) {
			particleSystem_->getEmitter(i)->setColour(colour_);
		}
	}

	ParticleEmitter* ParticleInterface::getEmitter( int emitterNr )
	{
		if (!(emitterNr<numberOfEmitters_)) return NULL;
		return particleSystem_->getEmitter(emitterNr);
	}

	void ParticleInterface::newEmitter ( void )
	{
		particleSystem_->addEmitter(particleSystem_->getEmitter(0)->getType());
		numberOfEmitters_=numberOfEmitters_+1;
		particleSystem_->getEmitter(0)->copyParametersTo( particleSystem_->getEmitter(numberOfEmitters_-1) );
	}

	void ParticleInterface::setPositionOfEmitter ( int emitterNr, Vector3 position )
	{
		particleSystem_->getEmitter(emitterNr)->setPosition(position);
	}

	Vector3 ParticleInterface::getPositionOfEmitter ( int emitterNr )
	{
		return particleSystem_->getEmitter(0)->getPosition();
	}

	void ParticleInterface::setDirection ( Vector3 direction )
	{
		for(int i=0; i<numberOfEmitters_; i++) {
			particleSystem_->getEmitter(i)->setDirection(direction);
		}
	}


	Vector3 ParticleInterface::getDirection ( void )
	{
		return particleSystem_->getEmitter(0)->getDirection();
	}

	void ParticleInterface::switchEnable(){
		bool enable=(!(particleSystem_->getEmitter(0)->getEnabled()));
		for(int i=0; i<numberOfEmitters_; i++) {
			particleSystem_->getEmitter(i)->setEnabled(enable);
		}
	}

}