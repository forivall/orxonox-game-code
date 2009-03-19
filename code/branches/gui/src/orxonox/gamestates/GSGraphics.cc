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
 *      Reto Grieder
 *   Co-authors:
 *      ...
 *
 */

#include "OrxonoxStableHeaders.h"
#include "GSGraphics.h"

#include <boost/filesystem.hpp>
#include <OgreRenderWindow.h>

#include "util/Debug.h"
#include "core/ConfigValueIncludes.h"
#include "core/CoreIncludes.h"
#include "core/Core.h"
#include "core/input/InputManager.h"
#include "core/input/KeyBinder.h"
#include "core/input/ExtendedInputState.h"
#include "core/Loader.h"
#include "core/XMLFile.h"
#include "overlays/console/InGameConsole.h"
#include "gui/GUIManager.h"

// for compatibility
#include "GraphicsManager.h"

namespace orxonox
{
    GSGraphics::GSGraphics()
        : GameState<GSRoot>("graphics")
        , inputManager_(0)
        , console_(0)
        , guiManager_(0)
        , graphicsManager_(0)
        , masterKeyBinder_(0)
        , debugOverlay_(0)
    {
        RegisterRootObject(GSGraphics);
        setConfigValues();
    }

    GSGraphics::~GSGraphics()
    {
    }

    void GSGraphics::setConfigValues()
    {
    }

    void GSGraphics::enter()
    {
        Core::setShowsGraphics(true);

        // initialise graphics manager. Doesn't load the render window yet!
        this->graphicsManager_ = new GraphicsManager();
        this->graphicsManager_->initialise();

        // load debug overlay
        COUT(3) << "Loading Debug Overlay..." << std::endl;
        this->debugOverlay_ = new XMLFile((Core::getMediaPath() / "overlay" / "debug.oxo").string());
        Loader::open(debugOverlay_);

        // Calls the InputManager which sets up the input devices.
        // The render window width and height are used to set up the mouse movement.
        inputManager_ = new InputManager();
        size_t windowHnd = 0;
        Ogre::RenderWindow* renderWindow = GraphicsManager::getInstance().getRenderWindow();
        renderWindow->getCustomAttribute("WINDOW", &windowHnd);
        inputManager_->initialise(windowHnd, renderWindow->getWidth(), renderWindow->getHeight(), true);
        // Configure master input state with a KeyBinder
        masterKeyBinder_ = new KeyBinder();
        masterKeyBinder_->loadBindings("masterKeybindings.ini");
        inputManager_->getMasterInputState()->addKeyHandler(masterKeyBinder_);

        // Load the InGameConsole
        console_ = new InGameConsole();
        console_->initialise(renderWindow->getWidth(), renderWindow->getHeight());

        // load the CEGUI interface
        guiManager_ = new GUIManager();
        guiManager_->initialise(renderWindow);
    }

    void GSGraphics::leave()
    {
        delete this->guiManager_;

        delete this->console_;

        //inputManager_->getMasterInputState()->removeKeyHandler(this->masterKeyBinder_);
        delete this->masterKeyBinder_;
        delete this->inputManager_;

        Loader::unload(this->debugOverlay_);
        delete this->debugOverlay_;

        delete graphicsManager_;
    }

    /**
    @note
        A note about the Ogre::FrameListener: Even though we don't use them,
        they still get called. However, the delta times are not correct (except
        for timeSinceLastFrame, which is the most important). A little research
        as shown that there is probably only one FrameListener that doesn't even
        need the time. So we shouldn't run into problems.
    */
    void GSGraphics::ticked(const Clock& time)
    {
        uint64_t timeBeforeTick = time.getRealMicroseconds();

        this->inputManager_->update(time);        // tick console
        this->console_->update(time);
        this->tickChild(time);

        uint64_t timeAfterTick = time.getRealMicroseconds();

        // Also add our tick time to the list in GSRoot
        this->getParent()->addTickTime(timeAfterTick - timeBeforeTick);

        // Update statistics overlay. Note that the values only change periodically in GSRoot.
        GraphicsManager::getInstance().setAverageFramesPerSecond(this->getParent()->getAvgFPS());
        GraphicsManager::getInstance().setAverageTickTime(this->getParent()->getAvgTickTime());

        this->graphicsManager_->update(time);
    }

    /**
    @brief
        Window has resized.
    @param rw
        The render window it occured in
    @note
        GraphicsManager has a render window stored itself. This is the same
        as rw. But we have to be careful when using multiple render windows!
    */
    void GSGraphics::windowResized(unsigned int newWidth, unsigned int newHeight)
    {
        // OIS needs this under linux even if we only use relative input measurement.
        if (this->inputManager_)
            this->inputManager_->setWindowExtents(newWidth, newHeight);
    }

    /**
    @brief
        Window focus has changed.
    @param rw
        The render window it occured in
    */
    void GSGraphics::windowFocusChanged()
    {
        // instruct InputManager to clear the buffers (core library so we cannot use the interface)
        if (this->inputManager_)
            this->inputManager_->clearBuffers();
    }

}
