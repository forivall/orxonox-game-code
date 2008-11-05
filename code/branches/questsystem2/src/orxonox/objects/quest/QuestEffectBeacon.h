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
 *      Damian 'Mozork' Frick
 *   Co-authors:
 *      ...
 *
 */

#ifndef _QuestEffectBeacon_H__
#define _QuestEffectBeacon_H__

#include "OrxonoxPrereqs.h"

#include "orxonox/objects/worldentities/PositionableEntity.h"

namespace QuestEffectBeaconStatus
{

    enum Enum
    {
        inactive,
        active
    };
    
}

namespace orxonox {

    /**
    @brief
        
    @author
        Damian 'Mozork' Frick
    */
    class _OrxonoxExport QuestEffectBeacon : public PositionableEntity
    {
	public:
	    QuestEffectBeacon(BaseObject* creator);
	    virtual ~QuestEffectBeacon();
	    
	    virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
	    
	    bool execute(ControllableEntity* player);
	    
	    bool isActive(void);
	    
	protected:
            std::list<QuestEffect*> effects_;
            int times_; //!< Number of times the beacon can be exectued.
            QuestEffectBeaconStatus::Enum status_;
            
            bool decrementTimes(void);
            
            bool setTimes(const int & n);
            bool addEffect(QuestEffect* effect);
            
            inline const int & getTimes(void) const
                { return this->times_; }
            const QuestEffect* getEffects(unsigned int index) const;
    
    };

}

#endif /* _QuestEffectBeacon_H__ */
