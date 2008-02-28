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

#ifndef _MultiTypeMath_H__
#define _MultiTypeMath_H__

#include "UtilPrereqs.h"

#include "MultiTypeString.h"
#include "Math.h"

class _UtilExport MultiTypeMath : public MultiTypeString
{
    public:
        MultiTypeMath(MultiType      type = MT_null);
        inline MultiTypeMath(int            value) : MultiTypeString(value) {}
        inline MultiTypeMath(unsigned int   value) : MultiTypeString(value) {}
        inline MultiTypeMath(char           value) : MultiTypeString(value) {}
        inline MultiTypeMath(unsigned char  value) : MultiTypeString(value) {}
        inline MultiTypeMath(short          value) : MultiTypeString(value) {}
        inline MultiTypeMath(unsigned short value) : MultiTypeString(value) {}
        inline MultiTypeMath(long           value) : MultiTypeString(value) {}
        inline MultiTypeMath(unsigned long  value) : MultiTypeString(value) {}
        inline MultiTypeMath(float          value) : MultiTypeString(value) {}
        inline MultiTypeMath(double         value) : MultiTypeString(value) {}
        inline MultiTypeMath(long double    value) : MultiTypeString(value) {}
        inline MultiTypeMath(bool           value) : MultiTypeString(value) {}
        inline MultiTypeMath(const char*        value) : MultiTypeString(value) {}
        inline MultiTypeMath(const std::string& value) : MultiTypeString(value) {}
        inline MultiTypeMath(const orxonox::Vector2&     value) { this->setValue(value); }
        inline MultiTypeMath(const orxonox::Vector3&     value) { this->setValue(value); }
        inline MultiTypeMath(const orxonox::ColourValue& value) { this->setValue(value); }
        inline MultiTypeMath(const orxonox::Quaternion&  value) { this->setValue(value); }
        inline MultiTypeMath(const orxonox::Radian&      value) { this->setValue(value); }
        inline MultiTypeMath(const orxonox::Degree&      value) { this->setValue(value); }
        MultiTypeMath(const MultiTypeMath& mtm);

        using MultiTypeString::operator=;
        inline MultiTypeMath& operator=(const orxonox::Vector2&     value) { this->setValue(value); return *this; }
        inline MultiTypeMath& operator=(const orxonox::Vector3&     value) { this->setValue(value); return *this; }
        inline MultiTypeMath& operator=(const orxonox::ColourValue& value) { this->setValue(value); return *this; }
        inline MultiTypeMath& operator=(const orxonox::Quaternion&  value) { this->setValue(value); return *this; }
        inline MultiTypeMath& operator=(const orxonox::Radian&      value) { this->setValue(value); return *this; }
        inline MultiTypeMath& operator=(const orxonox::Degree&      value) { this->setValue(value); return *this; }
        MultiTypeMath& operator=(const MultiTypeMath& mtm);

        using MultiTypeString::operator==;
        inline bool operator==(const orxonox::Vector2&     value) const { return (this->vector2_     == value); }
        inline bool operator==(const orxonox::Vector3&     value) const { return (this->vector3_     == value); }
        inline bool operator==(const orxonox::ColourValue& value) const { return (this->colourvalue_ == value); }
        inline bool operator==(const orxonox::Quaternion&  value) const { return (this->quaternion_  == value); }
        inline bool operator==(const orxonox::Radian&      value) const { return (this->radian_      == value); }
        inline bool operator==(const orxonox::Degree&      value) const { return (this->degree_      == value); }
        bool operator==(const MultiTypeMath& mtm) const;

        using MultiTypeString::operator!=;
        inline bool operator!=(const orxonox::Vector2&     value) const { return (this->vector2_     != value); }
        inline bool operator!=(const orxonox::Vector3&     value) const { return (this->vector3_     != value); }
        inline bool operator!=(const orxonox::ColourValue& value) const { return (this->colourvalue_ != value); }
        inline bool operator!=(const orxonox::Quaternion&  value) const { return (this->quaternion_  != value); }
        inline bool operator!=(const orxonox::Radian&      value) const { return (this->radian_      != value); }
        inline bool operator!=(const orxonox::Degree&      value) const { return (this->degree_      != value); }
        bool operator!=(const MultiTypeMath& mtm) const;

        inline operator orxonox::Vector2()     const { return this->vector2_;     }
        inline operator orxonox::Vector3()     const { return this->vector3_;     }
        inline operator orxonox::ColourValue() const { return this->colourvalue_; }
        inline operator orxonox::Quaternion()  const { return this->quaternion_;  }
        inline operator orxonox::Radian()      const { return this->radian_;      }
        inline operator orxonox::Degree()      const { return this->degree_;      }

        using MultiTypeString::setValue;
        inline void setValue(const orxonox::Vector2&     value) { this->type_ = MT_vector2;     this->vector2_     = value; }
        inline void setValue(const orxonox::Vector3&     value) { this->type_ = MT_vector3;     this->vector3_     = value; }
        inline void setValue(const orxonox::ColourValue& value) { this->type_ = MT_colourvalue; this->colourvalue_ = value; }
        inline void setValue(const orxonox::Quaternion&  value) { this->type_ = MT_quaternion;  this->quaternion_  = value; }
        inline void setValue(const orxonox::Radian&      value) { this->type_ = MT_radian;      this->radian_      = value; }
        inline void setValue(const orxonox::Degree&      value) { this->type_ = MT_degree;      this->degree_      = value; }
        void setValue(const MultiTypeMath& mtm);

        inline orxonox::Vector2&     getVector2()     { return this->vector2_;     }
        inline orxonox::Vector3&     getVector3()     { return this->vector3_;     }
        inline orxonox::ColourValue& getColourValue() { return this->colourvalue_; }
        inline orxonox::Quaternion&  getQuaternion()  { return this->quaternion_;  }
        inline orxonox::Radian&      getRadian()      { return this->radian_;      }
        inline orxonox::Degree&      getDegree()      { return this->degree_;      }

        using MultiTypeString::getValue;
        inline void getValue(orxonox::Vector2*     variable) const { (*variable) = orxonox::Vector2     (this->vector2_);     }
        inline void getValue(orxonox::Vector3*     variable) const { (*variable) = orxonox::Vector3     (this->vector3_);     }
        inline void getValue(orxonox::ColourValue* variable) const { (*variable) = orxonox::ColourValue (this->colourvalue_); }
        inline void getValue(orxonox::Quaternion*  variable) const { (*variable) = orxonox::Quaternion  (this->quaternion_);  }
        inline void getValue(orxonox::Radian*      variable) const { (*variable) = orxonox::Radian      (this->radian_);      }
        inline void getValue(orxonox::Degree*      variable) const { (*variable) = orxonox::Degree      (this->degree_);      }

    protected:
        orxonox::Vector2      vector2_;
        orxonox::Vector3      vector3_;
        orxonox::ColourValue  colourvalue_;
        orxonox::Quaternion   quaternion_;
        orxonox::Radian       radian_;
        orxonox::Degree       degree_;
};

std::ostream& operator<<(std::ostream& out, MultiTypeMath& mtm);

#endif /* _MultiTypeMath_H__ */
