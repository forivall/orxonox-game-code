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
 *      Jan Bernegger
 *   Co-authors:
 *      ...
 *
 */

#ifndef _MeshLodInformation_H__
#define _MeshLodInformation_H__

#include "OrxonoxPrereqs.h"

#include <string>
#include "tools/Mesh.h"
#include "worldentities/StaticEntity.h"

namespace orxonox
{
    class _OrxonoxExport MeshLodInformation : public BaseObject
    {
		private:
			void setLodLevel(unsigned int lodLevel);
			void setMeshSource(std::string meshSource);
			std::string getMeshSource();
			std::string meshSource_;
			int lodLevel_;
		
        public:
            MeshLodInformation(BaseObject* creator);
            virtual ~MeshLodInformation();
            
			int getLodLevel();
            std::string getMeshName();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
    };
}

#endif /* _MeshLodInformation_H__ */
