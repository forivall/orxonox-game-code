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
#include "GSGUI.h"

#include "GraphicsEngine.h"
#include "core/input/InputManager.h"
#include "core/input/SimpleInputState.h"
#include "gui/GUIManager.h"

namespace orxonox
{
    GSGUI::GSGUI()
        : GameState("gui")
    {
    }

    GSGUI::~GSGUI()
    {
    }

    void GSGUI::enter()
    {
        // show main menu
        GUIManager::getInstance().showGUI("MainMenu", 0);
        GraphicsEngine::getInstance().getViewport()->setCamera(GUIManager::getInstance().getCamera());
    }

    void GSGUI::leave()
    {
        GUIManager::getInstance().hideGUI();
    }

    void GSGUI::ticked(float dt)
    {
        // tick CEGUI
        GUIManager::getInstance().tick(dt);

        this->tickChild(dt);
    }
}
