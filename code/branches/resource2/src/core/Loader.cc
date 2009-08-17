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
 *      Fabian 'x3n' Landau
 *   Co-authors:
 *      ...
 *
 */

#include "Loader.h"

#include <sstream>
#include <tinyxml/ticpp.h>
#include <boost/scoped_ptr.hpp>

#include "util/Debug.h"
#include "util/Exception.h"
#include "util/StringUtils.h"
#include "BaseObject.h"
#include "Iterator.h"
#include "ObjectList.h"
#include "LuaState.h"
#include "Namespace.h"
#include "Resource.h"
#include "XMLFile.h"

namespace orxonox
{
    std::vector<std::pair<const XMLFile*, ClassTreeMask> > Loader::files_s;
    ClassTreeMask Loader::currentMask_s;

    bool Loader::open(const XMLFile* file, const ClassTreeMask& mask)
    {
        Loader::add(file, mask);
        return Loader::load(file, mask);
    }

    void Loader::close()
    {
        Loader::unload();
        Loader::files_s.clear();
    }

    void Loader::close(const XMLFile* file)
    {
        Loader::unload(file);
        Loader::remove(file);
    }

    void Loader::add(const XMLFile* file, const ClassTreeMask& mask)
    {
        if (!file)
            return;
        Loader::files_s.insert(Loader::files_s.end(), std::pair<const XMLFile*, ClassTreeMask>(file, mask));
    }

    void Loader::remove(const XMLFile* file)
    {
        if (!file)
            return;
        for (std::vector<std::pair<const XMLFile*, ClassTreeMask> >::iterator it = Loader::files_s.begin(); it != Loader::files_s.end(); ++it)
        {
            if ((*it).first == file)
            {
                Loader::files_s.erase(it);
                break;
            }
        }
    }

    bool Loader::load(const ClassTreeMask& mask)
    {
        bool success = true;
        for (std::vector<std::pair<const XMLFile*, ClassTreeMask> >::iterator it = Loader::files_s.begin(); it != Loader::files_s.end(); ++it)
            if (!Loader::load((*it).first, (*it).second * mask))
                success = false;

        return success;
    }

    void Loader::unload(const ClassTreeMask& mask)
    {
        for (ObjectList<BaseObject>::iterator it = ObjectList<BaseObject>::begin(); it != ObjectList<BaseObject>::end(); )
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

    bool Loader::load(const XMLFile* file, const ClassTreeMask& mask)
    {
        if (!file)
            return false;

        Loader::currentMask_s = file->getMask() * mask;

        // Use the LuaState to replace the XML tags (calls our function)
        scoped_ptr<LuaState> luaState(new LuaState());
        luaState->setIncludeParser(&Loader::replaceLuaTags);
        luaState->includeFile(file->getFilename(), file->getResourceGroup(), false);
        //luaState->doString(luaInput);

        try
        {
            COUT(0) << "Start loading " << file->getFilename() << "..." << std::endl;
            COUT(3) << "Mask: " << Loader::currentMask_s << std::endl;

            ticpp::Document xmlfile(file->getFilename());
            xmlfile.Parse(luaState->getOutput().str(), true);

            ticpp::Element rootElement;
            rootElement.SetAttribute("name", "root");
            rootElement.SetAttribute("bAutogenerated", true);

            for (ticpp::Iterator<ticpp::Element> child = xmlfile.FirstChildElement(false); child != child.end(); child++)
                rootElement.InsertEndChild(*child);

            COUT(4) << "  creating root-namespace..." << std::endl;
            Namespace* rootNamespace = new Namespace(0);
            rootNamespace->setLoaderIndentation("    ");
            rootNamespace->setFile(file);
            rootNamespace->setNamespace(rootNamespace);
            rootNamespace->setRoot(true);
            rootNamespace->XMLPort(rootElement, XMLPort::LoadObject);

            COUT(0) << "Finished loading " << file->getFilename() << "." << std::endl;

            COUT(4) << "Namespace-tree:" << std::endl << rootNamespace->toString("  ") << std::endl;

            return true;
        }
        catch (ticpp::Exception& ex)
        {
            COUT(1) << std::endl;
            COUT(1) << "An XML-error occurred in Loader.cc while loading " << file->getFilename() << ":" << std::endl;
            COUT(1) << ex.what() << std::endl;
            COUT(1) << "Loading aborted." << std::endl;
            return false;
        }
        catch (Exception& ex)
        {
            COUT(1) << std::endl;
            COUT(1) << "A loading-error occurred in Loader.cc while loading " << file->getFilename() << ":" << std::endl;
            COUT(1) << ex.what() << std::endl;
            COUT(1) << "Loading aborted." << std::endl;
            return false;
        }
        catch (std::exception& ex)
        {
            COUT(1) << std::endl;
            COUT(1) << "An error occurred in Loader.cc while loading " << file->getFilename() << ":" << std::endl;
            COUT(1) << ex.what() << std::endl;
            COUT(1) << "Loading aborted." << std::endl;
            return false;
        }
        catch (...)
        {
            COUT(1) << std::endl;
            COUT(1) << "An unknown error occurred in Loader.cc while loading " << file->getFilename() << ":" << std::endl;
            COUT(1) << "Loading aborted." << std::endl;
            return false;
        }
    }

    void Loader::unload(const XMLFile* file, const ClassTreeMask& mask)
    {
        if (!file)
            return;
        for (ObjectList<BaseObject>::iterator it = ObjectList<BaseObject>::begin(); it; )
        {
            if ((it->getFile() == file) && mask.isIncluded(it->getIdentifier()))
                delete (*(it++));
            else
                ++it;
        }
    }

    bool Loader::reload(const XMLFile* file, const ClassTreeMask& mask)
    {
        Loader::unload(file, mask);
        return Loader::load(file, mask);
    }

    std::string Loader::replaceLuaTags(const std::string& text)
    {
        // chreate map with all Lua tags
        std::map<size_t, bool> luaTags;
        {
            size_t pos = 0;
            while ((pos = text.find("<?lua", pos)) != std::string::npos)
                luaTags[pos++] = true;
        }
        {
            size_t pos = 0;
            while ((pos = text.find("?>", pos)) != std::string::npos)
                luaTags[pos++] = false;
        }

        // erase all tags from the map that are between two quotes
        {
            std::map<size_t, bool>::iterator it = luaTags.begin();
            std::map<size_t, bool>::iterator it2 = it;
            bool bBetweenQuotes = false;
            size_t pos = 0;
            while ((pos = getNextQuote(text, pos)) != std::string::npos)
            {
                while ((it != luaTags.end()) && (it->first < pos))
                {
                    if (bBetweenQuotes)
                    {
                        it2++;
                        if(it->second && !(it2->second) && it2->first < pos)
                            it = ++it2;
                        else
                            luaTags.erase(it++);
                    }
                    else
                        ++it;
                }
                bBetweenQuotes = !bBetweenQuotes;
                pos++;
            }
        }

        // check whether on every opening <?lua tag a closing ?> tag follows
        {
            bool expectedValue = true;
            for (std::map<size_t, bool>::iterator it = luaTags.begin(); it != luaTags.end(); ++it)
            {
                if (it->second == expectedValue)
                    expectedValue = !expectedValue;
                else
                {
                    expectedValue = false;
                    break;
                }
            }
            if (!expectedValue)
            {
                COUT(2) << "Warning: Error in level file" << std::endl;
                // todo: errorhandling
                return "";
            }
        }

        // Use a stringstream object to speed up the parsing
        std::ostringstream output;

        // cut the original string into pieces and put them together with print() instead of lua tags
        {
            std::map<size_t, bool>::iterator it = luaTags.begin();
            bool bInPrintFunction = true;
            size_t start = 0;
            size_t end = 0;

            do
            {
                if (it != luaTags.end())
                    end = (*(it++)).first;
                else
                    end = std::string::npos;

                unsigned int equalSignCounter = 0;

                if (bInPrintFunction)
                {
                    // count ['='[ and ]'='] and replace tags with print([[ and ]])
                    std::string temp = text.substr(start, end - start);
                    {
                    size_t pos = 0;
                    while ((pos = temp.find('[', pos)) != std::string::npos)
                    {
                        unsigned int tempCounter = 1;
                        size_t tempPos = pos++;
                        while(temp[++tempPos] == '=')
                        {
                            tempCounter++;
                        }
                        if(temp[tempPos] != '[')
                        {
                            tempCounter = 0;
                        }
                        else if(tempCounter == 0)
                        {
                            tempCounter = 1;
                        }
                        if (tempCounter > equalSignCounter)
                            equalSignCounter = tempCounter;
                        }
                    }
                    {
                        size_t pos = 0;
                        while ((pos = temp.find(']', pos)) != std::string::npos)
                        {
                            unsigned int tempCounter = 1;
                            size_t tempPos = pos++;
                            while(temp[++tempPos] == '=')
                            {
                                tempCounter++;
                            }
                            if(temp[tempPos] != ']')
                            {
                                tempCounter = 0;
                            }
                            else if(tempCounter == 0)
                            {
                                tempCounter = 1;
                            }
                            if (tempCounter > equalSignCounter)
                                equalSignCounter = tempCounter;
                        }
                    }
                    std::string equalSigns = "";
                    for(unsigned int i = 0; i < equalSignCounter; i++)
                    {
                        equalSigns += "=";
                    }
                    output << "print([" + equalSigns + "[" + temp + "]" + equalSigns +"])";
                    start = end + 5;
                }
                else
                {
                    output << text.substr(start, end - start);
                    start = end + 2;
                }

                bInPrintFunction = !bInPrintFunction;
            }
            while (end != std::string::npos);
        }

        return output.str();
    }
}
