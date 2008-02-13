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

/*!
    @file ClassTreeMask.cc
    @brief Implementation of the ClassTreeMask, ClassTreeMaskNode and ClassTreeMaskIterator classes.
*/

#include "ClassTreeMask.h"
#include "Identifier.h"
#include "BaseObject.h"

namespace orxonox
{
    // ###############################
    // ###    ClassTreeMaskNode    ###
    // ###############################
    /**
        @brief Constructor: Creates the node, sets the subclass and the rule.
        @param subclass The subclass the rule refers to
        @param bIncluded The rule: included (true) or excluded (false)
    */
    ClassTreeMaskNode::ClassTreeMaskNode(const Identifier* subclass, bool bIncluded)
    {
        this->subclass_ = subclass;
        this->bIncluded_ = bIncluded;
    }

    /**
        @brief Destructor: Deletes all subnodes.
    */
    ClassTreeMaskNode::~ClassTreeMaskNode()
    {
        // Go through the list of all subnodes and delete them
        for (std::list<ClassTreeMaskNode*>::iterator it = this->subnodes_.begin(); it != this->subnodes_.end(); )
            delete (*(it++));
    }

    /**
        @brief Sets the rule for the node to "included".
    */
    void ClassTreeMaskNode::include()
    {
        this->bIncluded_ = true;
    }

    /**
        @brief Sets the rule for the node to "excluded".
    */
    void ClassTreeMaskNode::exclude()
    {
        this->bIncluded_ = false;
    }

    /**
        @brief Sets the rule for the node to a given value.
        @param bIncluded The rule: included (true) or excluded (false)
    */
    void ClassTreeMaskNode::setIncluded(bool bIncluded)
    {
        this->bIncluded_ = bIncluded;
    }

    /**
        @brief Adds a new subnode to the list of subnodes.
        @param subnode The new subnode
    */
    void ClassTreeMaskNode::addSubnode(ClassTreeMaskNode* subnode)
    {
        this->subnodes_.insert(this->subnodes_.end(), subnode);
    }

    /**
        @brief Tells if the rule is "included" or not.
        @return The rule: true = included, false = excluded
    */
    bool ClassTreeMaskNode::isIncluded() const
    {
        return this->bIncluded_;
    }

    /**
        @brief Tells if the rule is "excluded" or not.
        @return The inverted rule: true = excluded, false = included
    */
    bool ClassTreeMaskNode::isExcluded() const
    {
        return (!this->bIncluded_);
    }

    /**
        @brief Returns the Identifier of the class the rule refers to.
        @return The Identifier representing the class
    */
    const Identifier* ClassTreeMaskNode::getClass() const
    {
        return this->subclass_;
    }


    // ###############################
    // ###  ClassTreeMaskIterator  ###
    // ###############################
    /**
        @brief Constructor: Initializes the iterator by creating a helper-list with the root-node and putting it to the stack.
        @param node The root-node
    */
    ClassTreeMaskIterator::ClassTreeMaskIterator(ClassTreeMaskNode* node)
    {
        // Create a list and put the root-note into it
        std::list<ClassTreeMaskNode*>::iterator it = this->rootlist_.insert(this->rootlist_.end(), node);

        // Put the iterator to the only element of the list and the corresponding end()-iterator on the stack
        this->nodes_.push(std::pair<std::list<ClassTreeMaskNode*>::iterator, std::list<ClassTreeMaskNode*>::iterator>(it, this->rootlist_.end()));
    }

    /**
        @brief Destructor: Does nothing.
    */
    ClassTreeMaskIterator::~ClassTreeMaskIterator()
    {
    }

    /**
        @brief Iterates through the rule-tree.
        @return A reference to the iterator itself
    */
    ClassTreeMaskIterator& ClassTreeMaskIterator::operator++()
    {
        // Check if the actual node has subnodes
        if ((*this->nodes_.top().first)->subnodes_.begin() != (*this->nodes_.top().first)->subnodes_.end())
        {
            // Yes it has: Push an iterator, pointing at the first subnode, on the stack
            this->nodes_.push(std::pair<std::list<ClassTreeMaskNode*>::iterator, std::list<ClassTreeMaskNode*>::iterator>((*this->nodes_.top().first)->subnodes_.begin(), (*this->nodes_.top().first)->subnodes_.end()));
        }
        else
        {
            // No subnodes, meaning we reached a leaf: Go to the next node
            do
            {
                // Iterate one step in the current list
                ++this->nodes_.top().first;

                // Check if we reached the end of the list (the second item in the stored pair always represents the end)
                if (this->nodes_.top().first == this->nodes_.top().second)
                {
                    // Yes we've reached the end: Pop the list-iterator from the stack
                    this->nodes_.pop();

                    // Continue with the loop, meaning: Try to iterate through the previous list
                    continue;
                }

                // If we reached this point, we aren't yet at the end of the list: Leave the loop
                break;
            } while (!this->nodes_.empty()); // Stop looping if the stack is empty, meaning: We've iterated to the end
        }

        // Finally return a reference to the iterator itself
        return *this;
    }

    /**
        @brief Returns a pointer to the ClassTreeMaskNode whereon the iterator points.
        @return The pointer to the node
    */
    ClassTreeMaskNode* ClassTreeMaskIterator::operator*() const
    {
        return (*this->nodes_.top().first);
    }

    /**
        @brief Returns a pointer to the ClassTreeMaskNode whereon the iterator points.
        @return The pointer to the node
    */
    ClassTreeMaskNode* ClassTreeMaskIterator::operator->() const
    {
        return (*this->nodes_.top().first);
    }

    /**
        @brief Returns true if the stack is empty, meaning we've reached the end of the tree.
        @return True if we've reached the end of the tree
    */
    ClassTreeMaskIterator::operator bool()
    {
        return (!this->nodes_.empty());
    }

    /**
        @brief Compares the current node with the given one and returns true if they match.
        @param compare The node to compare with
        @return The result of the comparison (true if they match)
    */
    bool ClassTreeMaskIterator::operator==(ClassTreeMaskNode* compare)
    {
        if (!this->nodes_.empty())
            return ((*this->nodes_.top().first) == compare);
        else
            return (compare == 0);
    }

    /**
        @brief Compares the current node with the given one and returns true if they don't match.
        @param compare The node to compare with
        @return The result of the comparison (true if they don't match)
    */
    bool ClassTreeMaskIterator::operator!=(ClassTreeMaskNode* compare)
    {
        if (!this->nodes_.empty())
            return ((*this->nodes_.top().first) != compare);
        else
            return (compare != 0);
    }


    // ###############################
    // ###      ClassTreeMask      ###
    // ###############################
    /**
        @brief Constructor: Adds the root-node of the tree with the first rule ("include everything").
    */
    ClassTreeMask::ClassTreeMask()
    {
        this->root_ = new ClassTreeMaskNode(ClassManager<BaseObject>::getIdentifier("BaseObject"), true);
    }

    /**
        @brief Copyconstructor: Adds the root-node of the tree with the first rule ("include everything") and adds all rules from the other mask.
        @param other The other mask
    */
    ClassTreeMask::ClassTreeMask(const ClassTreeMask& other)
    {
        this->root_ = new ClassTreeMaskNode(ClassManager<BaseObject>::getIdentifier("BaseObject"), true);
        for (ClassTreeMaskIterator it = other.root_; it; ++it)
            this->add(it->getClass(), it->isIncluded());
    }

    /**
        @brief Destructor: Deletes the root node (which will delete all subnodes too).
    */
    ClassTreeMask::~ClassTreeMask()
    {
        delete this->root_;
    }

    /**
        @brief Adds a new "include" rule for a given subclass to the mask.
        @param subclass The subclass
    */
    void ClassTreeMask::include(const Identifier* subclass)
    {
        this->add(subclass, true);
    }

    /**
        @brief Adds a new "exclude" rule for a given subclass to the mask.
        @param subclass The subclass
    */
    void ClassTreeMask::exclude(const Identifier* subclass)
    {
        this->add(subclass, false);
    }

    /**
        @brief Adds a new rule for a given subclass to the mask.
        @param subclass The subclass
        @param bInclude The rule: include (true) or exclude (false)
    */
    void ClassTreeMask::add(const Identifier* subclass, bool bInclude)
    {
        this->add(this->root_, subclass, bInclude);
    }

    /**
        @brief Adds a new rule for a given subclass to a node of the internal rule-tree (recursive function).
        @param node The node
        @param subclass The subclass
        @param bInclude The rule: include (true) or exclude (false)
    */
    void ClassTreeMask::add(ClassTreeMaskNode* node, const Identifier* subclass, bool bInclude)
    {
        // Check if the current node contains exactly the subclass we want to add
        if (subclass == node->getClass())
        {
            // We're at the right place, just change the mask and return
            node->setIncluded(bInclude);
            return;
        }
        else
        {
            // Search for an already existing node, containing the subclass we want to add
            for (std::list<ClassTreeMaskNode*>::iterator it = node->subnodes_.begin(); it != node->subnodes_.end(); ++it)
            {
                if (subclass->isA((*it)->getClass()))
                {
                    // We've found an existing node -> delegate the work with a recursive function-call and return
                    this->add(*it, subclass, bInclude);
                    return;
                }
            }

            // There is no existing node satisfying our needs -> we create a new node
            ClassTreeMaskNode* newnode = new ClassTreeMaskNode(subclass, bInclude);

            // Search for nodes that should actually be subnodes of our new node
            for (std::list<ClassTreeMaskNode*>::iterator it = node->subnodes_.begin(); it != node->subnodes_.end(); )
            {
                if ((*it)->getClass()->isChildOf(subclass))
                {
                    // We've found a subnode: add it to the new node an erase it from the current node
                    newnode->addSubnode(*it);
                    node->subnodes_.erase(it++);
                }
                else
                {
                    ++it;
                }
            }

            // Finally add the new node as a subnode to the current node
            node->addSubnode(newnode);
        }
    }

    /**
        @brief Resets the mask to "include everything".
    */
    void ClassTreeMask::reset()
    {
        delete this->root_;
        this->root_ = new ClassTreeMaskNode(ClassManager<BaseObject>::getIdentifier("BaseObject"), true);
    }

    /**
        @brief Tells if a given subclass is included or not.
        @param subclass The subclass
        @return Included (true) or excluded (false)
    */
    bool ClassTreeMask::isIncluded(const Identifier* subclass) const
    {
        return this->isIncluded(this->root_, subclass);
    }

    /**
        @brief Tells if a given subclass of a node in the rule-tree is included or not (recursive function).
        @param node The node
        @param subclass The subclass
        @return Included (true) or excluded (false)
    */
    bool ClassTreeMask::isIncluded(ClassTreeMaskNode* node, const Identifier* subclass) const
    {
        // Check if the searched subclass is of the same type as the class in the current node or a derivative
        if (subclass->isA(node->getClass()))
        {
            // Check for the special case
            if (subclass == node->getClass())
                return node->isIncluded();

            // Go through the list of subnodes and look for a node containing the searched subclass and delegate the request by a recursive function-call.
            for (std::list<ClassTreeMaskNode*>::iterator it = node->subnodes_.begin(); it != node->subnodes_.end(); ++it)
                if (subclass->isA((*it)->getClass()))
                    return isIncluded(*it, subclass);

            // There is no subnode containing our class -> the rule of the current node takes in effect
            return node->isIncluded();
        }
        else
        {
            // The class is not included in the mask: return false
            return false;
        }
    }

    /**
        @brief Tells if a given subclass is excluded or not.
        @param subclass The subclass
        @return The inverted rule: Excluded (true) or included (false)
    */
    bool ClassTreeMask::isExcluded(const Identifier* subclass) const
    {
        return (!this->isIncluded(subclass));
    }

    /**
        @brief Removes all unneeded rules that don't change the information of the mask.
    */
    void ClassTreeMask::clean()
    {
        this->clean(this->root_);
    }

    /**
        @brief Removes all unneded rules that don't change the information of a node of a mask (recursive function).
        @param node The node
    */
    void ClassTreeMask::clean(ClassTreeMaskNode* node)
    {
        // Iterate through all subnodes of the given node
        for (std::list<ClassTreeMaskNode*>::iterator it = node->subnodes_.begin(); it != node->subnodes_.end(); )
        {
            // Recursive function-call: Clean the subnode
            this->clean(*it);

            // Now check if the subnode contains the same rule as the current node
            if ((*it)->isIncluded() == node->isIncluded())
            {
                // It does: Move all subnodes of the redundant subnode to the current node
                node->subnodes_.insert(node->subnodes_.end(), (*it)->subnodes_.begin(), (*it)->subnodes_.end());
                (*it)->subnodes_.clear();

                // Remove the redundant subnode from the current node
                node->subnodes_.erase(it++);
            }
            else
            {
                // The subnode is necessary: Move on to the next subnode
                ++it;
            }
        }
    }

    /**
        @brief Assignment operator: Adds all rules of the other mask.
        @param other The other mask
        @return A reference to the mask itself
    */
    ClassTreeMask& ClassTreeMask::operator=(const ClassTreeMask& other)
    {
        // Make a copy to avoid troubles with self-assignments (like A = A).
        ClassTreeMask temp(other);

        // Removes all current rules
        this->reset();

        // Copy all rules from the other mask
        for (ClassTreeMaskIterator it = temp.root_; it; ++it)
            this->add(it->getClass(), it->isIncluded());

        // Return a reference to the mask itself
        return (*this);
    }

    /**
        @brief Prefix operator + does nothing.
        @return A reference to the mask itself
    */
    ClassTreeMask& ClassTreeMask::operator+()
    {
        return (*this);
    }

    /**
        @brief Prefix operator - inverts the mask.
        @return The inverted mask
    */
    ClassTreeMask ClassTreeMask::operator-() const
    {
        return (!(*this));
    }

    /**
        @brief Adds two masks in the sense of a union (all classes that are included in at least one of the masks will be included in the resulting mask too).
        @param other The mask to unite with
        @return The union
    */
    ClassTreeMask ClassTreeMask::operator+(const ClassTreeMask& other) const
    {
        // Create a new mask
        ClassTreeMask newmask;

        // Add all nodes from the first mask, calculate the rule with the or-operation
        for (ClassTreeMaskIterator it = this->root_; it; ++it)
        {
            const Identifier* subclass = it->getClass();
            newmask.add(subclass, this->isIncluded(subclass) or other.isIncluded(subclass));
        }

        // Add all nodes from the second mask, calculate the rule with the or-operation
        for (ClassTreeMaskIterator it = other.root_; it; ++it)
        {
            const Identifier* subclass = it->getClass();
            newmask.add(subclass, this->isIncluded(subclass) or other.isIncluded(subclass));
        }

        // Drop all redundant rules
        newmask.clean();

        // Return the new mask
        return newmask;
    }

    /**
        @brief Intersects two masks (only classes that are included in both masks will be included in the resulting mask too).
        @param other The mask to intersect with
        @return The intersection
    */
    ClassTreeMask ClassTreeMask::operator*(const ClassTreeMask& other) const
    {
        // Create a new mask
        ClassTreeMask newmask;

        // Add all nodes from the first mask, calculate the rule with the and-operation
        for (ClassTreeMaskIterator it = this->root_; it; ++it)
        {
            const Identifier* subclass = it->getClass();
            newmask.add(subclass, this->isIncluded(subclass) and other.isIncluded(subclass));
        }

        // Add all nodes from the second mask, calculate the rule with the and-operation
        for (ClassTreeMaskIterator it = other.root_; it; ++it)
        {
            const Identifier* subclass = it->getClass();
            newmask.add(subclass, this->isIncluded(subclass) and other.isIncluded(subclass));
        }

        // Drop all redundant rules
        newmask.clean();

        // Return the new mask
        return newmask;
    }

    /**
        @brief Removes all elements of the second mask from the first mask (all classes that are included in the first mask stay included, except those that are included in the second mask too).
        @param other The mask to subtract.
        @return The difference
    */
    ClassTreeMask ClassTreeMask::operator-(const ClassTreeMask& other) const
    {
        return ((*this) * (!other));
    }

    /**
        @brief Inverts the mask (all included classes are now excluded and vice versa).
        @return The complement
    */
    ClassTreeMask ClassTreeMask::operator!() const
    {
        // Create a new mask
        ClassTreeMask newmask;

        // Add all nodes from the other mask, inverting the rules
        for (ClassTreeMaskIterator it = this->root_; it; ++it)
        {
            const Identifier* subclass = it->getClass();
            newmask.add(subclass, !this->isIncluded(subclass));
        }

        // Return the new mask
        return newmask;
    }

    /**
        @brief Unites this mask with another mask.
        @param other The other mask
        @return A reference to the mask itself
    */
    ClassTreeMask& ClassTreeMask::operator+=(const ClassTreeMask& other)
    {
        (*this) = (*this) + other;
        return (*this);
    }

    /**
        @brief Intersects this mask with another mask.
        @param other The other mask
        @return A reference to the mask itself
    */
    ClassTreeMask& ClassTreeMask::operator*=(const ClassTreeMask& other)
    {
        (*this) = (*this) * other;
        return (*this);
    }

    /**
        @brief Subtracts another mask from this mask.
        @param other The other mask
        @return A reference to the mask itself
    */
    ClassTreeMask& ClassTreeMask::operator-=(const ClassTreeMask& other)
    {
        (*this) = (*this) - other;
        return (*this);
    }

    /**
        @brief Intersects two masks (only classes that are included in both masks will be included in the resulting mask too).
        @param other The mask to intersect with
        @return The intersection
    */
    ClassTreeMask ClassTreeMask::operator&(const ClassTreeMask& other) const
    {
        return ((*this) * other);
    }

    /**
        @brief Adds two masks in the sense of a union (all classes that are included in at least one of the masks will be included in the resulting mask too).
        @param other The mask to unite with
        @return The union
    */
    ClassTreeMask ClassTreeMask::operator|(const ClassTreeMask& other) const
    {
        return ((*this) + other);
    }

    /**
        @brief Joins two masks in the sense of a xor (exclusivity) operation (all classes that are included in exactly one of the masks, but not in both, will be included in the resulting mask too).
        @param other The mask to join with
        @return The result
    */
    ClassTreeMask ClassTreeMask::operator^(const ClassTreeMask& other) const
    {
        // Create a new mask
        ClassTreeMask newmask;

        // Add all nodes from the first mask, calculate the rule with the xor-operation
        for (ClassTreeMaskIterator it = this->root_; it; ++it)
        {
            const Identifier* subclass = it->getClass();
            newmask.add(subclass, this->isIncluded(subclass) xor other.isIncluded(subclass));
        }

        // Add all nodes from the second mask, calculate the rule with the xor-operation
        for (ClassTreeMaskIterator it = other.root_; it; ++it)
        {
            const Identifier* subclass = it->getClass();
            newmask.add(subclass, this->isIncluded(subclass) xor other.isIncluded(subclass));
        }

        // Drop all redundant rules
        newmask.clean();

        // Return the new mask
        return newmask;
    }

    /**
        @brief Inverts the mask (all included classes are now excluded and vice versa).
        @return The complement
    */
    ClassTreeMask ClassTreeMask::operator~() const
    {
        return (!(*this));
    }

    /**
        @brief Intersects this mask with another mask (and-operation)
        @param other The other mask
        @return A reference to the mask itself
    */
    ClassTreeMask& ClassTreeMask::operator&=(const ClassTreeMask& other)
    {
        (*this) = (*this) & other;
        return (*this);
    }

    /**
        @brief Unites this mask with another mask (or-operation).
        @param other The other mask
        @return A reference to the mask itself
    */
    ClassTreeMask& ClassTreeMask::operator|=(const ClassTreeMask& other)
    {
        (*this) = (*this) | other;
        return (*this);
    }

    /**
        @brief Joins this mask with another mask with a xor-operation.
        @param other The other mask
        @return A reference to the mask itself
    */
    ClassTreeMask& ClassTreeMask::operator^=(const ClassTreeMask& other)
    {
        (*this) = (*this) ^ other;
        return (*this);
    }

    /**
        @brief Converts the content of a mask into a human readable string and puts it on the ostream.
        @param out The ostream
        @param mask The mask
        @return A reference to the ostream
    */
    std::ostream& operator<<(std::ostream& out, const ClassTreeMask& mask)
    {
        // Iterate through all rules
        for (ClassTreeMaskIterator it = mask.root_; it; ++it)
        {
            // Calculate the prefix: + means included, - means excluded
            if (it->isIncluded())
                out << "+";
            else
                out << "-";

            // Put the name of the corresponding class on the stream
            out << it->getClass()->getName() << " ";
        }

        return out;
    }
}
