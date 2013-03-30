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

/**
    @file
    @ingroup Object
*/

#ifndef _Context_H__
#define _Context_H__

#include "core/CorePrereqs.h"

#include <vector>

#include "ObjectListBase.h"

namespace orxonox
{
    class _CoreExport Context
    {
        public:
            static Context* getRootContext();

            Context(Context* context);
            virtual ~Context();

            inline Context* getParentContext() const
                { return this->parentContext_; }

            ObjectListBase* getObjectList(const Identifier* identifier);

            template <class T>
            inline ObjectListBase* getObjectList()
                { return this->getObjectList(ClassIdentifier<T>::getIdentifier()); }

            template <class T>
            inline void addObject(T* object)
            {
                ObjectListBaseElement* element = this->getObjectList<T>()->add(object);
                object->elements_.push_back(element);
                if (this->getParentContext())
                    this->getParentContext()->addObject(object);
            }

        private:
            Context* parentContext_;
            std::vector<ObjectListBase*> objectLists_;
    };
}

#endif /* _Context_H__ */
