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
*/


#include "OgreVector3.h"
#include "OgreStringConverter.h"
#include "OgreRoot.h"
#include "OgreSceneManager.h"
#include "OgreSceneNode.h"
#include "OgreCamera.h"
#include "OgreViewport.h"
#include "OgreRenderWindow.h"
#include "OgreTextureManager.h"
#include "OgreMaterialManager.h"
#include "OgreLogManager.h"
#include "OgreWindowEventUtilities.h"

//Use this define to signify OIS will be used as a DLL
//(so that dll import/export macros are in effect)
#define OIS_DYNAMIC_LIB
#include <OIS/OIS.h>

#include "ogre_control.h"
#include "orxonox_scene.h"
#include "orxonox_ship.h"
#include "inertial_node.h"

#include "weapon/bullet.h"
#include "weapon/bullet_manager.h"
#include "weapon/base_weapon.h"

#include "hud/hud_overlay.h"
#include "hud/test_overlay.h"

#include "run_manager.h"


namespace orxonox {
  using namespace Ogre;
  using namespace weapon;
  using namespace hud;

  /**
  * RunManager is the basic control object during the game.
  * 
  * The RunManger class is designed to actually "run" the main part of the
  * game. The Idea is, that you could derive from the RunManager in order
  * to distinguish between a first person shooter or a space craft shooter.
  * RunManager loads and initialises everything in the scene (like the ship,
  * the enemies in the scene, any scripts, the physics, window events,
  * environment, HUD, etc.).
  * It also captures any input from keyboard, mous, joystick (optional) or
  * Ogre (window events).
  */


  RunManager* RunManager::singletonPtr_s = NULL;


  /**
  * Contructor only needs the Root object.
  */
  RunManager::RunManager()
        : ogre_(NULL), window_(NULL), screenShotCounter_(0),
        timeUntilNextToggle_(0), mouseSensitivity_(0.003), inputManager_(0),
        mouse_(0), keyboard_(0), joystick_(0), statsOn_(true)
  {
  }

  /**
  * @param ogre_ The OgreControl object holding the render window and the Root
  */
  void RunManager::initialise(OgreControl *ogre)
    {
    ogre_ = ogre;
    window_ = ogre->getRenderWindow();

    // SETTING UP THE SCENE

    // create one new SceneManger
    sceneMgr_ = ogre_->getRoot()->createSceneManager(ST_GENERIC, "Orxonox Scene");

    // background scene (world objects, skybox, lights, etc.)
    backgroundScene_ = new OrxonoxScene(sceneMgr_);


    // BULLET LIST FOR THE TEST APPLICATION

    // create a bullet manager
    bulletManager_ = new BulletManager(sceneMgr_);


    // PLAYER SPACESHIP

    // Create a space ship object and its SceneNode.
    // It should also be considered, that the ship should provide another Node
    // for a camera to be attached (otherwise the spaceship in front of the
    // ship would be very static, never moving at all).

    // Construct a new spaceship and give it the node
    playerShip_ = new OrxonoxShip(sceneMgr_->getRootSceneNode()
      ->createChildSceneNode("ShipNode", Vector3(20, 20, 20)));


    // RESOURCE LOADING (using ResourceGroups if implemented)

    // load all resources and create the entities by calling the initialise()
    // methods for each object (no constructor initialisations!).
    backgroundScene_->initialise();
    playerShip_->initialise();


    // CAMERA AND VIEWPORT

    // create camera and viewport
    createCamera();
    createViewports();

    // create HUD
    hud_ = new hud::TestOverlay(window_);
    hud_->show();


    // Set default mipmap level (NB some APIs ignore this)
    TextureManager::getSingleton().setDefaultNumMipmaps(5);


    // HUMAN INTERFACE

    using namespace OIS;    

    LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
    ParamList pl;
    size_t windowHnd = 0;
    std::ostringstream windowHndStr;

    window_->getCustomAttribute("WINDOW", &windowHnd);
    windowHndStr << windowHnd;
    pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

    inputManager_ = InputManager::createInputSystem( pl );

    // Create all devices (We only catch joystick exceptions here,
    // as, most people have Key/Mouse)
    keyboard_ = static_cast<Keyboard*>(inputManager_
      ->createInputObject( OISKeyboard, false ));
    mouse_ = static_cast<Mouse*>(inputManager_
      ->createInputObject( OISMouse, false ));
    try {
      joystick_ = static_cast<JoyStick*>(inputManager_
        ->createInputObject( OISJoyStick, false ));
    }
    catch(...) {
      joystick_ = 0;
    }

    //Set initial mouse clipping size
    windowResized(window_);

    //showDebugOverlay(true);

    // REGISTER THIS OBJECT AS A WINDOW EVENT LISTENER IN OGRE
    // It will then receive events liek windowClosed, windowResized, etc.
    WindowEventUtilities::addWindowEventListener(window_, this);
  }

  /**
  * Standard destructor.
  * Removes this object as a window event listener and deletes all created
  * variables.
  */
  RunManager::~RunManager()
  {
    //Remove ourself as a Window listener
    WindowEventUtilities::removeWindowEventListener(window_, this);
    windowClosed(window_);

    if (backgroundScene_)
      delete backgroundScene_;
    if (playerShip_)
      delete playerShip_;
    if (bulletManager_)
      delete bulletManager_;
    if (hud_)
      delete hud_;
  }


  /**
  * Method to compute anything between 2 frames.
  * 
  * Everything that needs to be computed during the games happens right here.
  * The only exception are the listeners (which should only set variables,
  * not actually do something).
  * 
  * @param time Absolute play time
  * @param deltaTime Time passed since last frame
  * @return Return false to end rendering
  */
  bool RunManager::tick(unsigned long time, float deltaTime)
  {
    // synchronize with internal class timer
    totalTime_ = time;

    // Call tick() for every object
    // This could be done by registering (needs a factory..)
    backgroundScene_->tick(time, deltaTime);
    playerShip_->tick(time, deltaTime);


    // Update the HUD
    hud_->setDebugText(" | Speed = "
      + StringConverter::toString(playerShip_->getSpeed())
      + " | Left Ammo = "
      + StringConverter::toString(playerShip_
      ->getMainWeapon()->getAmmoState())
      + " | Ammo stock = "
      + StringConverter::toString(playerShip_->getAmmoStock()));

    hud_->tick(time, deltaTime);

    // update the bullet positions
    bulletManager_->tick(time, deltaTime);


    // HUMAN INTERFACE

    using namespace OIS;

    if(window_->isClosed())	return false;

    //Need to capture/update each device
    keyboard_->capture();
    mouse_->capture();
    if( joystick_ ) joystick_->capture();

    bool buffJ = (joystick_) ? joystick_->buffered() : true;

    //Check if one of the devices is not buffered
    if( !mouse_->buffered() || !keyboard_->buffered() || !buffJ )
    {
      // one of the input modes is immediate, so setup what
      // is needed for immediate movement
      if (timeUntilNextToggle_ >= 0)
        timeUntilNextToggle_ -= deltaTime;
    }

    // handle HID devices
    if( processUnbufferedKeyInput() == false )
        return false;
    if( processUnbufferedMouseInput() == false )
        return false;

    // keep rendering
    return true;
  }


  SceneManager& RunManager::getSceneManager()
  {
    return *sceneMgr_;
  }

  SceneManager* RunManager::getSceneManagerPtr()
  {
    return sceneMgr_;
  }

  BulletManager* RunManager::getBulletManagerPtr()
  {
    return bulletManager_;
  }

  int RunManager::getAmmunitionID(const Ogre::String &ammoName)
  {
    Ogre::String ammoTypes[] = { "Energy Cell", "Barrel", "Lead Shot" };
    int ammoTypesLength = 3;

    for (int i = 0; i < ammoTypesLength; i++)
    {
      if (ammoTypes[i] == ammoName)
        return i;
    }
    return -1;
  }

  int RunManager::getNumberOfAmmos()
  {
    return 3;
  }


  /**
  * Adjust mouse clipping area.
  * This method is called by Ogre without regards of tick()!
  * Avoid doing too much in this call.
  * @param rw render window
  */
  void RunManager::windowResized(RenderWindow* rw)
  {
    unsigned int width, height, depth;
    int left, top;
    rw->getMetrics(width, height, depth, left, top);

    const OIS::MouseState &ms = mouse_->getMouseState();
    ms.width = width;
    ms.height = height;
  }


  /**
  * Unattach OIS before window shutdown (very important under Linux).
  * Again, avoid computing a lot in this function.
  * @param rw Render Window
  */
  void RunManager::windowClosed(RenderWindow* rw)
  {
    //Only close for window that created OIS (the main window in these demos)
    if( rw == window_ )
    {
      if( inputManager_ )
      {
        inputManager_->destroyInputObject( mouse_ );
        inputManager_->destroyInputObject( keyboard_ );
        inputManager_->destroyInputObject( joystick_ );

        OIS::InputManager::destroyInputSystem(inputManager_);
        inputManager_ = 0;
      }
    }
  }

  /**
  * Processes the Keyboard input.
  * TODO: Use listeners to improve performance.
  * A lookup table should be implemented to bind any key to a specific action.
  * @return Return true to keep rendering
  */
  bool RunManager::processUnbufferedKeyInput()
  {
    using namespace OIS;

    if(keyboard_->isKeyDown(KC_A) || keyboard_->isKeyDown(KC_LEFT))
      playerShip_->setSideThrust(1);
    else if(keyboard_->isKeyDown(KC_D) || keyboard_->isKeyDown(KC_RIGHT))
      playerShip_->setSideThrust(-1);
    else
      playerShip_->setSideThrust(0);

    if(keyboard_->isKeyDown(KC_UP) || keyboard_->isKeyDown(KC_W) )
      playerShip_->setMainThrust(1);
    else if(keyboard_->isKeyDown(KC_DOWN) || keyboard_->isKeyDown(KC_S) )
      playerShip_->setMainThrust(-1);
    else
      playerShip_->setMainThrust(0);

    if (keyboard_->isKeyDown(KC_C))
      playerShip_->setYThrust(1);
    else if (keyboard_->isKeyDown(KC_SPACE))
      playerShip_->setYThrust(-1);
    else
      playerShip_->setYThrust(0);

    if (keyboard_->isKeyDown(KC_G))
      playerShip_->getMainWeapon()->addAction(BaseWeapon::RELOAD);

    if( keyboard_->isKeyDown(KC_ESCAPE) || keyboard_->isKeyDown(KC_Q) )
      return false;

    if( keyboard_->isKeyDown(KC_F) && timeUntilNextToggle_ <= 0 )
    {
      statsOn_ = !statsOn_;
      timeUntilNextToggle_ = 1;
    }

    if(keyboard_->isKeyDown(KC_SYSRQ) && timeUntilNextToggle_ <= 0)
    {
      std::ostringstream ss;
      ss << "screenshot_" << ++screenShotCounter_ << ".png";
      window_->writeContentsToFile(ss.str());
      timeUntilNextToggle_ = 0.5;
    }

    // Return true to continue rendering
    return true;
  }


  /**
  * Processes the Mouse input.
  * TODO: Use listeners to improve performance.
  * A lookup table should be implemented to bind ANY button or movement
  * to a specific action.
  * @return Return true to keep rendering
  */
  bool RunManager::processUnbufferedMouseInput()
  {
    using namespace OIS;

    const MouseState &ms = mouse_->getMouseState();

    if (ms.buttonDown(MB_Left))
      playerShip_->getMainWeapon()->primaryFireRequest();

    if (ms.buttonDown(MB_Right))
      playerShip_->getMainWeapon()->secondaryFireRequest();

    playerShip_->turnUpAndDown(Radian(ms.Y.rel * mouseSensitivity_));
    playerShip_->turnLeftAndRight(Radian(ms.X.rel * mouseSensitivity_));

    // keep rendering
    return true;
  }

  /**
  * Show an overlay desired.
  * @param show Whether or not to show the debug overlay
  */
  void RunManager::showDebugOverlay(bool show)
  {
    if (hud_)
    {
      if (show)
        hud_->show();
      else
        hud_->hide();
    }
  }


  /**
  * Simple camera creator.
  * playerShip_Node->attachObject(camera_) should not be here! This is what
  * the camera manager is for. Right now, this method should do just fine,
  * setting the cam behind the ship.
  */
  void RunManager::createCamera()
  {
    camera_ = sceneMgr_->createCamera("PlayerCam");
    playerShip_->getRootNode()->getSceneNode()->attachObject(camera_);
    camera_->setNearClipDistance(5);
    camera_->setPosition(Vector3(0,10,500));
    camera_->lookAt(Vector3(0,0,0));
  }

  /**
  * Simple viewport creator.
  * For now the viewport uses the entire render window and is based on the one
  * camera created so far.
  */
  void RunManager::createViewports()
  {
    // Create one viewport, entire window
    Viewport* vp = window_->addViewport(camera_);
    vp->setBackgroundColour(ColourValue(0,0,0));

    // Alter the camera aspect ratio to match the viewport
    camera_->setAspectRatio(
      Real(vp->getActualWidth()) / Real(vp->getActualHeight()));
  }


  RunManager* RunManager::createSingleton()
  {
    if (singletonPtr_s)
      return NULL;
    singletonPtr_s = new RunManager();
    return singletonPtr_s;
  }

  void RunManager::destroySingleton()
  {
    if (singletonPtr_s)
      delete singletonPtr_s;
  }


  RunManager& RunManager::getSingleton()
  {
    return *singletonPtr_s;
  }

  RunManager* RunManager::getSingletonPtr()
  {
    return singletonPtr_s;
  }

}