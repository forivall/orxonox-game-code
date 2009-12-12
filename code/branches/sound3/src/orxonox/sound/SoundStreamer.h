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
 *      Erwin 'vaiursch' Herrsche
 *   Co-authors:
 *
 */

#ifndef _SoundStreamer_H__
#define _SoundStreamer_H__

#include "OrxonoxPrereqs.h"

#include <string>
#include <OgreDataStream.h>
#include "core/OrxonoxClass.h"

namespace orxonox
{
    class _OrxonoxExport SoundStreamer : virtual public OrxonoxClass
    {
    public:
        SoundStreamer(ALuint audioSource);
        ~SoundStreamer();
        bool stream(DataStreamPtr dataStream);
        static void streamcallable(ALuint audioSource_, DataStreamPtr dataStream);

    private:
        ALuint audioSource_;
        ALuint buffers[3];
        boost::thread::id threadId;
    }

}

#endif /* _SoundStreamer_H__ */
