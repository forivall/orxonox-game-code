/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
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
 *      Fabian 'x3n' Landau
 *   Co-authors:
 *      ...
 *
 */

#include "Loader.h"
#include "Level.h"
#include "BaseObject.h"
#include "Identifier.h"
#include "Iterator.h"
#include "Debug.h"
#include "CoreIncludes.h"
#include "../script/Script.h"

#include "util/tinyxml/ticpp.h"

namespace orxonox
{
    std::vector<std::pair<const Level*, ClassTreeMask> > Loader::levels_s;
    ClassTreeMask Loader::currentMask_s;

    bool Loader::open(const Level* level, const ClassTreeMask& mask)
    {
        Loader::add(level, mask);
        return Loader::load(level, mask);
    }

    void Loader::close()
    {
        Loader::unload();
        Loader::levels_s.clear();
    }

    void Loader::close(const Level* level)
    {
        Loader::unload(level);
        Loader::remove(level);
    }

    void Loader::add(const Level* level, const ClassTreeMask& mask)
    {
        Loader::levels_s.insert(Loader::levels_s.end(), std::pair<const Level*, ClassTreeMask>(level, mask));
    }

    void Loader::remove(const Level* level)
    {
        for (std::vector<std::pair<const Level*, ClassTreeMask> >::iterator it = Loader::levels_s.begin(); it != Loader::levels_s.end(); ++it)
        {
            if ((*it).first == level)
            {
                Loader::levels_s.erase(it);
                break;
            }
        }
    }

    bool Loader::load(const ClassTreeMask& mask)
    {
        bool success = true;
        for (std::vector<std::pair<const Level*, ClassTreeMask> >::iterator it = Loader::levels_s.begin(); it != Loader::levels_s.end(); ++it)
            if (!Loader::load((*it).first, (*it).second * mask))
                success = false;

        return success;
    }

    void Loader::unload(const ClassTreeMask& mask)
    {
        for (Iterator<BaseObject> it = ObjectList<BaseObject>::begin(); it; )
        {
            if (mask.isIncluded(it->getIdentifier()))
                delete (*(it++));
            else
                ++it;
        }
    }

    bool Loader::reload(const ClassTreeMask& mask)
    {
        Loader::unload(mask);
        return Loader::load(mask);
    }

    bool Loader::load(const Level* level, const ClassTreeMask& mask)
    {
        Loader::currentMask_s = level->getMask() * mask;

        // let Lua work this out:
        //Script* lua;
        Script::loadFile(level->getFile(), true);
        Script::init(Script::getLuaState());
        Script::run();

        try
        {
            COUT(0) << "Start loading " << level->getFile() << "..." << std::endl;
            COUT(3) << "Mask: " << Loader::currentMask_s << std::endl;

            //ticpp::Document xmlfile(level->getFile());
            //xmlfile.LoadFile();
            //ticpp::Element myelement(*Script::getFileString());
            ticpp::Document xmlfile;
            //xmlfile.ToDocument();
            xmlfile.Parse(*Script::getFileString(), true);

            for ( ticpp::Iterator<ticpp::Element> child = xmlfile.FirstChildElement(false); child != child.end(); child++ )
            {
                Identifier* identifier = ID(child->Value());
                if (identifier)
                {
                    if (Loader::currentMask_s.isIncluded(identifier))
                    {
                        COUT(4) << "  fabricating " << child->Value() << "..." << std::endl;
                        BaseObject* newObject = identifier->fabricate();
                        newObject->setLoaderIndentation("    ");
                        newObject->setLevel(level);
                        newObject->XMLPort(*child, true);
                        COUT(5) << "  ...fabricated " << child->Value() << " (objectname " << newObject->getName() << ")." << std::endl;
                    }
                }
                else
                {
                    COUT(2) << "  Warning: '" << child->Value() << "' is not a valid classname." << std::endl;
                }
            }

            COUT(0) << "Finished loading " << level->getFile() << "." << std::endl;

            return true;
        }
        catch(ticpp::Exception& ex)
        {
            COUT(1) << std::endl;
            COUT(1) << "An error occurred in Loader.cc while loading " << level->getFile() << ":" << std::endl;
            COUT(1) << ex.what() << std::endl;
            COUT(1) << "Loading aborted." << std::endl;
            return false;
        }
    }

    void Loader::unload(const Level* level, const ClassTreeMask& mask)
    {
        for (Iterator<BaseObject> it = ObjectList<BaseObject>::begin(); it; )
        {
            if ((it->getLevel() == level) && mask.isIncluded(it->getIdentifier()))
                delete (*(it++));
            else
                ++it;
        }
    }

    bool Loader::reload(const Level* level, const ClassTreeMask& mask)
    {
        Loader::unload(level, mask);
        return Loader::load(level, mask);
    }
}
