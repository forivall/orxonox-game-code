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
 *   Inspiration: MultiType by Benjamin Grauer
 */

#include "MultiTypeMath.h"

MultiTypeMath::MultiTypeMath(MultiType type) : MultiTypeString(type)
{
    if (type == MT_vector2)
        this->vector2_ = orxonox::Vector2(0, 0);
    else if (type == MT_vector3)
        this->vector3_ = orxonox::Vector3(0, 0, 0);
    else if (type == MT_colourvalue)
        this->colourvalue_ = orxonox::ColourValue(0, 0, 0, 0);
    else if (type == MT_quaternion)
        this->quaternion_ = orxonox::Quaternion(1, 0, 0, 0);
    else if (type == MT_radian)
        this->radian_ = orxonox::Radian(0);
    else if (type == MT_degree)
        this->degree_ = orxonox::Degree(0);
}

MultiTypeMath::MultiTypeMath(const MultiTypeMath& mtm)
{
    this->type_ = mtm.type_;
    this->value_ = mtm.value_;
}

MultiTypeMath& MultiTypeMath::operator=(const MultiTypeMath& mtm)
{
    this->type_ = mtm.type_;
    this->value_ = mtm.value_;
    return *this;
}

bool MultiTypeMath::operator==(const MultiTypeMath& mtm) const
{
    if (!MultiTypeString::operator==(mtm) && this->type_ == mtm.type_)
    {
        if (this->type_ == MT_vector2)
            return (this->vector2_ == mtm.vector2_);
        else if (this->type_ == MT_vector3)
            return (this->vector3_ == mtm.vector3_);
        else if (this->type_ == MT_colourvalue)
            return (this->colourvalue_ == mtm.colourvalue_);
        else if (this->type_ == MT_quaternion)
            return (this->quaternion_ == mtm.quaternion_);
        else if (this->type_ == MT_radian)
            return (this->radian_ == mtm.radian_);
        else if (this->type_ == MT_degree)
            return (this->degree_ == mtm.degree_);
    }

    return false;
}

bool MultiTypeMath::operator!=(const MultiTypeMath& mtm) const
{
    if (MultiTypeString::operator==(mtm) && this->type_ == mtm.type_)
    {
        if (this->type_ == MT_vector2)
            return (this->vector2_ != mtm.vector2_);
        else if (this->type_ == MT_vector3)
            return (this->vector3_ != mtm.vector3_);
        else if (this->type_ == MT_colourvalue)
            return (this->colourvalue_ != mtm.colourvalue_);
        else if (this->type_ == MT_quaternion)
            return (this->quaternion_ != mtm.quaternion_);
        else if (this->type_ == MT_radian)
            return (this->radian_ != mtm.radian_);
        else if (this->type_ == MT_degree)
            return (this->degree_ != mtm.degree_);
    }

    return true;
}

void MultiTypeMath::setValue(const MultiTypeMath& mtm)
{
    this->type_ = mtm.type_;
    this->value_ = mtm.value_;
}
