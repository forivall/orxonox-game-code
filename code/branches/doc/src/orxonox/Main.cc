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
 *      Reto Grieder
 *   Co-authors:
 *      ...
 *
 */

/**
@file
@brief
    The main function of Orxonox (but not the entry point of the program!)
*/

#include "OrxonoxPrereqs.h"

#include "core/CommandLineParser.h"
#include "core/Game.h"
#include "core/LuaState.h"
#include "ToluaBindOrxonox.h"
#include "ToluaBindNetwork.h"
#include "Main.h"

DeclareToluaInterface(Orxonox);
DeclareToluaInterface(Network);

namespace orxonox
{
    //! @cmdarg
    SetCommandLineSwitch(console).information("Start in console mode (text IO only)");
    //! @cmdarg
    SetCommandLineSwitch(server).information("Start in server mode");
    //! @cmdarg
    SetCommandLineSwitch(client).information("Start in client mode");
    //! @cmdarg
    SetCommandLineSwitch(dedicated).information("Start in dedicated server mode");
    //! @cmdarg
    SetCommandLineSwitch(standalone).information("Start in standalone mode");
    //! @cmdarg
    SetCommandLineSwitch(dedicatedClient).information("Start in dedicated client mode");

    //! @cmdarg
    SetCommandLineSwitch(generateDoc)
        .information("Generates a Doxygen file from things like SetConsoleCommand");
    //! @cmdarg
    SetCommandLineArgument(docFile, "Internal.dox")
        .information("Name of the doc file to be generated with --generateDoc");

    /**
    @brief
        Starting point of orxonox (however not the entry point of the program!)
    */
    int main(const std::string& strCmdLine)
    {
        Game* game = new Game(strCmdLine);

        if (CommandLineParser::getValue("generateDoc").getBool())
        {
            // Generate additional documentation written to ONE file
            std::string filename;
            CommandLineParser::getValue("docFile", &filename);
        }
        else
        {
            game->setStateHierarchy(
            "root"
            " graphics"
            "  mainMenu"
            "  standalone,server,client"
            "   level"
            " server,client"
            "  level"
            );

            game->requestState("root");

            // Some development hacks (not really, but in the future, this calls won't make sense anymore)
            if (CommandLineParser::getValue("standalone").getBool())
                Game::getInstance().requestStates("graphics, standalone, level");
            else if (CommandLineParser::getValue("server").getBool())
                Game::getInstance().requestStates("graphics, server, level");
            else if (CommandLineParser::getValue("client").getBool())
                Game::getInstance().requestStates("graphics, client, level");
            else if (CommandLineParser::getValue("dedicated").getBool())
                Game::getInstance().requestStates("server, level");
            else if (CommandLineParser::getValue("dedicatedClient").getBool())
                Game::getInstance().requestStates("client, level");
            else
            {
                if (!CommandLineParser::getValue("console").getBool())
                    Game::getInstance().requestStates("graphics, mainMenu");
            }

            game->run();
        }

        delete game;

        return 0;
    }
}
