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
  @brief Contains all the necessary forward declarations for all classes and structs.
*/

#ifndef _CorePrereqs_H__
#define _CorePrereqs_H__

#include "OrxonoxConfig.h"

//-----------------------------------------------------------------------
// Shared library settings
//-----------------------------------------------------------------------
#if defined(ORXONOX_PLATFORM_WINDOWS) && !defined( CORE_STATIC_BUILD )
#  ifdef CORE_SHARED_BUILD
#    define _CoreExport __declspec(dllexport)
#  else
#    if defined( __MINGW32__ )
#      define _CoreExport
#    else
#      define _CoreExport __declspec(dllimport)
#    endif
#  endif
#elif defined ( ORXONOX_GCC_VISIBILITY )
#  define _CoreExport  __attribute__ ((visibility("default")))
#else
#  define _CoreExport
#endif


//-----------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------
namespace orxonox
{
    namespace XMLPort
    {
        enum Mode
        {
            LoadObject,
            SaveObject,
            ExpandObject
        };
    }

    namespace KeybindMode
    {
        enum Value
        {
            OnPress,
            OnHold,
            OnRelease,
            None
        };
    };

    typedef std::string LanguageEntryLabel;

    class ArgumentCompleter;
    class ArgumentCompletionListElement;
    class BaseFactory;
    class BaseMetaObjectListElement;
    class BaseObject;
    template <class T>
    class ClassFactory;
    template <class T>
    class ClassIdentifier;
    class ClassTreeMask;
    class ClassTreeMaskIterator;
    class ClassTreeMaskNode;
    class ClassTreeMaskObjectIterator;
    class Clock;
    class CommandEvaluation;
    class CommandExecutor;
    class CommandLine;
    class CommandLineArgument;
    class ConfigFile;
    class ConfigFileEntry;
    class ConfigFileEntryComment;
    class ConfigFileEntryValue;
    class ConfigFileManager;
    class ConfigFileSection;
    class ConfigValueContainer;
    class ConsoleCommand;
    class Core;
    class DynLib;
    class DynLibManager;
    struct Event;
    class EventContainer;
    class Executor;
    template <class T>
    class ExecutorMember;
    class ExecutorStatic;
    class Factory;
    class Functor;
    template <class T>
    class FunctorMember;
    class FunctorStatic;
    class GraphicsManager;
    class GUIManager;
    class Identifier;
    class IRC;
    template <class T>
    class Iterator;
    class IteratorBase;
    class Language;
    class LanguageEntry;
    class Loader;
    class LuaState;
    class MemoryArchive;
    class MemoryArchiveFactory;
    class MetaObjectList;
    class MetaObjectListElement;
    class Namespace;
    class NamespaceNode;
    template <class T>
    class ObjectList;
    class ObjectListBase;
    class ObjectListBaseElement;
    template <class T>
    class ObjectListElement;
    template <class T>
    class ObjectListIterator;
    class OgreWindowEventListener;
    class OrxonoxClass;
    struct ResourceInfo;
    class Shell;
    class ShellListener;
    template <class T>
    class SubclassIdentifier;
    class TclBind;
    struct TclInterpreterBundle;
    template <class T>
    class TclThreadList;
    class TclThreadManager;
    class Template;
    class WindowEventListener;
    class XMLFile;
    class XMLNameListener;
    template <class T, class O>
    class XMLPortClassObjectContainer;
    template <class T>
    class XMLPortClassParamContainer;
    class XMLPortObjectContainer;
    class XMLPortParamContainer;

    // game states
    class Game;
    class GameState;
    struct GameStateInfo;
    struct GameStateTreeNode;

    // input
    class BaseCommand;
    class BufferedParamCommand;
    class Button;
    class CalibratorCallback;
    class HalfAxis;
    class InputBuffer;
    class InputDevice;
    template <class Traits>
    class InputDeviceTemplated;
    class InputHandler;
    class InputManager;
    class InputState;
    class JoyStick;
    class Mouse;
    class Keyboard;
    class KeyBinder;
    class KeyDetector;
    class ParamCommand;
    class SimpleCommand;


    // multithreading
    class Thread;
    class ThreadPool;
}

// CppTcl
namespace Tcl
{
    class interpreter;
    class object;
}

// Boost
namespace boost
{
    namespace filesystem
    {
        struct path_traits;
        template <class String, class Traits> class basic_path;
        typedef basic_path<std::string, path_traits> path;
    }
    class thread;
    class mutex;
    class shared_mutex;
    class condition_variable;
}

// Ogre
namespace Ogre
{
    class DataStream;
    template <class T> class SharedPtr;
    typedef SharedPtr<DataStream> DataStreamPtr;
}
namespace orxonox
{
    // Import the Ogre::DataStream
    using Ogre::DataStream;
    using Ogre::DataStreamPtr;
}

// CEGUI
namespace CEGUI
{
    class DefaultLogger;
    class Logger;
    class LuaScriptModule;

    class OgreCEGUIRenderer;
    class OgreCEGUIResourceProvider;
    class OgreCEGUITexture;
}

// Lua
struct lua_State;

// TinyXML and TinyXML++
class TiXmlString;
class TiXmlOutStream;
class TiXmlNode;
class TiXmlHandle;
class TiXmlDocument;
class TiXmlElement;
class TiXmlComment;
class TiXmlUnknown;
class TiXmlAttribute;
class TiXmlText;
class TiXmlDeclaration;
class TiXmlParsingData;
namespace ticpp
{
    class Document;
    class Element;
    class Declaration;
    class StylesheetReference;
    class Text;
    class Comment;
    class Attribute;
}
namespace orxonox
{
    using ticpp::Document;
    using ticpp::Element;
    using ticpp::Declaration;
    using ticpp::StylesheetReference;
    using ticpp::Text;
    using ticpp::Comment;
    using ticpp::Attribute;
}


#endif /* _CorePrereqs_H__ */