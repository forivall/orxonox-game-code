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
 *      Reto Grieder
 *
 */

/**
@file
@ingroup Util Output
@brief
    Declaration of classes related to output (logging), most notably OutputHandler and OutputListener.
*/

#ifndef _OutputHandler_H__
#define _OutputHandler_H__

#include "UtilPrereqs.h"

#include <ostream>
#include <string>
#include <vector>
#include <utility>

namespace orxonox
{
    /**
    @brief
        Denotes different levels of text output (log output)

         - 0, None   : Very important output
         - 1, Error  : Errors
         - 2, Warning: Warnings
         - 3, Info   : Information
         - 4, Debug  : Debug information
         - 5, Verbose: More debug information
         - 6, Ultra  : Crazy debug information
    */
    namespace OutputLevel
    {
        enum Value
        {
            TDebug  = -1,
            None    = 0,
            Error   = 1,
            Warning = 2,
            Info    = 3,
            Debug   = 4,
            Verbose = 5,
            Ultra   = 6,
        };
    }

    // Forward declarations for classes in the source file
    class LogFileWriter;
    class ConsoleWriter;
    class MemoryLogWriter;

    /**
    @brief
        The OutputHandler acts like @c std::cout, but output isn't only shown in the console.

        Output passed to the OutputHandler is distributed to all registered listeners,
        for example the console, the logfile, or the ingame shell.

        You can register your own listener for output by inheriting from OutputListener.
        And if you need the output previously processed, iterate over it with
        OutputHandler::getOutputVectorBegin and OutputHandler::getOutputVectorEnd.

        The way to output text is to first set the desired output level with
        @ref getOutStream "OutputHandler::getOutStream(level)" and then use
        the "<<" operator like with @c std::cout. Alternatively you can use the COUT() macro.
    */
    class _UtilExport OutputHandler
    {
        public:
            //! Returns a reference to the only existing instance of the OutputHandler class.
            static OutputHandler& getInstance();

            //! Sets the output level and returns a stream to be used with "<<"
            static inline OutputHandler& getOutStream(int level)
                { return OutputHandler::getInstance().setOutputLevel(level); }

            typedef std::vector<std::pair<int, std::string> > OutputVector;
            //! Returns all output written so far (empty if disableMemoryLog() was called)
            const OutputVector& getOutput() const;

            //! Writes to all output devices
            static inline void log(const std::string& text)
                { OutputHandler::getOutStream(0).output(text) << std::endl; }

            //! Writes an error message to the output
            static inline void error(const std::string& text)
                { OutputHandler::getOutStream(1).output(text) << std::endl; }

            //! Writes a warning message to the output
            static inline void warning(const std::string& text)
                { OutputHandler::getOutStream(2).output(text) << std::endl; }

            //! Writes an informational message to the output
            static inline void info(const std::string& text)
                { OutputHandler::getOutStream(3).output(text) << std::endl; }

            //! Writes a debug message to the output
            static inline void debug(const std::string& text)
                { OutputHandler::getOutStream(4).output(text) << std::endl; }

            //! Registers an object that receives output via a provided std::ostream
            void registerOutputListener(OutputListener* listener);
            //! Unregisters an object that receives output via a provided std::ostream
            void unregisterOutputListener(OutputListener* listener);

            //! Set the log path once the program has been properly initialised
            void setLogPath(const std::string& path);
            /** Rewrites the log file (completely respects the current debug level).
                Once disableMemoryLog() has been called, this function will do nothing.
            */
            void rewriteLogFile();

            //! Disables the std::cout stream for output
            void disableCout();
            //! Enables the std::cout stream for output (startup behaviour)
            void enableCout();
            //! Stop writing to the memory buffer (call this as soon as possible to minimise memory usage)
            void disableMemoryLog();

            //! Sets the level of the incoming output and returns the OutputHandler
            inline OutputHandler& setOutputLevel(int level)
                { this->outputLevel_ = level; return *this; }

            //! Returns the level of the incoming output
            inline int getOutputLevel() const
                { return this->outputLevel_; }

            //! Returns the maximum debug level over all registered listeners (devices)
            static int getSoftDebugLevel() { return softDebugLevel_s; }
            //! Returns the soft debug level for a device by its name   @return The level or -1 if the listener was not found
            int  getSoftDebugLevel(const std::string& name) const;
            //! Sets the soft debug level for a listener by its name   @remarks Only works for registered listeners!
            void setSoftDebugLevel(const std::string& name, int level);

            /**
            @brief
                General template that copes with all output.
                Required because operator << might be ambiguous.
                @a output will be streamed into every listener with an appropriate debug level
            @return
                Returns a reference to the OutputHandler so you can use it again directly
            */
            template <class T>
            OutputHandler& output(const T& output);

            //! Overloaded << operator, redirects the output to the listeners
            inline OutputHandler& operator<<(unsigned char val)      { return this->output(val); }
            //! Overloaded << operator, redirects the output to the listeners
            inline OutputHandler& operator<<(short val)              { return this->output(val); }
            //! Overloaded << operator, redirects the output to the listeners
            inline OutputHandler& operator<<(unsigned short val)     { return this->output(val); }
            //! Overloaded << operator, redirects the output to the listeners
            inline OutputHandler& operator<<(int val)                { return this->output(val); }
            //! Overloaded << operator, redirects the output to the listeners
            inline OutputHandler& operator<<(unsigned int val)       { return this->output(val); }
            //! Overloaded << operator, redirects the output to the listeners
            inline OutputHandler& operator<<(long val)               { return this->output(val); }
            //! Overloaded << operator, redirects the output to the listeners
            inline OutputHandler& operator<<(unsigned long val)      { return this->output(val); }
            //! Overloaded << operator, redirects the output to the listeners
            inline OutputHandler& operator<<(long long val)          { return this->output(val); }
            //! Overloaded << operator, redirects the output to the listeners
            inline OutputHandler& operator<<(unsigned long long val) { return this->output(val); }
            //! Overloaded << operator, redirects the output to the listeners
            inline OutputHandler& operator<<(float val)              { return this->output(val); }
            //! Overloaded << operator, redirects the output to the listeners
            inline OutputHandler& operator<<(double val)             { return this->output(val); }
            //! Overloaded << operator, redirects the output to the listeners
            inline OutputHandler& operator<<(long double val)        { return this->output(val); }
            //! Overloaded << operator, redirects the output to the listeners
            inline OutputHandler& operator<<(const void* val)        { return this->output(val); }
            //! Overloaded << operator, redirects the output to the listeners
            inline OutputHandler& operator<<(bool val)               { return this->output(val); }

            //! Overloaded << operator, redirects the output to the listeners
            inline OutputHandler& operator<<(std::streambuf* sb)     { return this->output(sb); }

            //! Overloaded << operator, redirect the output of classes with self defined 'operator <<' to the listeners
            template <class T>
            inline OutputHandler& operator<<(const T& val)           { return this->output(val); }

            //! Overloaded << operator for std manipulators like std::endl, redirects the output to the listeners
            inline OutputHandler& operator<<(std::ostream&  (*manip)(std::ostream&))  { return this->output(manip); }
            //! Overloaded << operator for std manipulators like std::endl, redirects the output to the listeners
            inline OutputHandler& operator<<(std::ios&      (*manip)(std::ios&))      { return this->output(manip); }
            //! Overloaded << operator for std manipulators like std::endl, redirects the output to the listeners
            inline OutputHandler& operator<<(std::ios_base& (*manip)(std::ios_base&)) { return this->output(manip); }

            //! Dummy operator required by Debug.h for the ternary operator
            inline operator int() const { return 0; }

        private:
            OutputHandler();
            ~OutputHandler();
            OutputHandler(const OutputHandler& rhs);      //!< Copy-constructor: Unused and undefined

            /// Evaluates the maximum global log level
            void updateGlobalDebugLevel();

            std::vector<OutputListener*> listeners_;        //!< Array with all registered output listeners
            int                          outputLevel_;      //!< The level of the incoming output
            LogFileWriter*               logFile_;          //!< Writes output to the log file
            ConsoleWriter*               consoleWriter_;    //!< Writes to std::cout (can be disabled)
            MemoryLogWriter*             memoryBuffer_;     //!< Writes to memory as a buffer (can/must be stopped at some time)
            static int                   softDebugLevel_s;  //!< Maximum of all soft debug levels. @note This is only static for faster access
    };

    /**
    @brief
        Interface for listening to output.
    @remarks
        Remember to register the listener (not done automatically!)
    */
    class OutputListener
    {
        friend class OutputHandler;

    public:
        OutputListener(const std::string& name)
            : outputStream_(NULL)
            , name_(name)
            , softDebugLevel_(OutputLevel::Info)
        {}
        virtual ~OutputListener() {}

        //! Gets called whenever output is put into the stream
        virtual void outputChanged(int level) {}
        //! Returns the name of this output listener
        const std::string& getOutputListenerName() const { return this->name_; }

    protected:
        std::ostream*     outputStream_;   //!< Pointer to the associated output stream, can be NULL

    private:
        const std::string name_;           //!< Name of the listener, constant and unique!
        int               softDebugLevel_; //!< Current soft debug level that defines what kind of output is written to the stream
    };

    template<class T>
    inline OutputHandler& OutputHandler::output(const T& output)
    {
        for (std::vector<OutputListener*>::const_iterator it = this->listeners_.begin(); it != this->listeners_.end(); ++it)
        {
            if (this->outputLevel_ <= (*it)->softDebugLevel_ && (*it)->outputStream_ != NULL)
            {
                std::ostream& stream = *((*it)->outputStream_);
                stream << output;
                stream.flush();
                (*it)->outputChanged(this->outputLevel_);
            }
        }

        return *this;
    }
}

#endif /* _OutputHandler_H__ */
