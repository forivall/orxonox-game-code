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
 *      Yuning Chai
 *   Co-authors:
 *      Felix Schulthess
 *
 */


#ifndef _HUD_H__
#define _HUD_H__

#include "OrxonoxPrereqs.h"

#include <OgrePrerequisites.h>
#include <OgreTextAreaOverlayElement.h>
#include "core/BaseObject.h"
#include "objects/Tickable.h"
#include "util/Math.h"
#include "OverlayElementFactories.h"
#include "tools/WindowEventListener.h"

namespace orxonox
{
    class HUDBar;
    class HUDOverlay;

    class _OrxonoxExport HUD : public BaseObject, public TickableReal, public WindowEventListener
    {
      public:
        HUD();
        ~HUD();

        virtual void XMLPort(Element& xmlElement, XMLPort::Mode mode);

        virtual void tick(float);

        void resize();
        void addRadarObject(WorldEntity* object, const ColourValue& colour = ColourValue(0.5, 0.5, 0.5, 1));
        void removeRadarObject(WorldEntity* object);
        void setFPS();

        inline std::list<RadarObject*>& getRadarObjects()
            { return this->roSet_; }

        static HUD& getSingleton();

        static void setEnergy(float value);
        static void cycleNavigationFocus();
        static void releaseNavigationFocus();
        static void toggleVisibility(const std::string& name);

      private:
        HUD(const HUD& instance);

        void addHUDElement(HUDOverlay* element);
        HUDOverlay* getHUDElement(unsigned int index);

        void windowResized(int newWidth, int newHeight);

        std::map<std::string, HUDOverlay*> hudElements_;

        std::list<RadarObject*> roSet_;

        RadarOverlayElementFactory radarOverlayElementFactory_;

        Ogre::Overlay* orxonoxHUD_;
        Ogre::OverlayContainer* container_;
        Ogre::TextAreaOverlayElement* fpsText_;
        Ogre::TextAreaOverlayElement* rTRText_;
        HUDBar* energyBar_;
        BarOverlayElement* speedoBar_;
        RadarOverlayElement* radar_;
        Navigation* nav_;

        bool showFPS_;
        bool showRenderTime_;

        static HUD* instance_s;
    };
}

#endif /* _HUD_H__ */
