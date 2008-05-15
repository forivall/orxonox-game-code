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
*      Yuning Chai
*   Co-authors:
*      ...
*
*/

#ifndef _FACTORIES_H__
#define _FACTORIES_H__


#include <OgreOverlayElement.h>
#include <OgrePrerequisites.h>
#include <OgreOverlayElementFactory.h>

#include "BarOverlayElement.h"


namespace orxonox{

  class _OrxonoxExport BarOverlayElementFactory : public Ogre::OverlayElementFactory{
    public:
    Ogre::OverlayElement* createOverlayElement(const Ogre::String& instanceName){
      return new BarOverlayElement(instanceName);
    }
    const Ogre::String& getTypeName(void) const{
      static Ogre::String name = "Bar";
      return name;
    }
  };


  class _OrxonoxExport SmartBarOverlayElementFactory : public Ogre::OverlayElementFactory{
    public:
    Ogre::OverlayElement* createOverlayElement(const Ogre::String& instanceName){
      return new SmartBarOverlayElement(instanceName);
    }
    const Ogre::String& getTypeName(void) const{
      static Ogre::String name = "SmartBar";
      return name;
    }
  };

  class _OrxonoxExport RadarOverlayElementFactory : public Ogre::OverlayElementFactory{
    public:
      Ogre::OverlayElement* createOverlayElement(const Ogre::String& instanceName){
	return new RadarOverlayElement(instanceName);
      }
      const Ogre::String& getTypeName(void) const{
	static Ogre::String name = "Radar";
	return name;
      }
  };
}




#endif
