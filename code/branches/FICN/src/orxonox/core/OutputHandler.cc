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

#include "OutputHandler.h"

namespace orxonox
{
    /**
        @brief Constructor: Opens the logfile and writes the first line.
        @param logfilename The name of the logfile
    */
    OutputHandler::OutputHandler(const std::string& logfilename)
    {
        this->logfilename_ = logfilename;
        this->logfile_.open(this->logfilename_.c_str(), std::fstream::out);
        this->logfile_ << "Started log" << std::endl;
        this->logfile_.flush();
    }

    /**
        @brief Destructor: Writes the last line to the logfile and closes it.
    */
    OutputHandler::~OutputHandler()
    {
        this->logfile_ << "Closed log" << std::endl;
        this->logfile_.close();
    }

    /**
        @returns a reference to the only existing instance of the OutputHandler class.
    */
    OutputHandler& OutputHandler::getOutStream()
    {
        static OutputHandler orxout("orxonox.log");
        return orxout;
    }

    /**
        @brief Overloaded << operator, redirects the output to the console and the logfile.
        @param sb The streambuffer that should be shown in the console
        @return A reference to the OutputHandler itself
    */
    OutputHandler& OutputHandler::operator<<(std::streambuf* sb)
    {
        std::cout << sb;
        this->logfile_ << sb;
        this->logfile_.flush();
        return *this;
    }

    /**
        @brief Overloaded << operator, redirects the output to the console and the logfile.
        @param manipulator A function, manipulating the outstream.
        @return A reference to the OutputHandler itself
    */
    OutputHandler& OutputHandler::operator<<(std::ostream& (*manipulator)(std::ostream&))
    {
        manipulator(std::cout);
        manipulator(this->logfile_);
        this->logfile_.flush();
        return *this;
    }

    /**
        @brief Overloaded << operator, redirects the output to the console and the logfile.
        @param manipulator A function, manipulating the outstream.
        @return A reference to the OutputHandler itself
    */
    OutputHandler& OutputHandler::operator<<(std::ios& (*manipulator)(std::ios&))
    {
        manipulator(std::cout);
        manipulator(this->logfile_);
        this->logfile_.flush();
        return *this;
    }

    /**
        @brief Overloaded << operator, redirects the output to the console and the logfile.
        @param manipulator A function, manipulating the outstream.
        @return A reference to the OutputHandler itself
    */
    OutputHandler& OutputHandler::operator<<(std::ios_base& (*manipulator)(std::ios_base&))
    {
        manipulator(std::cout);
        manipulator(this->logfile_);
        this->logfile_.flush();
        return *this;
    }

    /**
        @brief Overloaded non-member << operator, redirects the output to the console and the logfile.
        @param output The value that should be shown in the console
        @return A reference to the OutputHandler itself
    *//*
    OutputHandler& operator<<(OutputHandler& out, char c)
    {
        std::cout << c;
        out.getLogfile() << c;
        out.getLogfile().flush();
        return out;
    }*/

    /**
        @brief Overloaded non-member << operator, redirects the output to the console and the logfile.
        @param output The value that should be shown in the console
        @return A reference to the OutputHandler itself
    *//*
    OutputHandler& operator<<(OutputHandler& out, signed char c)
    {
        std::cout << c;
        out.getLogfile() << c;
        out.getLogfile().flush();
        return out;
    }*/

    /**
        @brief Overloaded non-member << operator, redirects the output to the console and the logfile.
        @param output The value that should be shown in the console
        @return A reference to the OutputHandler itself
    *//*
    OutputHandler& operator<<(OutputHandler& out, unsigned char c)
    {
        std::cout << c;
        out.getLogfile() << c;
        out.getLogfile().flush();
        return out;
    }*/

    /**
        @brief Overloaded non-member << operator, redirects the output to the console and the logfile.
        @param output The value that should be shown in the console
        @return A reference to the OutputHandler itself
    *//*
    OutputHandler& operator<<(OutputHandler& out, const char* s)
    {
        std::cout << s;
        out.getLogfile() << s;
        out.getLogfile().flush();
        return out;
    }*/

    /**
        @brief Overloaded non-member << operator, redirects the output to the console and the logfile.
        @param output The value that should be shown in the console
        @return A reference to the OutputHandler itself
    *//*
    OutputHandler& operator<<(OutputHandler& out, const signed char* s)
    {
        std::cout << s;
        out.getLogfile() << s;
        out.getLogfile().flush();
        return out;
    }*/

    /**
        @brief Overloaded non-member << operator, redirects the output to the console and the logfile.
        @param output The value that should be shown in the console
        @return A reference to the OutputHandler itself
    *//*
    OutputHandler& operator<<(OutputHandler& out, const unsigned char* s)
    {
        std::cout << s;
        out.getLogfile() << s;
        out.getLogfile().flush();
        return out;
    }*/
 }
