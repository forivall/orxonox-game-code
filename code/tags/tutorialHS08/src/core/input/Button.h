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

/**
@file
@brief
    Different definitions of input processing.
*/

#ifndef _Button_H__
#define _Button_H__

#include "core/CorePrereqs.h"

#include <string>
#include <vector>

namespace orxonox
{
    class _CoreExport Button
    {
    public:
        Button() { nCommands_[0]=0; nCommands_[1]=0; nCommands_[2]=0; clear(); }
        virtual ~Button() { clear(); }
        virtual void clear();
        virtual bool addParamCommand(ParamCommand* command) { return false; }
        void parse(std::vector<BufferedParamCommand*>& paramCommandBuffer);
        bool execute(KeybindMode::Enum mode, float abs = 1.0f, float rel = 1.0f);

        //! The configured string value
        std::string bindingString_;
        //! Name of the trigger as strings
        std::string name_;
        //! Basic commands for OnPress, OnHold and OnRelease
        BaseCommand** commands_[3];
        //! Number of basic commands
        unsigned int nCommands_[3];
        //! Says how much it takes for an analog axis to trigger a button
        //! Note: This variable is here to have only one parse() function.
        float buttonThreshold_;

    private:
        void parseError(std::string message, bool serious);
    };
}

#endif /* _Button_H__ */