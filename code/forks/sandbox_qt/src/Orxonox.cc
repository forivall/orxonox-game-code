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
    Entry point of the program.
*/

#include "OrxonoxConfig.h"

#include "util/Debug.h"
#include "util/Exception.h"
#include "orxonox/Main.h"

/*
@brief
    Main method. Game starts here (except for static initialisations).
*/
int main(int argc, char** argv)
{
    try
    {
        return orxonox::main(argc, argv);
    }
    catch (...)
    {
        COUT(0) << "Orxonox failed to initialise: " << orxonox::Exception::handleMessage() << std::endl;
        COUT(0) << "Terminating program." << std::endl;
        return 1;
    }
}