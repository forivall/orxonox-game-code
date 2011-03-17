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
 *      Benjamin Knecht
 *
 */

/**
    @file
    @brief Declaration of the GUIManager class.
*/

#ifndef _GUIManager_H__
#define _GUIManager_H__

#include "OrxonoxPrereqs.h"
#include <OgrePrerequisites.h>
#include <CEGUIForwardRefs.h>
#include <CEGUIInputEvent.h>
#include <CEGUISystem.h>
#include "core/input/InputInterfaces.h"

// Forward declaration
namespace CEGUI { class DefaultLogger; }

// tolua_begin
namespace orxonox
{
    /**
    @class GUIManager
    @brief
        Provides a simple interface to CEGUI with tolua methods and console commands. It also acts as a key and mouse handler.

        The GUIManager is a singleton and can be called anywhere when access on the GUI is needed.
        Creation of the GUIManager is therefore not possible and the cunstructor is private.

        Since the GUI needs user input, the GUIManager implements the functions needed to act as a key and/or mouse handler.
        Those input events are then injected into CEGUI in Lua.
    */
    class _OrxonoxExport GUIManager
// tolua_end
        : public KeyHandler, public MouseHandler
// tolua_begin
    {
// tolua_end
    public:
        /**
        @enum State
            The current state of the GUIManager. There should maybe be more (or we can omit this totally).
        */
        enum State
        {
            Uninitialised,  //!< Initial state of the GUIManager
            Ready,          //!< State after initialisation if ready
            OnDisplay       //!< State if GUI is displayed
        };

        GUIManager();
        ~GUIManager();

        bool initialise(Ogre::RenderWindow* renderWindow);

        void update(const Clock& time);

        void showGUI(const std::string& name);
        void executeCode(const std::string& str);

        void setCamera(Ogre::Camera* camera);

        static GUIManager& getInstance()    { assert(singletonRef_s); return *singletonRef_s; } // tolua_export
        static GUIManager* getInstancePtr() { return singletonRef_s; }

    private:
        GUIManager(const GUIManager& instance);                 //!< private constructor (this is a singleton class)

        void loadLuaCode();

        // keyHandler functions
        void keyPressed (const KeyEvent& evt)
            { guiSystem_->injectKeyDown(evt.key); guiSystem_->injectChar(evt.text); }
        void keyReleased(const KeyEvent& evt)
            { guiSystem_->injectKeyUp(evt.key); }
        void keyHeld    (const KeyEvent& evt) { }

        // mouseHandler functions
        void mouseButtonPressed (MouseButtonCode::ByEnum id);
        void mouseButtonReleased(MouseButtonCode::ByEnum id);
        void mouseButtonHeld    (MouseButtonCode::ByEnum id) { }
        void mouseMoved         (IntVector2 abs, IntVector2 rel, IntVector2 clippingSize)
            { guiSystem_->injectMouseMove(rel.x, rel.y); }
        void mouseScrolled      (int abs, int rel)
            { guiSystem_->injectMouseWheelChange(rel);}

        void updateInput(float dt)  { }
        void updateKey  (float dt)  { }
        void updateMouse(float dt)  { }

        static CEGUI::MouseButton convertButton(MouseButtonCode::ByEnum button);

        Ogre::RenderWindow*         renderWindow_;      //!< Ogre's render window to give CEGUI access to it
        CEGUI::OgreCEGUIRenderer*   guiRenderer_;       //!< CEGUI's interface to the Ogre Engine
        CEGUI::ResourceProvider*    resourceProvider_;  //!< CEGUI's resource provider
        CEGUI::LuaScriptModule*     scriptModule_;      //!< CEGUI's script module to use Lua
        CEGUI::DefaultLogger*       ceguiLogger_;       //!< CEGUI's logger to be able to log CEGUI errors in our log
        CEGUI::System*              guiSystem_;         //!< CEGUI's main system
        lua_State*                  luaState_;          //!< Lua state, access point to the Lua engine

        State                       state_;             //!< reflects state of the GUIManager

        static GUIManager*          singletonRef_s;     //!< Singleton reference to GUIManager
    }; // tolua_export
} // tolua_export

#endif /* _GUIManager_H__ */