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
 *       Erwin 'vaiursch' Herrsche
 *   Co-authors:
 *      ...
 *
 */

#include "orxonox/objects/worldentities/WorldEntity.h"
#include "util/Math.h"
#include "SoundManager.h"
#include "SoundBase.h"

namespace orxonox 
{
    SoundBase::SoundBase()
    {
        this->source_ = 0;
        this->buffer_ = 0;
        this->entity_ = NULL;
    }
    SoundBase::SoundBase(WorldEntity* entity)
    {
        this->source_ = 0;
        this->buffer_ = 0;
        this->entity_ = entity;
    }

    void SoundBase::attachToEntity(WorldEntity* entity)
    {
        this->entity_ = entity;
        this->update();
    }

    void SoundBase::update() {
        if(this->entity_ != NULL && alIsSource(this->source_)) {
            Vector3 pos = this->entity_->getPosition();
            alSource3f(this->source_, AL_POSITION, pos.x, pos.y, pos.z);
            ALenum error = alGetError();
            if(error == AL_INVALID_VALUE)
                COUT(2) << "OpenAL: Invalid sound position" << std::endl;

            Vector3 vel = this->entity_->getVelocity();
            alSource3f(this->source_, AL_VELOCITY, vel.x, vel.y, vel.z);
            error = alGetError();
            if(error == AL_INVALID_VALUE)
                COUT(2) << "OpenAL: Invalid sound position" << std::endl;

            Quaternion orient = this->entity_->getOrientation();
            Vector3 at = orient.zAxis();
            alSource3f(this->source_, AL_DIRECTION, at.x, at.y, at.z);
            error = alGetError();
            if(error == AL_INVALID_VALUE)
                COUT(2) << "OpenAL: Invalid sound position" << std::endl;
        }
    }

    void SoundBase::play(bool loop) {
        if(alIsSource(this->source_)) {
            if(loop)
                alSourcei(this->source_, AL_LOOPING, AL_TRUE);
            else
                alSourcei(this->source_, AL_LOOPING, AL_FALSE);
            alSourcePlay(this->source_);
        }
    }

    void SoundBase::stop() {
        if(alIsSource(this->source_)) {
            alSourceStop(this->source_);
        }
    }

    void SoundBase::pause() {
        if(alIsSource(this->source_)) {
            alSourcePause(this->source_);
        }
    }

    bool SoundBase::isPlaying() {
        if(alIsSource(this->source_)) {
            return getSourceState() == AL_PLAYING;
        }
        return false;
    }

    bool SoundBase::isPaused() {
        if(alIsSource(this->source_)) {
            return getSourceState() == AL_PAUSED;
        }
        return true;
    }

    bool SoundBase::isStopped() {
        if(alIsSource(this->source_)) {
            return getSourceState() == AL_INITIAL || getSourceState() == AL_STOPPED;
        }
        return true;
    }

    bool SoundBase::loadFile(std::string filename) {
        COUT(3) << "OpenAL ALUT: loading file " << filename << std::endl;
        this->buffer_ = alutCreateBufferFromFile(filename.c_str());
        if(this->buffer_ == AL_NONE) {
            COUT(2) << "OpenAL ALUT: " << alutGetErrorString(alutGetError()) << std::endl;
            return false;
        }

        alGenSources(1, &this->source_);
        alSourcei(this->source_, AL_BUFFER, this->buffer_);
        if(alGetError() != AL_NO_ERROR) {
            COUT(2) << "OpenAL: Error loading sample file" << std::endl;
            return false;
        }
        return true;
    }

    ALint SoundBase::getSourceState() {
        ALint state;
        alGetSourcei(this->source_, AL_SOURCE_STATE, &state);
        return state;
    }
} // namespace: orxonox
