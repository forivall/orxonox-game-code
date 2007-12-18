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
 *      Benjamin Knecht <beni_at_orxonox.net>, (C) 2007
 *   Co-authors:
 *      ...
 *
 */
 /**
    @file orxonox.cc
    @brief Orxonox class
  */

#include "GraphicsEngine.h"

#include <OgreRoot.h>
#include <OgreConfigFile.h>
#include <OgreTextureManager.h>
#include "core/Debug.h"


namespace orxonox {

  using namespace Ogre;

  GraphicsEngine::GraphicsEngine()
  {
    // set to standard values
    this->configPath_ = "";
    this->dataPath_ = "";
    scene_ = NULL;
  }


  GraphicsEngine::~GraphicsEngine()
  {
  }

  void GraphicsEngine::setup()
  {
    //TODO: Check if file exists (maybe not here)
/*#ifndef OGRE_STATIC_LIB
    root_ = new Root(configPath_ + "plugins.cfg", configPath_ + "ogre.cfg",
                     configPath_ + "Ogre.log");
#else
    root_ = new Root(NULL, configPath_ + "ogre.cfg", configPath_ + "Ogre.log");
#endif*/
    root_ = new Root();
  }

  /**
   * @return scene manager
   */
  SceneManager* GraphicsEngine::getSceneManager()
  {
    if(!scene_)
    {
      scene_ = root_->createSceneManager(ST_GENERIC, "Default SceneManager");
      COUT(3) << "Info: Created SceneMan: " << scene_ << std::endl;
    }
    return scene_;
  }

  bool GraphicsEngine::load()
  {
    loadRessourceLocations(this->dataPath_);
    if (!root_->restoreConfig() && !root_->showConfigDialog())
      return false;
    return true;
  }

  void GraphicsEngine::startRender()
  {
    root_->initialise(true, "OrxonoxV2");
    TextureManager::getSingleton().setDefaultNumMipmaps(5);
    ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
  }

  void GraphicsEngine::loadRessourceLocations(std::string dataPath)
  {
    //TODO: Specify layout of data file and maybe use xml-loader
    //TODO: Work with ressource groups (should be generated by a special loader)
    // Load resource paths from data file using configfile ressource type
    ConfigFile cf;
    cf.load(dataPath + "resources.cfg");

    // Go through all sections & settings in the file
    ConfigFile::SectionIterator seci = cf.getSectionIterator();

    String secName, typeName, archName;
    while (seci.hasMoreElements())
    {
      secName = seci.peekNextKey();
      ConfigFile::SettingsMultiMap *settings = seci.getNext();
      ConfigFile::SettingsMultiMap::iterator i;
      for (i = settings->begin(); i != settings->end(); ++i)
      {
        typeName = i->first; // for instance "FileSystem" or "Zip"
        archName = i->second; // name (and location) of archive

        ResourceGroupManager::getSingleton().addResourceLocation(
                                           String(dataPath + archName),
                                           typeName, secName);
      }
    }
  }


}
