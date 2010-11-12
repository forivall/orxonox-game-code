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
#include "core/Identifier.h"
#include "core/ConfigFileManager.h"
#include "core/ConfigValueContainer.h"
#include "CommandExecutor.h"
#include "ConsoleCommand.h"
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

        namespace detail
        {
            bool groupIsVisible(const std::map<std::string, ConsoleCommand*>& group, bool bOnlyShowHidden)
            {
                for (std::map<std::string, ConsoleCommand*>::const_iterator it_command = group.begin(); it_command != group.end(); ++it_command)
                    if (it_command->second->isActive() && it_command->second->hasAccess() && (!it_command->second->isHidden())^bOnlyShowHidden)
                        return true;

                return false;
            }

            ArgumentCompletionList _groupsandcommands(const std::string& fragment, bool bOnlyShowHidden)
            {
                ArgumentCompletionList groupList;
                std::string fragmentLC = getLowercase(fragment);

                const std::map<std::string, std::map<std::string, ConsoleCommand*> >& commands = ConsoleCommand::getCommands();
                for (std::map<std::string, std::map<std::string, ConsoleCommand*> >::const_iterator it_group = commands.begin(); it_group != commands.end(); ++it_group)
                    if (groupIsVisible(it_group->second, bOnlyShowHidden) && it_group->first != "" && (fragmentLC == "" || getLowercase(it_group->first).find_first_of(fragmentLC) == 0))
                        groupList.push_back(ArgumentCompletionListElement(it_group->first, getLowercase(it_group->first)));

                std::map<std::string, std::map<std::string, ConsoleCommand*> >::const_iterator it_group = commands.find("");
                if (it_group != commands.end())
                {
                    if (!groupList.empty())
                        groupList.push_back(ArgumentCompletionListElement("", "", "\n"));

                    for (std::map<std::string, ConsoleCommand*>::const_iterator it_command = it_group->second.begin(); it_command != it_group->second.end(); ++it_command)
                        if (it_command->second->isActive() && it_command->second->hasAccess() && (!it_command->second->isHidden())^bOnlyShowHidden && (fragmentLC == "" || getLowercase(it_command->first).find_first_of(fragmentLC) == 0))
                            groupList.push_back(ArgumentCompletionListElement(it_command->first, getLowercase(it_command->first)));
                }

                if (!groupList.empty() && groupList.back().getDisplay() == "\n")
                    groupList.pop_back();

                return groupList;
            }

            ArgumentCompletionList _subcommands(const std::string& fragment, const std::string& group, bool bOnlyShowHidden)
            {
                ArgumentCompletionList commandList;

                std::string groupLC = getLowercase(group);

                std::map<std::string, std::map<std::string, ConsoleCommand*> >::const_iterator it_group = ConsoleCommand::getCommands().begin();
                for ( ; it_group != ConsoleCommand::getCommands().end(); ++it_group)
                    if (getLowercase(it_group->first) == groupLC)
                        break;

                if (it_group != ConsoleCommand::getCommands().end())
                {
                    for (std::map<std::string, ConsoleCommand*>::const_iterator it_command = it_group->second.begin(); it_command != it_group->second.end(); ++it_command)
                        if (it_command->second->isActive() && it_command->second->hasAccess() && (!it_command->second->isHidden())^bOnlyShowHidden)
                            commandList.push_back(ArgumentCompletionListElement(it_command->first, getLowercase(it_command->first)));
                }

                return commandList;
            }
        }

        ARGUMENT_COMPLETION_FUNCTION_IMPLEMENTATION(groupsandcommands)(const std::string& fragment)
        {
            return detail::_groupsandcommands(fragment, false);
        }

        ARGUMENT_COMPLETION_FUNCTION_IMPLEMENTATION(subcommands)(const std::string& fragment, const std::string& group)
        {
            return detail::_subcommands(fragment, group, false);
        }

        ARGUMENT_COMPLETION_FUNCTION_IMPLEMENTATION_MULTI(command)(const std::string& fragment)
        {
            CommandEvaluation evaluation = CommandExecutor::evaluate(fragment);
            const std::string& hint = evaluation.hint();

            if (evaluation.getPossibleArguments().size() > 0 && evaluation.getPossibleArgumentsSize() > 0)
            {
                return evaluation.getPossibleArguments();
            }
            else
            {
                ArgumentCompletionList list;
                list.push_back(ArgumentCompletionListElement("", "", hint));
                return list;
            }
        }

        ARGUMENT_COMPLETION_FUNCTION_IMPLEMENTATION_MULTI(hiddencommand)(const std::string& fragment)
        {
            SubString tokens(fragment, " ", SubString::WhiteSpaces, false, '\\', true, '"', true, '{', '}', true, '\0');

            if (tokens.size() == 0)
                return detail::_groupsandcommands(fragment, true);

            if (ConsoleCommand::getCommandLC(getLowercase(tokens[0])))
                return ARGUMENT_COMPLETION_FUNCTION_CALL(command)(fragment);

            if (tokens.size() == 1)
            {
                std::map<std::string, std::map<std::string, ConsoleCommand*> >::const_iterator it_group = ConsoleCommand::getCommands().find(tokens[0]);
                if (it_group != ConsoleCommand::getCommands().end())
                    return detail::_subcommands(fragment, tokens[0], true);
                else
                    return detail::_groupsandcommands(fragment, true);
            }

            if (ConsoleCommand::getCommandLC(getLowercase(tokens[0]), getLowercase(tokens[1])))
                return ARGUMENT_COMPLETION_FUNCTION_CALL(command)(fragment);

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