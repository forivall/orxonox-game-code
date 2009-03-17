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

#include "OrxonoxStableHeaders.h"
#include "GametypeStatus.h"

#include <OgreTextAreaOverlayElement.h>

#include "core/CoreIncludes.h"
#include "util/Convert.h"
#include "objects/infos/GametypeInfo.h"
#include "objects/infos/PlayerInfo.h"
#include "objects/worldentities/ControllableEntity.h"
#include "objects/worldentities/pawns/Spectator.h"

namespace orxonox
{
    CreateFactory(GametypeStatus);

    GametypeStatus::GametypeStatus(BaseObject* creator) : OverlayText(creator)
    {
        RegisterObject(GametypeStatus);

        this->owner_ = 0;
    }

    GametypeStatus::~GametypeStatus()
    {
    }

    void GametypeStatus::tick(float dt)
    {
        SUPER(GametypeStatus, tick, dt);

        if (this->owner_ && this->owner_->getGametypeInfo() && this->owner_->getPlayer())
        {
            const GametypeInfo* gtinfo = this->owner_->getGametypeInfo();
            PlayerInfo* pinfo = this->owner_->getPlayer();

            if (!gtinfo->hasStarted() && !gtinfo->isStartCountdownRunning())
            {
                if (!pinfo->isReadyToSpawn())
                    this->setCaption("Press [Fire] to start the match");
                else
                    this->setCaption("Waiting for other players");
            }
            else if (!gtinfo->hasEnded())
            {
                if (gtinfo->isStartCountdownRunning())
                    this->setCaption(convertToString((int)ceil(gtinfo->getStartCountdown())));
                else if (this->owner_->isA(Class(Spectator)))
                    this->setCaption("Press [Fire] to respawn");
                else
                    this->setCaption("");
            }
            else
                this->setCaption("Game has ended");
        }
    }


    void GametypeStatus::changedOwner()
    {
        SUPER(GametypeStatus, changedOwner);

        this->owner_ = dynamic_cast<ControllableEntity*>(this->getOwner());
    }
}
