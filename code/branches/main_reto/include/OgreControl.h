/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
 *
 *
 *   License notice:
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 *   Author:
 *      Reto Grieder
 *   Co-authors:
 *      ...
 *
 */


#ifndef __OgreControl__
#define __OgreControl__

#include "Ogre.h"
#include "OgreConfigFile.h"


#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
#include <CoreFoundation/CoreFoundation.h>
#endif

using namespace Ogre;

class OgreControl
{
public:
	OgreControl();

	virtual ~OgreControl();

	/** Sets up the application - returns false if the user chooses to abandon configuration. */
	virtual bool initialise(void);

	virtual Root* getRoot(void);

	virtual RenderWindow* getRenderWindow(void);

	virtual Ogre::String getResourcePath(void);

protected:
	Root         *mRoot;
	RenderWindow *mWindow;
	Ogre::String  mResourcePath;

	/** Configures the application - returns false if the user chooses to abandon configuration. */
	virtual bool configure(void);

	/// Method which will define the source of resources (other than current folder)
	virtual void setupResources(void);

};

#endif