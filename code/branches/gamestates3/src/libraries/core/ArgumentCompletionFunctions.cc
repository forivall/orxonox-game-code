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

#include "ArgumentCompletionFunctions.h"

#include <map>
#include <boost/version.hpp>
#include <boost/filesystem.hpp>

#include "util/Convert.h"
#include "util/StringUtils.h"
#include "Identifier.h"
#include "ConfigFileManager.h"
#include "ConfigValueContainer.h"
#include "TclThreadManager.h"

// Boost 1.36 has some issues with deprecated functions that have been omitted
#if (BOOST_VERSION == 103600)
#  define BOOST_LEAF_FUNCTION filename
#else
#  define BOOST_LEAF_FUNCTION leaf
#endif

namespace orxonox
{
    namespace autocompletion
    {
        ARGUMENT_COMPLETION_FUNCTION_IMPLEMENTATION(fallback)()
        {
            return ArgumentCompletionList();
        }

        ARGUMENT_COMPLETION_FUNCTION_IMPLEMENTATION(files)(const std::string& fragment)
        {
            ArgumentCompletionList dirlist;
            ArgumentCompletionList filelist;

            try
            {
                boost::filesystem::path input(fragment);
                boost::filesystem::path startdirectory(input.branch_path());

                if (!boost::filesystem::exists(startdirectory))
                {
                    startdirectory = ".";
                }
#ifdef ORXONOX_PLATFORM_WINDOWS
                else
                {
                    const std::string& dir = startdirectory.string();
                    if (dir.size() > 0 && dir[dir.size() - 1] == ':')
                        startdirectory = dir + '/';
                }
#endif

                boost::filesystem::directory_iterator file(startdirectory);
                boost::filesystem::directory_iterator end;

                while (file != end)
                {
                    if (boost::filesystem::is_directory(*file))
                        dirlist.push_back(ArgumentCompletionListElement(file->string() + '/', getLowercase(file->string()) + '/', file->BOOST_LEAF_FUNCTION() + '/'));
                    else
                        filelist.push_back(ArgumentCompletionListElement(file->string(), getLowercase(file->string()), file->BOOST_LEAF_FUNCTION()));
                    ++file;
                }
            }
            catch (...) {}

            filelist.insert(filelist.begin(), dirlist.begin(), dirlist.end());
            return filelist;
        }

        ARGUMENT_COMPLETION_FUNCTION_IMPLEMENTATION(settingssections)()
        {
            ArgumentCompletionList sectionList;

            const std::set<std::string>& names = SettingsConfigFile::getInstance().getSectionNames();
            for (std::set<std::string>::const_iterator it = names.begin(); it != names.end(); ++it)
                sectionList.push_back(ArgumentCompletionListElement(*it, getLowercase(*it)));

            return sectionList;
        }

        ARGUMENT_COMPLETION_FUNCTION_IMPLEMENTATION(settingsentries)(const std::string& fragment, const std::string& section)
        {
            ArgumentCompletionList entryList;
            SettingsConfigFile& settings = SettingsConfigFile::getInstance();
            const std::string& sectionLC = getLowercase(section);

            SettingsConfigFile::ContainerMap::const_iterator upper = settings.getContainerUpperBound(sectionLC);
            for (SettingsConfigFile::ContainerMap::const_iterator it = settings.getContainerLowerBound(sectionLC); it != upper; ++it)
                entryList.push_back(ArgumentCompletionListElement(it->second.second->getName(), it->second.first));

            return entryList;
        }

        ARGUMENT_COMPLETION_FUNCTION_IMPLEMENTATION(settingsvalue)(const std::string& fragment, const std::string& entry, const std::string& section)
        {
            ArgumentCompletionList oldValue;
            SettingsConfigFile& settings = SettingsConfigFile::getInstance();
            const std::string& sectionLC = getLowercase(section);
            const std::string& entryLC = getLowercase(entry);

            SettingsConfigFile::ContainerMap::const_iterator upper = settings.getContainerUpperBound(sectionLC);
            for (SettingsConfigFile::ContainerMap::const_iterator it = settings.getContainerLowerBound(sectionLC); it != upper; ++it)
            {
                if (it->second.first == entryLC)
                {
                    const std::string& valuestring = it->second.second->toString();
                    oldValue.push_back(ArgumentCompletionListElement(valuestring, getLowercase(valuestring), "Old value: " + valuestring));
                }
            }

            return oldValue;
        }

        ARGUMENT_COMPLETION_FUNCTION_IMPLEMENTATION(tclthreads)()
        {
            std::list<unsigned int> threadnumbers = TclThreadManager::getInstance().getThreadList();
            ArgumentCompletionList threads;

            for (std::list<unsigned int>::const_iterator it = threadnumbers.begin(); it != threadnumbers.end(); ++it)
                threads.push_back(ArgumentCompletionListElement(multi_cast<std::string>(*it)));

            return threads;
        }
    }
}