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

#ifndef _GSLevel_H__
#define _GSLevel_H__

#include "OrxonoxPrereqs.h"

#include <string>
#include "core/OrxonoxClass.h"
#include "core/GameState.h"

namespace orxonox
{
    class _OrxonoxExport GSLevel : public GameState, public OrxonoxClass
    {
    public:
        GSLevel(const GameStateInfo& info);
        ~GSLevel();
        void setConfigValues();

        void activate();
        void deactivate();
        void update(const Clock& time);

        static void showIngameGUI(bool show);

        static XMLFile* startFile_s;

    protected:
        void loadLevel();
        void unloadLevel();

        InputState*              gameInputState_;          //!< input state for normal ingame playing
        InputState*              guiMouseOnlyInputState_;  //!< input state if we only need the mouse to use the GUI
        InputState*              guiKeysOnlyInputState_;   //!< input state if we only need the keys to use the GUI
    };
}

#endif /* _GSLevel_H__ */
