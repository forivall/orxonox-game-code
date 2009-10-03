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

#include "KeyBinderManager.h"

#include "util/Debug.h"
#include "util/Exception.h"
#include "core/ConfigValueIncludes.h"
#include "core/ConsoleCommand.h"
#include "core/CoreIncludes.h"
#include "InputManager.h"
#include "KeyDetector.h"

namespace orxonox
{
    KeyBinderManager* KeyBinderManager::singletonPtr_s = 0;

    KeyBinderManager::KeyBinderManager()
        : currentBinder_(NULL)
        , bDefaultFileLoaded_(true)
        , bBinding_(false)
    {
        this->callbackFunction_ = createFunctor(&KeyBinderManager::callback, this);

        RegisterObject(KeyBinderManager);
        this->setConfigValues();

        // keybind console commands
        CommandExecutor::addConsoleCommandShortcut(createConsoleCommand(createFunctor(&KeyBinderManager::keybind,  this), "keybind" ));
        CommandExecutor::addConsoleCommandShortcut(createConsoleCommand(createFunctor(&KeyBinderManager::tkeybind, this), "tkeybind"));

        // Load default key binder
        this->setCurrent(defaultFilename_);
    }

    KeyBinderManager::~KeyBinderManager()
    {
        // Delete all remaining KeyBinders
        for (std::map<std::string, KeyBinder*>::const_iterator it = this->binders_.begin(); it != this->binders_.end(); ++it)
            delete it->second;
        delete this->callbackFunction_;
    }

    void KeyBinderManager::setConfigValues()
    {
        SetConfigValue(defaultFilename_, "keybindings.ini")
            .description("Filename for the default keybindings file.")
            .callback(this, &KeyBinderManager::defaultFilenameChanged);
    }

    void KeyBinderManager::defaultFilenameChanged()
    {
        if (this->bDefaultFileLoaded_)
            this->setCurrent(this->defaultFilename_);
    }

    void KeyBinderManager::setCurrent(const std::string& filename)
    {
        this->currentBinder_ = this->get(filename);
        if (filename == this->defaultFilename_)
            this->bDefaultFileLoaded_ = true;
        else
            this->bDefaultFileLoaded_ = false;
    }

    void KeyBinderManager::load(const std::string& filename)
    {
        std::map<std::string, KeyBinder*>::const_iterator it = this->binders_.find(filename);
        if (it != this->binders_.end())
            return;

        KeyBinder* binder = new KeyBinder(filename);
        this->binders_[filename] = binder;
    }

    void KeyBinderManager::unload(const std::string& filename)
    {
        if (filename == this->defaultFilename_)
            ThrowException(General, "KeyBinderManager: Cannot unload the default file");
        if (filename == this->currentBinder_->getBindingsFilename())
        {
            // unloading current file --> set default file
            this->setCurrent(this->defaultFilename_);
        }
        std::map<std::string, KeyBinder*>::iterator it = this->binders_.find(filename);
        if (it != this->binders_.end())
        {
            delete it->second;
            this->binders_.erase(it);
        }
    }

    KeyBinder* KeyBinderManager::get(const std::string& name)
    {
        this->load(name);
        return this->binders_[name];
    }

    InputHandler* KeyBinderManager::getCurrentAsHandler()
    {
        return this->getCurrent();
    }

    InputHandler* KeyBinderManager::getDefaultAsHandler()
    {
        return this->getDefault();
    }

    InputHandler* KeyBinderManager::getAsHandler(const std::string& name)
    {
        return this->get(name);
    }

    void KeyBinderManager::keybindInternal(const std::string& command, bool bTemporary)
    {
        if (!this->bBinding_)
        {
            COUT(0) << "Press any button/key or move a mouse/joystick axis" << std::endl;
            KeyDetector::getInstance().setCallback(callbackFunction_);
            InputManager::getInstance().enterState("detector");
            this->command_ = command;
            this->bTemporary_ = bTemporary;
            this->bBinding_ = true;
        }
        // else: We're still in a keybind command. Ignore this call.
    }

    // Gets called by the KeyDetector (registered with a Functor)
    void KeyBinderManager::callback(const std::string& keyName)
    {
        if (this->bBinding_)
        {
            COUT(0) << "Binding string \"" << command_ << "\" on key '" << keyName << "'" << std::endl;
            this->currentBinder_->setBinding(command_, keyName, bTemporary_);
            InputManager::getInstance().leaveState("detector");
            this->bBinding_ = false;
        }
        // else: A key was probably pressed within the same tick, ignore it.
    }
}
