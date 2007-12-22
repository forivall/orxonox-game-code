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
 *      ...
 *   Co-authors:
 *      ...
 *
 */

#include <fstream>
#include "ConfigValueContainer.h"
#include "../../misc/Tokenizer.h"
#include "../../misc/String2Number.h"

#define CONFIGFILEPATH "orxonox.ini"

namespace orxonox
{
    std::list<std::string>* ConfigValueContainer::configFileLines_s = 0; // Set the static member variable configFileLines_s to zero
    bool ConfigValueContainer::readConfigFile_s = false;                 // Set the static member variable readConfigFile_s to false

    /**
        @brief Constructor: Converts the default-value to a string, checks the config-file for a changed value, sets the intern value variable.
        @param value This is only needed to determine the right type.
        @param classname The name of the class the variable belongs to
        @param varname The name of the variable
        @param defvalue The default-value
    */
    ConfigValueContainer::ConfigValueContainer(const std::string& classname, const std::string& varname, int defvalue)
    {
        this->classname_ = classname;
        this->varname_ = varname;
        this->type_ = Int;

        this->defvalueString_ = number2String(defvalue, "0");                   // Try to convert the default-value to a string
        this->searchConfigFileLine();                                           // Search the entry in the config-file

        std::string valueString = this->parseValueString();                     // Parses the value string from the config-file-entry
        if (!string2Number(this->value_.value_int_, valueString, defvalue))     // Try to convert the string to a value
            this->setConfigFileEntyToDefault();                                 // The conversion failed

        std::cout << "CVC: int: " << this->value_.value_int_ << std::endl;
    }

    /**
        @brief Constructor: Converts the default-value to a string, checks the config-file for a changed value, sets the intern value variable.
        @param value This is only needed to determine the right type.
        @param classname The name of the class the variable belongs to
        @param varname The name of the variable
        @param defvalue The default-value
    */
    ConfigValueContainer::ConfigValueContainer(const std::string& classname, const std::string& varname, unsigned int defvalue)
    {
        this->classname_ = classname;
        this->varname_ = varname;
        this->type_ = uInt;

        this->defvalueString_ = number2String(defvalue, "0");                   // Try to convert the default-value to a string
        this->searchConfigFileLine();                                           // Search the entry in the config-file

        std::string valueString = this->parseValueString();                     // Parses the value string from the config-file-entry
        if (!string2Number(this->value_.value_uint_, valueString, defvalue))    // Try to convert the string to a value
            this->setConfigFileEntyToDefault();                                 // The conversion failed

        std::cout << "CVC: uint: " << this->value_.value_uint_ << std::endl;
    }

    /**
        @brief Constructor: Converts the default-value to a string, checks the config-file for a changed value, sets the intern value variable.
        @param value This is only needed to determine the right type.
        @param classname The name of the class the variable belongs to
        @param varname The name of the variable
        @param defvalue The default-value
    */
    ConfigValueContainer::ConfigValueContainer(const std::string& classname, const std::string& varname, char defvalue)
    {
        this->classname_ = classname;
        this->varname_ = varname;
        this->type_ = Char;

        this->defvalueString_ = number2String((int)defvalue, "0");              // Try to convert the default-value to a string
        this->searchConfigFileLine();                                           // Search the entry in the config-file

        std::string valueString = this->parseValueString();                     // Parses the value string from the config-file-entry
        // I used value_int_ instead of value_char_ to avoid number <-> char confusion in the config-file
        if (!string2Number(this->value_.value_int_, valueString, (int)defvalue))// Try to convert the string to a value
            this->setConfigFileEntyToDefault();                                 // The conversion failed
    }

    /**
        @brief Constructor: Converts the default-value to a string, checks the config-file for a changed value, sets the intern value variable.
        @param value This is only needed to determine the right type.
        @param classname The name of the class the variable belongs to
        @param varname The name of the variable
        @param defvalue The default-value
    */
    ConfigValueContainer::ConfigValueContainer(const std::string& classname, const std::string& varname, unsigned char defvalue)
    {
        this->classname_ = classname;
        this->varname_ = varname;
        this->type_ = uChar;

        this->defvalueString_ = number2String((unsigned int)defvalue, "0");     // Try to convert the default-value to a string
        this->searchConfigFileLine();                                           // Search the entry in the config-file

        std::string valueString = this->parseValueString();                     // Parses the value string from the config-file-entry
        // I used value_uint_ instead of value_uchar_ to avoid number <-> char confusion in the config-file
        if (!string2Number(this->value_.value_uint_, valueString, (unsigned int)defvalue)) // Try to convert the string to a value
            this->setConfigFileEntyToDefault();                                 // The conversion failed
    }

    /**
        @brief Constructor: Converts the default-value to a string, checks the config-file for a changed value, sets the intern value variable.
        @param value This is only needed to determine the right type.
        @param classname The name of the class the variable belongs to
        @param varname The name of the variable
        @param defvalue The default-value
    */
    ConfigValueContainer::ConfigValueContainer(const std::string& classname, const std::string& varname, float defvalue)
    {
        this->classname_ = classname;
        this->varname_ = varname;
        this->type_ = Float;

        this->defvalueString_ = number2String(defvalue, "0.000000");            // Try to convert the default-value to a string
        this->searchConfigFileLine();                                           // Search the entry in the config-file

        std::string valueString = this->parseValueString();                     // Parses the value string from the config-file-entry
        if (!string2Number(this->value_.value_float_, valueString, defvalue))   // Try to convert the string to a value
            this->setConfigFileEntyToDefault();                                 // The conversion failed
    }

    /**
        @brief Constructor: Converts the default-value to a string, checks the config-file for a changed value, sets the intern value variable.
        @param value This is only needed to determine the right type.
        @param classname The name of the class the variable belongs to
        @param varname The name of the variable
        @param defvalue The default-value
    */
    ConfigValueContainer::ConfigValueContainer(const std::string& classname, const std::string& varname, double defvalue)
    {
        this->classname_ = classname;
        this->varname_ = varname;
        this->type_ = Double;

        this->defvalueString_ = number2String(defvalue, "0.000000");            // Try to convert the default-value to a string
        this->searchConfigFileLine();                                           // Search the entry in the config-file

        std::string valueString = this->parseValueString();                     // Parses the value string from the config-file-entry
        if (!string2Number(this->value_.value_double_, valueString, defvalue))  // Try to convert the string to a value
            this->setConfigFileEntyToDefault();                                 // The conversion failed
    }

    /**
        @brief Constructor: Converts the default-value to a string, checks the config-file for a changed value, sets the intern value variable.
        @param value This is only needed to determine the right type.
        @param classname The name of the class the variable belongs to
        @param varname The name of the variable
        @param defvalue The default-value
    */
    ConfigValueContainer::ConfigValueContainer(const std::string& classname, const std::string& varname, bool defvalue)
    {
        this->classname_ = classname;
        this->varname_ = varname;
        this->type_ = Bool;

        // Convert the default-value from bool to string
        if (defvalue)
            this->defvalueString_ = "true";
        else
            this->defvalueString_ = "false";

        this->searchConfigFileLine();                                           // Search the entry in the config-file

        std::string valueString = this->parseValueString();                     // Parses the value string from the config-file-entry

        // Try to parse the value-string - is it a word?
        if (valueString.find("true") < valueString.size()
         || valueString.find("True") < valueString.size()
         || valueString.find("yes") < valueString.size()
         || valueString.find("Yes") < valueString.size())
            this->value_.value_bool_ = true;
        else if (valueString.find("false") < valueString.size()
              || valueString.find("False") < valueString.size()
              || valueString.find("no") < valueString.size()
              || valueString.find("No") < valueString.size())
            this->value_.value_bool_ = false;
        else
        {
            // Its not a known word - is it a number?
            std::string valueString = this->parseValueString();                     // Parses the value string from the config-file-entry
            if (!string2Number(this->value_.value_bool_, valueString, defvalue))    // Try to convert the string to a value
                this->setConfigFileEntyToDefault();                                 // The conversion failed
        }
    }

    /**
        @brief Constructor: Converts the default-value to a string, checks the config-file for a changed value, sets the intern value variable.
        @param value This is only needed to determine the right type.
        @param classname The name of the class the variable belongs to
        @param varname The name of the variable
        @param defvalue The default-value
    */
    ConfigValueContainer::ConfigValueContainer(const std::string& classname, const std::string& varname, const std::string& defvalue)
    {
        this->classname_ = classname;
        this->varname_ = varname;
        this->type_ = String;

        this->defvalueString_ = "\"" + defvalue + "\"";                         // Convert the string to a "config-file-string" with quotes

        this->searchConfigFileLine();                                           // Search the entry in the config-file
        std::string valueString = this->parseValueString(false);                // Parses the value string from the config-file-entry

        // Strip the quotes
        unsigned int pos1 = valueString.find("\"") + 1;
        unsigned int pos2 = valueString.find("\"", pos1);

        // Check if the entry was correctly quoted
        if (pos1 < valueString.length() && pos2 < valueString.length() && !(valueString.find("\"", pos2 + 1) < valueString.length()))
        {
            // It was - get the string between the quotes
            valueString = valueString.substr(pos1, pos2 - pos1);
            this->value_string_ = valueString;
        }
        else
        {
            // It wasn't - use the default-value and restore the entry in the config-file.
            this->value_string_ = defvalue;
            this->setConfigFileEntyToDefault();
        }
    }

    /**
        @brief Constructor: Converts the default-value to a string, checks the config-file for a changed value, sets the intern value variable.
        @param value This is only needed to determine the right type.
        @param classname The name of the class the variable belongs to
        @param varname The name of the variable
        @param defvalue The default-value
    */
    ConfigValueContainer::ConfigValueContainer(const std::string& classname, const std::string& varname, const char* defvalue)
    {
        this->classname_ = classname;
        this->varname_ = varname;
        this->type_ = ConstChar;

        this->defvalueString_ = "\"" + std::string(defvalue) + "\"";            // Convert the string to a "config-file-string" with quotes

        this->searchConfigFileLine();                                           // Search the entry in the config-file
        std::string valueString = this->parseValueString(false);                // Parses the value string from the config-file-entry

        // Strip the quotes
        unsigned int pos1 = valueString.find("\"") + 1;
        unsigned int pos2 = valueString.find("\"", pos1);

        // Check if the entry was correctly quoted
        if (pos1 < valueString.length() && pos2 < valueString.length() && !(valueString.find("\"", pos2 + 1) < valueString.length()))
        {
            // It was - get the string between the quotes
            valueString = valueString.substr(pos1, pos2 - pos1);
            this->value_string_ = valueString;
        }
        else
        {
            // It wasn't - use the default-value and restore the entry in the config-file.
            this->value_string_ = defvalue;
            this->setConfigFileEntyToDefault();
        }
    }

    /**
        @brief Constructor: Converts the default-value to a string, checks the config-file for a changed value, sets the intern value variable.
        @param value This is only needed to determine the right type.
        @param classname The name of the class the variable belongs to
        @param varname The name of the variable
        @param defvalue The default-value
    */
    ConfigValueContainer::ConfigValueContainer(const std::string& classname, const std::string& varname, Ogre::Vector2 defvalue)
    {
        this->classname_ = classname;
        this->varname_ = varname;
        this->type_ = Vector2;

        // Try to convert the default-value from Vector2 to string
        std::ostringstream ostream;
        if (ostream << "(" << defvalue.x << "," << defvalue.y << ")")
            this->defvalueString_ = ostream.str();
        else
            this->defvalueString_ = "(0,0)";

        this->searchConfigFileLine();                                           // Search the entry in the config-file
        std::string valueString = this->parseValueString();                     // Parses the value string from the config-file-entry

        // Strip the value-string
        unsigned int pos1 = valueString.find("(") + 1;
        unsigned int pos2 = valueString.find(")", pos1);

        // Try to convert the stripped value-string to Vector2
        if (pos1 < valueString.length() && pos2 < valueString.length() && pos1 < pos2)
        {
            valueString = valueString.substr(pos1, pos2 - pos1);
            std::vector<std::string> tokens = tokenize(valueString, ",");
            if (!string2Number(this->value_vector2_.x, tokens[0], defvalue.x))
                this->setConfigFileEntyToDefault();
            if (!string2Number(this->value_vector2_.y, tokens[1], defvalue.y))
                this->setConfigFileEntyToDefault();
        }
        else
        {
            this->value_vector2_ = defvalue;
            this->setConfigFileEntyToDefault();
        }
    }

    /**
        @brief Constructor: Converts the default-value to a string, checks the config-file for a changed value, sets the intern value variable.
        @param value This is only needed to determine the right type.
        @param classname The name of the class the variable belongs to
        @param varname The name of the variable
        @param defvalue The default-value
    */
    ConfigValueContainer::ConfigValueContainer(const std::string& classname, const std::string& varname, Ogre::Vector3 defvalue)
    {
        this->classname_ = classname;
        this->varname_ = varname;
        this->type_ = Vector3;

        // Try to convert the default-value from Vector3 to string
        std::ostringstream ostream;
        if (ostream << "(" << defvalue.x << "," << defvalue.y << "," << defvalue.z << ")")
            this->defvalueString_ = ostream.str();
        else
            this->defvalueString_ = "(0,0,0)";

        this->searchConfigFileLine();                                           // Search the entry in the config-file
        std::string valueString = this->parseValueString();                     // Parses the value string from the config-file-entry

        // Strip the value-string
        unsigned int pos1 = valueString.find("(") + 1;
        unsigned int pos2 = valueString.find(")", pos1);

        // Try to convert the stripped value-string to Vector3
        if (pos1 < valueString.length() && pos2 < valueString.length() && pos1 < pos2)
        {
            valueString = valueString.substr(pos1, pos2 - pos1);
            std::vector<std::string> tokens = tokenize(valueString, ",");
            if (!string2Number(this->value_vector3_.x, tokens[0], defvalue.x))
                this->setConfigFileEntyToDefault();
            if (!string2Number(this->value_vector3_.y, tokens[1], defvalue.y))
                this->setConfigFileEntyToDefault();
            if (!string2Number(this->value_vector3_.z, tokens[2], defvalue.z))
                this->setConfigFileEntyToDefault();
        }
        else
        {
            this->value_vector3_ = defvalue;
            this->setConfigFileEntyToDefault();
        }
    }

    /**
        @brief Constructor: Converts the default-value to a string, checks the config-file for a changed value, sets the intern value variable.
        @param value This is only needed to determine the right type.
        @param classname The name of the class the variable belongs to
        @param varname The name of the variable
        @param defvalue The default-value
    */
    ConfigValueContainer::ConfigValueContainer(const std::string& classname, const std::string& varname, Ogre::ColourValue defvalue)
    {
        this->classname_ = classname;
        this->varname_ = varname;
        this->type_ = ColourValue;

        // Try to convert the default-value from ColourValue to string
        std::ostringstream ostream;
        if (ostream << "(" << defvalue.r << "," << defvalue.g << "," << defvalue.b << "," << defvalue.a << ")")
            this->defvalueString_ = ostream.str();
        else
            this->defvalueString_ = "(0,0,0,0)";

        this->searchConfigFileLine();                                           // Search the entry in the config-file
        std::string valueString = this->parseValueString();                     // Parses the value string from the config-file-entry

        // Strip the value-string
        unsigned int pos1 = valueString.find("(") + 1;
        unsigned int pos2 = valueString.find(")", pos1);

        // Try to convert the stripped value-string to Vector3
        if (pos1 < valueString.length() && pos2 < valueString.length() && pos1 < pos2)
        {
            valueString = valueString.substr(pos1, pos2 - pos1);
            std::vector<std::string> tokens = tokenize(valueString, ",");
            if (!string2Number(this->value_colourvalue_.r, tokens[0], defvalue.r))
                this->setConfigFileEntyToDefault();
            if (!string2Number(this->value_colourvalue_.g, tokens[1], defvalue.g))
                this->setConfigFileEntyToDefault();
            if (!string2Number(this->value_colourvalue_.b, tokens[2], defvalue.b))
                this->setConfigFileEntyToDefault();
            if (!string2Number(this->value_colourvalue_.a, tokens[3], defvalue.a))
                this->setConfigFileEntyToDefault();
        }
        else
        {
            this->value_colourvalue_ = defvalue;
            this->setConfigFileEntyToDefault();
        }
    }

    /**
        @brief Sets the corresponding enty in the config-file back to the default value.
    */
    void ConfigValueContainer::setConfigFileEntyToDefault()
    {
        (*this->configFileLine_) = this->varname_ + "=" + this->defvalueString_;
        ConfigValueContainer::writeConfigFile(CONFIGFILEPATH);
    }


    /**
        @brief Searches the corresponding entry in the config-file and creates it, if there is no entry.
    */
    void ConfigValueContainer::searchConfigFileLine()
    {
        // Read the file if needed
        if (!ConfigValueContainer::readConfigFile_s)
            ConfigValueContainer::readConfigFile(CONFIGFILEPATH);

        // The string of the section we're searching
        std::string section = "";
        section.append("[");
        section.append(this->classname_);
        section.append("]");

        // Iterate through all config-file-lines
        bool success = false;
        std::list<std::string>::iterator it1;
        for(it1 = ConfigValueContainer::configFileLines_s->begin(); it1 != ConfigValueContainer::configFileLines_s->end(); ++it1)
        {
            // Don't try to parse comments
            if (this->isComment(*it1))
                continue;

            if ((*it1).find(section) < (*it1).length())
            {
                // We found the right section
                bool bLineIsEmpty = false;
                std::list<std::string>::iterator positionToPutNewLineAt;

                // Iterate through all lines in the section
                std::list<std::string>::iterator it2;
                for(it2 = ++it1; it2 != ConfigValueContainer::configFileLines_s->end(); ++it2)
                {
                    // Don't try to parse comments
                    if (this->isComment(*it2))
                        continue;

                    // This if-else block is used to write a new line right after the last line of the
                    // section but in front of the following empty lines before the next section.
                    // (So this helps to keep a nice formatting with empty-lines between sections in the config-file)
                    if (this->isEmpty(*it2))
                    {
                        if (!bLineIsEmpty)
                        {
                            bLineIsEmpty = true;
                            positionToPutNewLineAt = it2;
                        }
                    }
                    else
                    {
                        if (!bLineIsEmpty)
                            positionToPutNewLineAt = it2;

                        bLineIsEmpty = false;
                    }

                    // Look out for the beginning of the next section
                    unsigned int open = (*it2).find("[");
                    unsigned int close = (*it2).find("]");
                    if ((open < (*it2).length()) && (close < (*it2).length()) && (open < close))
                    {
                        // The next section startet, so our line isn't yet in the file - now we add it and safe the file
                        this->configFileLine_ = this->configFileLines_s->insert(positionToPutNewLineAt, this->varname_ + "=" + this->defvalueString_);
                        ConfigValueContainer::writeConfigFile(CONFIGFILEPATH);
                        success = true;
                        break;
                    }

                    // Look out for the variable-name
                    if ((*it2).find(this->varname_) < (*it2).length())
                    {
                        // We found the right line - safe it and return
                        this->configFileLine_ = it2;
                        success = true;
                        break;
                    }
                }

                // Check if we succeeded
                if (!success)
                {
                    // Looks like we found the right section, but the file ended without containing our variable - so we add it and safe the file
                    this->configFileLine_ = this->configFileLines_s->insert(positionToPutNewLineAt, this->varname_ + "=" + this->defvalueString_);
                    ConfigValueContainer::writeConfigFile(CONFIGFILEPATH);
                    success = true;
                }
                break;
            }
        }

        // Check if we succeeded
        if (!success)
        {
            // We obviously didn't found the right section, so we'll create it
            this->configFileLines_s->push_back("[" + this->classname_ + "]");                   // Create the section
            this->configFileLines_s->push_back(this->varname_ + "=" + this->defvalueString_);   // Create the line
            this->configFileLine_ = --this->configFileLines_s->end();                           // Set the pointer to the last element
            success = true;
            this->configFileLines_s->push_back("");                                             // Add an empty line - this is needed for the algorithm in the searchConfigFileLine-function
            ConfigValueContainer::writeConfigFile(CONFIGFILEPATH);                              // Save the changed config-file
        }
    }

    /**
        @brief Determines if a line in the config-file is a comment.
        @param line The line to check
        @return True = it's a comment
    */
    bool ConfigValueContainer::isComment(const std::string& line)
    {
        // Strip the line, whitespaces are disturbing
        std::string teststring = getStrippedLine(line);

        // There are four possible comment-symbols:
        //  1) #comment in script-language style
        //  2) %comment in matlab style
        //  3) ;comment in unreal tournament config-file style
        //  4) //comment in code style
        if (teststring[0] == '#' || teststring[0] == '%' || teststring[0] == ';' || (teststring[0] == '/' && teststring[0] == '/'))
            return true;

        return false;
    }

    /**
        @brief Determines if a line in the config-file is empty (contains only whitespaces).
        @param line The line to check
        @return True = it's empty
    */
    bool ConfigValueContainer::isEmpty(const std::string& line)
    {
        return getStrippedLine(line) == "";
    }

    /**
        @brief Removes all whitespaces from a line.
        @param line The line to strip
        @return The stripped line
    */
    std::string ConfigValueContainer::getStrippedLine(const std::string& line)
    {
        std::string output = line;
        unsigned int pos;
        while ((pos = output.find(" ")) < output.length())
            output.erase(pos, 1);
        while ((pos = output.find("\t")) < output.length())
            output.erase(pos, 1);

        return output;
    }

    /**
        @brief Returns the part in the corresponding config-file-entry of the container that defines the value.
        @param bStripped True = strip the value-string
        @return The value-string
    */
    std::string ConfigValueContainer::parseValueString(bool bStripped)
    {
        std::string output;
        if (bStripped)
            output = this->getStrippedLine(*this->configFileLine_);
        else
            output = *this->configFileLine_;

        return output.substr(output.find("=") + 1);
    }

    /**
        @brief Reads the config-file and stores the lines in a list.
        @param filename The name of the config-file
    */
    void ConfigValueContainer::readConfigFile(const std::string& filename)
    {
        ConfigValueContainer::readConfigFile_s = true;

        // Create the list if needed
        if (!ConfigValueContainer::configFileLines_s)
            ConfigValueContainer::configFileLines_s = new std::list<std::string>;

        // This creates the file if it's not existing
        std::ofstream createFile;
        createFile.open(filename.c_str(), std::fstream::app);
        createFile.close();

        // Open the file
        std::ifstream file;
        file.open(filename.c_str(), std::fstream::in);

        char line[1024];

        // Iterate through the file and add the lines into the list
        while (file.good() && !file.eof())
        {
            file.getline(line, 1024);
            ConfigValueContainer::configFileLines_s->push_back(line);
//            std::cout << "### ->" << line << "<- : empty: " << isEmpty(line) << " comment: " << isComment(line) << std::endl;
        }

        // The last line is useless
        ConfigValueContainer::configFileLines_s->pop_back();

        // Add an empty line to the end of the file if needed
        // this is needed for the algorithm in the searchConfigFileLine-function
        if ((ConfigValueContainer::configFileLines_s->size() > 0) && !isEmpty(*ConfigValueContainer::configFileLines_s->rbegin()))
        {
//            std::cout << "### newline added" << std::endl;
            ConfigValueContainer::configFileLines_s->push_back("");
        }

        file.close();
    }

    /**
     *  @param Writes the content of the list, containing all lines of the config-file, into the config-file.
     *  @param filename The name of the config-file
     */
    void ConfigValueContainer::writeConfigFile(const std::string& filename)
    {
        // Make sure we stored the config-file in the list
        if (!ConfigValueContainer::readConfigFile_s)
            ConfigValueContainer::readConfigFile(filename);

        // Open the file
        std::ofstream file;
        file.open(filename.c_str(), std::fstream::out);

        // Iterate through the list an write the lines into the file
        std::list<std::string>::iterator it;
        for(it = ConfigValueContainer::configFileLines_s->begin(); it != ConfigValueContainer::configFileLines_s->end(); ++it)
        {
            file << (*it) << std::endl;
        }

        file.close();
    }
}
