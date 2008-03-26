/**
  @file GraphicsEngine.h
  @brief Graphics Engine
  @author Benjamin Knecht <beni_at_orxonox.net>
 */

#ifndef _GraphicsEngine_H__
#define _GraphicsEngine_H__

#include <string>

#include <OgrePrerequisites.h>
#include <OgreRoot.h>
#include <OgreSceneManager.h>

#include "OrxonoxPrereqs.h"
#include "core/OrxonoxClass.h"


namespace orxonox {

/**
   * graphics engine manager class
 */
  class _OrxonoxExport GraphicsEngine : public OrxonoxClass
  {
    public:
      GraphicsEngine();
      inline void setConfigPath(std::string path) { this->configPath_ = path; };
      // find a better way for this
      //inline Ogre::Root* getRoot() { return root_; };
      void setConfigValues();
      void setup();
      bool load(std::string path);
      void loadRessourceLocations(std::string path);
      Ogre::SceneManager* getSceneManager();
      void initialise();

      // several window properties
      Ogre::RenderWindow* getRenderWindow() { return this->renderWindow_; }
      size_t getWindowHandle();
      int getWindowWidth() const;
      int getWindowHeight() const;

      // Ogre Root access for Orxonox
      void frameStarted(Ogre::FrameEvent &evt)
      { if (root_) root_->_fireFrameStarted(evt); }
      void frameEnded  (Ogre::FrameEvent &evt)
      { if (root_) root_->_fireFrameEnded(evt);   }
      void renderOneFrame()
      { if (root_) root_->_updateAllRenderTargets(); }

      virtual ~GraphicsEngine();

    private:
      Ogre::Root*         root_;        //!< Ogre's root
      std::string         configPath_;  //!< path to config file
      std::string         dataPath_;    //!< path to data file
      Ogre::SceneManager* scene_;       //!< scene manager of the game
      Ogre::RenderWindow* renderWindow_;//!< the current render window
      //bool               bOverwritePath_; //!< overwrites path

  };

}

#endif /* _GraphicsEngine_H__ */
