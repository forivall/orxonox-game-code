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
 *      Fabian 'x3n' Landau
 *
 */

#include "OrxonoxStableHeaders.h"
#include "GSServer.h"

#include "core/CommandLine.h"
#include "core/Core.h"
#include "network/Server.h"

namespace orxonox
{
    SetCommandLineArgument(port, 55556).shortcut("p").information("0-65535");

    GSServer::GSServer()
        : GameState<GSGraphics>("server")
        , server_(0)
    {
    }

    GSServer::~GSServer()
    {
    }

    void GSServer::enter()
    {
        Core::setHasServer(true);

        this->server_ = new Server(CommandLine::getValue("port"));
        COUT(0) << "Loading scene in server mode" << std::endl;

        GSLevel::enter(this->getParent()->getViewport());

        server_->open();
    }

    void GSServer::leave()
    {
        GSLevel::leave();

        this->server_->close();
        delete this->server_;

        Core::setHasServer(false);
    }

    void GSServer::ticked(const Clock& time)
    {
        GSLevel::ticked(time);
        server_->tick(time.getDeltaTime());
        this->tickChild(time);
    }
}