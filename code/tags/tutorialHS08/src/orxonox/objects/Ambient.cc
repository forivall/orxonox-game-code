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
 *      ...
 *   Co-authors:
 *      ...
 *
 */

#include "OrxonoxStableHeaders.h"
#include "Ambient.h"

#include <vector>
#include <string>

#include <OgreSceneManager.h>

#include "tinyxml/tinyxml.h"
#include "util/SubString.h"
#include "util/Math.h"
#include "util/Debug.h"
#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "core/ConsoleCommand.h"
#include "Settings.h"
#include "GraphicsEngine.h"

namespace orxonox
{
    SetConsoleCommandAlias(Ambient, setAmbientLightTest, "setAmbientLight", false).defaultValues(ColourValue(1, 1, 1, 1)).accessLevel(AccessLevel::Offline);

    CreateFactory(Ambient);

    Ambient* Ambient::instance_s;

    Ambient::Ambient()
    {
        RegisterObject(Ambient);
        Ambient::instance_s = this;
        registerAllVariables();
    }

    Ambient::~Ambient()
    {
    }

    bool Ambient::create()
    {
        if (Settings::showsGraphics())
            GraphicsEngine::getInstance().getLevelSceneManager()->setAmbientLight(ambientLight_);
        return Synchronisable::create();
    }

    void Ambient::registerAllVariables()
    {
        registerVar(&ambientLight_, sizeof(ColourValue), network::DATA);
    }

    void Ambient::setAmbientLight(const ColourValue& colour)
    {
        if (Settings::showsGraphics())
            GraphicsEngine::getInstance().getLevelSceneManager()->setAmbientLight(colour);
        ambientLight_ = colour;
    }

    /**
    @brief
        XML loading and saving.
    @param
        xmlelement The XML-element
    @param
        loading Loading (true) or saving (false)
    @return
        The XML-element
    */
    void Ambient::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(Ambient, XMLPort, xmlelement, mode);

        XMLPortParam(Ambient, "colourvalue", setAmbientLight, getAmbienetLight, xmlelement, mode);
        create();
    }
}