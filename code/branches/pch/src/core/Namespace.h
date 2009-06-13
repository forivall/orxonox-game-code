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

#ifndef _Namespace_H__
#define _Namespace_H__

#include "CorePrereqs.h"

#include <set>
#include <string>
#include "XMLIncludes.h"
#include "BaseObject.h"

namespace orxonox
{
    class Namespace : public BaseObject
    {
        public:
            Namespace(BaseObject* creator);
            virtual ~Namespace();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);

            void loadObjects(BaseObject* object);
            const BaseObject* saveObjects(unsigned int index) const;

            void setAutogenerated(bool bAutogenerated)
                { this->bAutogeneratedFileRootNamespace_ = bAutogenerated; }
            bool isAutogenerated() const
                { return this->bAutogeneratedFileRootNamespace_; }

            void setOperator(const std::string& op)
                { this->operator_ = op; }
            const std::string& getOperator() const
                { return this->operator_; }

            void setRoot(bool bRoot)
                { this->bRoot_ = bRoot; }

            bool includes(const Namespace* ns) const;
            bool isIncludedIn(const Namespace* ns) const { return ns->includes(this); }

            std::string toString() const;
            std::string toString(const std::string& indentation) const;

        private:
            std::set<NamespaceNode*> representingNamespaces_;
            bool bAutogeneratedFileRootNamespace_;
            bool bRoot_;
            std::string operator_;
    };
}

#endif /* _Namespace_H__ */
