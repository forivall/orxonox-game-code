/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
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
    @file Identifier.cc
    @brief Implementation of the Identifier class.
*/

#include <ostream>

#include "Identifier.h"
#include "Factory.h"

namespace orxonox
{
    // ###############################
    // ###       Identifier        ###
    // ###############################
    int Identifier::hierarchyCreatingCounter_s = 0; // Set the static member variable hierarchyCreatingCounter_s to zero (this static member variable is ok: it's used in main(), not before)

    /**
        @brief Constructor: No factory, no object created, new ObjectList and a unique networkID.
    */
    Identifier::Identifier()
    {
        this->bCreatedOneObject_ = false;
        this->factory_ = 0;

        this->children_ = new std::list<const Identifier*>();
        this->directChildren_ = new std::list<const Identifier*>();

        // Use a static variable because the classID gets created before main() and that's why we should avoid static member variables
        static unsigned int classIDcounter_s = 0;
        this->classID_ = classIDcounter_s++;
    }

    /**
        @brief Destructor: Deletes the list containing the children.
    */
    Identifier::~Identifier()
    {
        delete this->children_;
        delete this->directChildren_;
    }

    /**
        @brief Initializes the Identifier with a list containing all parents of the class the Identifier belongs to.
        @param parents A list containing all parents
    */
    void Identifier::initialize(std::list<const Identifier*>* parents)
    {
        COUT(4) << "*** Identifier: Initialize " << this->name_ << "-Singleton." << std::endl;
        this->bCreatedOneObject_ = true;

        if (parents)
        {
            this->parents_ = (*parents);
            this->directParents_ = (*parents);

            // Iterate through all parents
            for (std::list<const Identifier*>::iterator it = parents->begin(); it != parents->end(); ++it)
            {
                // Tell the parent we're one of it's children
                (*it)->getChildrenIntern().insert((*it)->getChildrenIntern().end(), this);

                // Erase all parents of our parent from our direct-parent-list
                for (std::list<const Identifier*>::const_iterator it1 = (*it)->getParents().begin(); it1 != (*it)->getParents().end(); ++it1)
                {
                    // Search for the parent's parent in our direct-parent-list
                    for (std::list<const Identifier*>::iterator it2 = this->directParents_.begin(); it2 != this->directParents_.end(); ++it2)
                    {
                        if ((*it1) == (*it2))
                        {
                            // We've found a non-direct parent in our list: Erase it
                            this->directParents_.erase(it2);
                            break;
                        }
                    }
                }
            }

            // Now iterate through all direct parents
            for (std::list<const Identifier*>::iterator it = this->directParents_.begin(); it != this->directParents_.end(); ++it)
            {
                // Tell the parent we're one of it's direct children
                (*it)->getDirectChildrenIntern().insert((*it)->getDirectChildrenIntern().end(), this);
            }
        }
    }

    /**
        @brief Creates an object of the type the Identifier belongs to.
        @return The new object
    */
    BaseObject* Identifier::fabricate()
    {
        if (this->factory_)
        {
            return this->factory_->fabricate(); // We have to return a BaseObject, because we don't know the exact type.
        }
        else
        {
            COUT(1) << "An error occurred in Identifier.cc:" << std::endl;
            COUT(1) << "Error: Cannot fabricate an object of type '" << this->name_ << "'. Class has no factory." << std::endl;
            COUT(1) << "Aborting..." << std::endl;
            abort();
            return NULL;
        }
    }

    /**
        @brief Sets the network ID to a new value and changes the entry in the Factory.
        @param id The new network ID
    */
    void Identifier::setNetworkID(unsigned int id)
    {
        Factory::changeNetworkID(this, this->classID_, id);
        this->classID_ = id;
    }

    /**
        @brief Returns true, if the Identifier is at least of the given type.
        @param identifier The identifier to compare with
    */
    bool Identifier::isA(const Identifier* identifier) const
    {
        return (identifier == this || this->identifierIsInList(identifier, this->parents_));
    }

    /**
        @brief Returns true, if the Identifier is exactly of the given type.
        @param identifier The identifier to compare with
    */
    bool Identifier::isExactlyA(const Identifier* identifier) const
    {
        return (identifier == this);
    }

    /**
        @brief Returns true, if the assigned identifier is a child of the given identifier.
        @param identifier The identifier to compare with
    */
    bool Identifier::isChildOf(const Identifier* identifier) const
    {
        return this->identifierIsInList(identifier, this->parents_);
    }

    /**
        @brief Returns true, if the assigned identifier is a direct child of the given identifier.
        @param identifier The identifier to compare with
    */
    bool Identifier::isDirectChildOf(const Identifier* identifier) const
    {
        return this->identifierIsInList(identifier, this->directParents_);
    }

    /**
        @brief Returns true, if the assigned identifier is a parent of the given identifier.
        @param identifier The identifier to compare with
    */
    bool Identifier::isParentOf(const Identifier* identifier) const
    {
        return this->identifierIsInList(identifier, *this->children_);
    }

    /**
        @brief Returns true, if the assigned identifier is a direct parent of the given identifier.
        @param identifier The identifier to compare with
    */
    bool Identifier::isDirectParentOf(const Identifier* identifier) const
    {
        return this->identifierIsInList(identifier, *this->directChildren_);
    }

    /**
        @brief Searches for a given identifier in a list and returns whether the identifier is in the list or not.
        @param identifier The identifier to look for
        @param list The list
        @return True = the identifier is in the list
    */
    bool Identifier::identifierIsInList(const Identifier* identifier, const std::list<const Identifier*>& list)
    {
        for (std::list<const Identifier*>::const_iterator it = list.begin(); it != list.end(); ++it)
            if (identifier == (*it))
                return true;

        return false;
    }

    std::ostream& operator<<(std::ostream& out, const std::list<const Identifier*>& list)
    {
        for (std::list<const Identifier*>::const_iterator it = list.begin(); it != list.end(); ++it)
            out << (*it)->getName() << " ";

        return out;
    }
}
