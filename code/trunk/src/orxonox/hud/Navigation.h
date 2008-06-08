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
 *      Felix Schulthess
 *   Co-authors:
 *      ...
 *
 */

#ifndef _Navigation_H__
#define _Navigation_H__

#include "OrxonoxPrereqs.h"

#include <OgrePrerequisites.h>
#include <OgreTextAreaOverlayElement.h>
#include <OgrePanelOverlayElement.h>

namespace orxonox
{
    class _OrxonoxExport Navigation
    {
      public:
        Navigation(Ogre::OverlayContainer* container);
        Navigation(Ogre::OverlayContainer* container, RadarObject* focus);
        ~Navigation();

        void update();
        void cycleFocus();
        float getDist2Focus() const;

        inline RadarObject* getFocus() const
            { return this->focus_; }
        void releaseFocus();

      private:
        void init();
        void updateMarker();
        void updateFocus();

        Ogre::OverlayContainer* container_;
        Ogre::PanelOverlayElement* navMarker_;      // the panel used to show the arrow
        Ogre::PanelOverlayElement* aimMarker_;
        Ogre::TextAreaOverlayElement* navText_;     // displaying distance
        std::list<RadarObject*>::iterator it_;
        RadarObject* focus_;                        // next pointer of linked list
  };
}

#endif /* _Navigation_H__ */
