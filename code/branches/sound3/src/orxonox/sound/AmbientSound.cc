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

#include "AmbientSound.h"

#include "core/CoreIncludes.h"
#include "core/EventIncludes.h"
#include "core/XMLPort.h"
#include "SoundManager.h"

namespace orxonox
{
    CreateFactory(AmbientSound);

    AmbientSound::AmbientSound(BaseObject* creator)
        : BaseObject(creator)
    {
        RegisterObject(AmbientSound);
    }

    AmbientSound::~AmbientSound()
    {
    }

    void AmbientSound::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(AmbientSound, XMLPort, xmlelement, mode);
        XMLPortParamExtern(AmbientSound, BaseSound, this, "source", setSource, getSource, xmlelement, mode);
        XMLPortParamExtern(AmbientSound, BaseSound, this, "loop", setLoop, getLoop, xmlelement, mode);
        XMLPortParamExtern(AmbientSound, BaseSound, this, "playOnLoad", setPlayOnLoad, getPlayOnLoad, xmlelement, mode);
    }

    void AmbientSound::XMLEventPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(AmbientSound, XMLEventPort, xmlelement, mode);
        XMLPortEventState(AmbientSound, BaseObject, "play", play, xmlelement, mode);
    }

    void AmbientSound::play()
    {
        COUT(3) << this->getSource() << ": Playing" << std::endl;
        SoundManager::getInstance().registerAmbientSound(this);
        SUPER(AmbientSound, play);
    }

    void AmbientSound::replay()
    {
        SUPER(AmbientSound, play);
    }

    void AmbientSound::stop()
    {
        SUPER(AmbientSound, stop);
        SoundManager::getInstance().unregisterAmbientSound(this);
    }

    void AmbientSound::pause()
    {
        SUPER(AmbientSound, pause);
    }

    void AmbientSound::setSource(const std::string& source)
    {
        if(source.find('/') == std::string.npos)
        {
            std::string filePath = SoundManager::getInstance().getAmbientPath(source);
            if(!(filePath.empty()))
            {
                BaseSound::setSource(filePath);
                return;
            }
        }
        COUT(3) << source << ": Not a valid name! Ambient sound will not change." << std::endl;       
    }

    void AmbientSound::changedActivity() 
    {
        COUT(3) << this->getSource() << ": ChangedActivity: " << this->isActive() << std::endl;
        SUPER(AmbientSound, changedActivity);
        if(this->isActive())
        {
            this->play();
        }
        else 
        {
            this->stop();
        }
    }

}
