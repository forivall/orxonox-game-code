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
 *      Benjamin Knecht <beni_at_orxonox.net>, (C) 2007
 *   Co-authors:
 *      Reto Grieder
 *
 */

/**
  @file
  @brief Declaration of GraphicsEngine Singleton.
  @author Benjamin Knecht <beni_at_orxonox.net>
 */

#ifndef _GraphicsEngine_H__
#define _GraphicsEngine_H__

#include "OrxonoxPrereqs.h"

#include <string>

#include <OgrePrerequisites.h>
#include <OgreLog.h>

#include "core/OrxonoxClass.h"


namespace orxonox {

  /**
    @brief Graphics engine manager class
  */
  class _OrxonoxExport GraphicsEngine : public OrxonoxClass, public Ogre::LogListener
  {
    friend class ClassIdentifier<GraphicsEngine>;
    public:
      void setConfigPath(std::string path) { this->configPath_ = path; };
      void setConfigValues();
      void setup();
      bool load(std::string path);
      void loadRessourceLocations(std::string path);
      Ogre::SceneManager* getSceneManager();
      void initialise();
      void destroy();

      // several window properties
      Ogre::RenderWindow* getRenderWindow() { return this->renderWindow_; }
      size_t getWindowHandle();
      int getWindowWidth() const;
      int getWindowHeight() const;

      static GraphicsEngine& getSingleton();
      static GraphicsEngine* getSingletonPtr() { return &getSingleton(); }

    private:
      // don't mess with singletons
      GraphicsEngine();
      ~GraphicsEngine();
      GraphicsEngine(GraphicsEngine&) { }

      //! Method called by the LogListener from Ogre
      void messageLogged(const std::string&, Ogre::LogMessageLevel,
                         bool, const std::string&);

      Ogre::Root*         root_;        //!< Ogre's root
      Ogre::SceneManager* scene_;       //!< scene manager of the game
      Ogre::RenderWindow* renderWindow_;//!< the current render window
      //bool               bOverwritePath_; //!< overwrites path
      std::string         configPath_;  //!< path to config file
      std::string         dataPath_;    //!< path to data file
      std::string         ogreLogfile_; //!< log file name for Ogre log messages
      int ogreLogLevelTrivial_;         //!< Corresponding Orxonx debug level for LL_TRIVIAL
      int ogreLogLevelNormal_;          //!< Corresponding Orxonx debug level for LL_NORMAL
      int ogreLogLevelCritical_;        //!< Corresponding Orxonx debug level for LL_CRITICAL

  };

}

#endif /* _GraphicsEngine_H__ */
