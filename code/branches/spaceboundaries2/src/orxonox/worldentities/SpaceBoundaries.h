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
 *      Maurus Kaufmann
 *   Co-authors:
 *      ...
 *
 */

#ifndef _SpaceBoundaries_H__
#define _SpaceBoundaries_H__


#include "core/CoreIncludes.h"
#include "core/WeakPtr.h"
#include "tools/interfaces/Tickable.h"
#include "interfaces/RadarViewable.h"
#include "worldentities/StaticEntity.h"
#include "worldentities/WorldEntity.h"

#include <string>
#include <list>
#include <vector>

namespace orxonox
{

/**
@brief SpaceBoundaries gives level creators the possibility to bar Pawns from leaving a defined area (until now this area is a ball).

       Some attributes can/should be defined in the XML-File:
       - 'position' : absolute position of the object of SpaceBoundaries in the level (usually 0,0,0) 
       - 'maxDistance' : defines the area, where a pawn is allowed to be (radius of a ball).
       - 'warnDistance' : If the distance between the pawn of the human player and 'position' is bigger than 'warnDistance', a message is displayed to
                          inform the player that he'll soon be leaving the allowed area. (not implemented yet!)
       - 'showDistance' : If the distance between the pawn and the boundary of the allowed area is smaller than 'showDistance', the boundary is shown. 
       - 'reactionMode' : Integer-Value. Defines what effect appears if a space ship has crossed the boundaries.
                            0: Reflect the space ship (default).
                            1: Decrease Health of the space ship after having left the allowed area.
                            2: Inverted Version of 0. Prohibit to fly INTO a defined area.
       - 'healthDecrease' : a measure to define how fast the health of a pawn should decrease after leaving the allowed area (unnecessary if 'reactionMode' == 0).
                            Recommended values: 0.1 (slow health decrease) to 5 (very fast health decrease)

Follow http://www.orxonox.net/wiki/SpaceBoundaries to get some further information.

Examples:
Two examples how one could include SpaceBoundaries in the XML-File. The first one uses reflection, the second one health decrease.
@code
<SpaceBoundaries position="0,0,0" maxDistance="1000" warnDistance="800" showDistance="100" reactionMode="0" />
@endcode

@code
<SpaceBoundaries position="0,0,0" maxDistance="1000" warnDistance="800" showDistance="100" reactionMode="1" healthDecrease="0.2" />
@endcode
*/

    class _OrxonoxExport SpaceBoundaries : public StaticEntity, public Tickable
    {
        public:
            SpaceBoundaries(BaseObject* creator);
            ~SpaceBoundaries();
            
            void setMaxDistance(float r);
            float getMaxDistance();
            
            void setWarnDistance(float r);
            float getWarnDistance();
            
            void setShowDistance(float r);
            float getShowDistance();
            
            void setHealthDecrease(float amount);
            float getHealthDecrease();
            
            void setReaction(int mode);
            int getReaction();

            void XMLPort(Element& xmlelement, XMLPort::Mode mode);
            
            void tick(float dt);

        private:
            struct billboardAdministration{ bool usedYet; Billboard* billy; };
            
            // Variabeln::
            std::list<WeakPtr<Pawn> > pawnsIn_; //!< List of the pawns that this instance of SpaceBoundaries has to handle.
            
            std::vector<billboardAdministration> billboards_;
        
            int reaction_; //!< Werte: 0, 1, 2. 
                           //!< 0: Reflektion an Boundaries (Standard).
                           //!< 1: Health-Abzug-Modus.
                           //!< 2: Invertierte Version von 0. Verbiete es, in ein Gebiet hinein zu fliegen.
            float maxDistance_; //!< maximal zulaessige Entfernung von 'this->getPosition()'.
            float warnDistance_; //!< Entfernung von 'this->getPosition()', ab der eine Warnung angezeigt wird, dass man bald das zulaessige Areal verlaesst.
            float showDistance_; //!< Definiert, wann die Grenzen visualisiert werden sollen. ("Entfernung von 'this->maxDistance_'")
            
            float healthDecrease_; //!< Mass fuer die Anzahl Health-Points, die nach ueberschreiten der Entfernung 'maxDistance_' von 'this->getPosition()' abgezogen werden.
                                   //!< Empfohlene Werte: 0.1 (langsame Health-Verminderung) bis 5 (sehr schnelle Health-Verminderung).
                                   //!< Hat nur eine Bedeutung, falls 'this->reaction_' 1 (eins) ist. 
            
            
            RadarViewable* centerRadar_; //!< Repraesentation von SpaceBoundaries auf dem Radar.
        
        
            // Funktionen::
            float computeDistance(WorldEntity *item); //!< Auf den Mittelpunkt 'this->getPosition()' bezogen.
            void displayWarning(const std::string warnText); //!< Einen Warntext auf dem Bildschirm ausgeben. (noch nicht implementiert, TODO)
            void displayBoundaries(Pawn *item);
            void conditionalBounceBack(Pawn *item, float currentDistance, float dt);
            void bounceBack(Pawn *item, Vector3 *normal, Vector3 *velocity);
            bool isHumanPlayer(Pawn *item);
            
            void checkWhoIsIn(); //!< Update the list 'pawnsIn_'.
            
            void positionBillboard(const Vector3 position); //!< Display a Billboard at the position 'position'.
            void setBillboardOptions(Billboard *billy);
            void removeAllBillboards(); //!< Hide all elements of '*billboard_' and set their attribute 'usedYet' to 0.
            
    };
}

#endif /* _SpaceBoundaries_H__ */
