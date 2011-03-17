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
 *      Benjamin Grauer
 *   Co-authors:
 *      ...
 */

/*!
    @file Convert.h
    @brief Definition and Implementation of the Convert class.
*/

#ifndef _Convert_H__
#define _Convert_H__

#include "UtilPrereqs.h"

#include <string>
#include <sstream>
#include <typeinfo>

#include "Math.h"
#include "Debug.h"
#include "SubString.h"
#include "String.h"

// disable annoying warning about forcing value to boolean
#if ORXONOX_COMPILER == ORXONOX_COMPILER_MSVC
#pragma warning(push)
#pragma warning(disable:4100 4800)
#endif


//////////
// MAIN //
//////////

// Enum to declare the wanted conversion preference in case of equal type-levels
enum ConversionPreference
{
    CP_PreferToType,
    CP_PreferFromType,
};

// Helper classes to determine the preferred partial template specialization
class _ToType_   {};
class _FromType_ {};
class _Explicit_ {};


// The default convert functions
template <class FromType, class ToType, class Type>
struct ConverterSpecialized
{
    enum { specialized = false };
    static bool convert(ToType* output, const FromType& input)
    {
        COUT(2) << "Warning: Couldn't convert a value: From \"" << typeid(FromType).name() << "\" to \"" << typeid(ToType).name() << "\"" << std::endl;
        return false;
    }
};


// The default convert function if both types are the same
template <class BothTypes>
struct ConverterSpecialized<BothTypes, BothTypes, _Explicit_>
{
    enum { specialized = true };
    static bool convert(BothTypes* output, const BothTypes& input)
    { (*output) = input; return true; }
};


// The possible levels
#define __low__  0 // Everything that is or behaves like a primitive type (an can be converted with a typecast to every other low-level type)
#define __mid__  1 // Everything that has overloaded << and >> operators to operate on a std::stream
#define __high__ 2 // Everything that doesn't fullfill the lowerlevel-requirements and therefore needs specialized conversions

// Defines the levels of all types: Default is __high__ so you don't have to define every high-level type
template <class T> struct ConverterLevel              { enum { level = __high__ }; };
template <> struct ConverterLevel<std::string>        { enum { level = __mid__ }; };
template <> struct ConverterLevel<orxonox::Radian>    { enum { level = __mid__ }; };
template <> struct ConverterLevel<orxonox::Degree>    { enum { level = __mid__ }; };
template <> struct ConverterLevel<int>                { enum { level = __low__ }; };
template <> struct ConverterLevel<unsigned int>       { enum { level = __low__ }; };
template <> struct ConverterLevel<char>               { enum { level = __low__ }; };
template <> struct ConverterLevel<unsigned char>      { enum { level = __low__ }; };
template <> struct ConverterLevel<short>              { enum { level = __low__ }; };
template <> struct ConverterLevel<unsigned short>     { enum { level = __low__ }; };
template <> struct ConverterLevel<long>               { enum { level = __low__ }; };
template <> struct ConverterLevel<unsigned long>      { enum { level = __low__ }; };
template <> struct ConverterLevel<long long>          { enum { level = __low__ }; };
template <> struct ConverterLevel<unsigned long long> { enum { level = __low__ }; };
template <> struct ConverterLevel<float>              { enum { level = __low__ }; };
template <> struct ConverterLevel<double>             { enum { level = __low__ }; };
template <> struct ConverterLevel<long double>        { enum { level = __low__ }; };
template <> struct ConverterLevel<bool>               { enum { level = __low__ }; };


// Calculates the preference based on the levels of FromType and ToType
template <int from, int to>
struct ConverterPreference
{
    enum
    {
        // The maximum of both levels: element of {0, 1, 2}
        // max 0: Both types are primitives or have a similar behaviour
        // max 1: At least one type is not a primitive, but both can be put on a std::stream
        // max 2: There is at least one generic type that needs specialized conversions
        max = (from > to) ? from : to,

        // The difference between both levels limited to +-1: element of {-1, 0, 1}
        // diff -1: The FromType has higher level than the ToType
        // diff  0: Both types have the same level
        // diff  1: The ToType has higher level than the FromType
        diff = ((to - from) > 1) ? 1 : (((to - from) < -1) ? -1 : to - from)
    };
};


// The default conversion: This usually does nothing
template <int max, class FromType, class ToType>
struct ConverterDefault
{
    static bool convert(ToType* output, const FromType& input)
    {
        COUT(2) << "Warning: Couldn't convert a value: From \"" << typeid(FromType).name() << "\" to \"" << typeid(ToType).name() << "\"" << std::endl;
        return false;
    }
};
// The default conversion for primitives: A typecast (defined over two partial specialized templates to exclude all non-primitive types and classes)    template <int max, class FromType, class ToType>
template <class FromType, class ToType>
struct ConverterDefault<0, FromType, ToType>
{
    static bool convert(ToType* output, const FromType& input)
    {
        (*output) = (ToType)input;
        return true;
    }
};


// Converter: Converts input of FromType into output of ToType
template <int diff, int max, class FromType, class ToType, ConversionPreference pref>
struct Converter
{
    static bool convert(ToType* output, const FromType& input)
    {
        return false;
    }
};
// Converter: level{FromType} > level{ToType}
template <int max, class FromType, class ToType, ConversionPreference pref>
struct Converter<-1, max, FromType, ToType, pref>
{   static bool convert(ToType* output, const FromType& input)
    { return (ConverterSpecialized<FromType, ToType, _Explicit_>::specialized) ? (ConverterSpecialized<FromType, ToType, _Explicit_>::convert(output, input)) : (ConverterSpecialized<FromType, ToType, _FromType_>::specialized) ? (ConverterSpecialized<FromType, ToType, _FromType_>::convert(output, input)) : (ConverterDefault<max, FromType, ToType>::convert(output, input)); } };
// Converter: level{FromType} < level{ToType}
template <int max, class FromType, class ToType, ConversionPreference pref>
struct Converter<1, max, FromType, ToType, pref>
{   static bool convert(ToType* output, const FromType& input)
    { return (ConverterSpecialized<FromType, ToType, _Explicit_>::specialized) ? (ConverterSpecialized<FromType, ToType, _Explicit_>::convert(output, input)) : (ConverterSpecialized<FromType, ToType, _ToType_>::specialized) ? (ConverterSpecialized<FromType, ToType, _ToType_>::convert(output, input)) : (ConverterDefault<max, FromType, ToType>::convert(output, input)); } };
// Converter: level{FromType} = level{ToType}
// CP_PreferToType
template <int max, class FromType, class ToType>
struct Converter<0, max, FromType, ToType, CP_PreferToType>
{   static bool convert(ToType* output, const FromType& input)
    { return (ConverterSpecialized<FromType, ToType, _Explicit_>::specialized) ? (ConverterSpecialized<FromType, ToType, _Explicit_>::convert(output, input)) : (ConverterSpecialized<FromType, ToType, _ToType_>::specialized) ? (ConverterSpecialized<FromType, ToType, _ToType_>::convert(output, input)) : (ConverterSpecialized<FromType, ToType, _FromType_>::specialized) ? (ConverterSpecialized<FromType, ToType, _FromType_>::convert(output, input)) : (ConverterDefault<max, FromType, ToType>::convert(output, input)); } };
// CP_PreferFromType
template <int max, class FromType, class ToType>
struct Converter<0, max, FromType, ToType, CP_PreferFromType>
{   static bool convert(ToType* output, const FromType& input)
    { return (ConverterSpecialized<FromType, ToType, _Explicit_>::specialized) ? (ConverterSpecialized<FromType, ToType, _Explicit_>::convert(output, input)) : (ConverterSpecialized<FromType, ToType, _FromType_>::specialized) ? (ConverterSpecialized<FromType, ToType, _FromType_>::convert(output, input)) : (ConverterSpecialized<FromType, ToType, _ToType_>::specialized) ? (ConverterSpecialized<FromType, ToType, _ToType_>::convert(output, input)) : (ConverterDefault<max, FromType, ToType>::convert(output, input)); } };


// Calls the Converter::convertValue function with the correct template type parameters calculated by ConverterPreference
template <class FromType, class ToType>
static bool convertValue(ToType* output, const FromType& input, ConversionPreference preference = CP_PreferToType)
{
    return (preference == CP_PreferToType) ?
           Converter<ConverterPreference<ConverterLevel<FromType>::level, ConverterLevel<ToType>::level>::diff,
                     ConverterPreference<ConverterLevel<FromType>::level, ConverterLevel<ToType>::level>::max,
                     FromType,
                     ToType,
                     CP_PreferToType>::convert(output, input)
         : Converter<ConverterPreference<ConverterLevel<FromType>::level, ConverterLevel<ToType>::level>::diff,
                     ConverterPreference<ConverterLevel<FromType>::level, ConverterLevel<ToType>::level>::max,
                     FromType,
                     ToType,
                     CP_PreferFromType>::convert(output, input);
}


//////////////////////
// HELPER FUNCTIONS //
//////////////////////

// Helper function: Calls convertValue with and without default value and returns true if the conversion was successful
template<class FromType, class ToType>
static bool ConvertValue(ToType* output, const FromType& input, ConversionPreference preference = CP_PreferToType)
{
    return convertValue(output, input, preference);
}
template<class FromType, class ToType>
static bool ConvertValue(ToType* output, const FromType& input, const ToType& fallback, ConversionPreference preference = CP_PreferToType)
{
    if (convertValue(output, input, preference))
        return true;

    (*output) = fallback;
    return false;
}

// Helper function: Calls convertValue with and without default value and returns the converted value
template<class FromType, class ToType>
static ToType getConvertedValue(const FromType& input, ConversionPreference preference = CP_PreferToType)
{
    ToType output = ToType();
    ConvertValue(&output, input, preference);
    return output;
}
template<class FromType, class ToType>
static ToType getConvertedValue(const FromType& input, const ToType& fallback, ConversionPreference preference = CP_PreferToType)
{
    ToType output = fallback;
    ConvertValue(&output, input, fallback, preference);
    return output;
}


/////////////////////
// SPECIALIZATIONS //
/////////////////////

/////////////
// SAMPLES //
/////////////
/*
// convert everything to xyz
template <class FromType>
struct ConverterSpecialized<FromType, xyz, _ToType_>
{
    enum { specialized = true };
    static bool convert(xyz* output, const FromType& input)
    { return ...; }
};

// convert xyz to everything
template <class ToType>
struct ConverterSpecialized<xyz, ToType, _FromType_>
{
    enum { specialized = true };
    static bool convert(ToType* output, const xyz& input)
    { return ...; }
};

// convert abc to xyz
template <>
struct ConverterSpecialized<abc, xyz, _Explicit_>
{
    enum { specialized = true };
    static bool convert(xyz* output, const abc& input)
    { return ...; }
};
*/

////////////
// STRING //
////////////

// convert to string
template <class FromType>
struct ConverterSpecialized<FromType, std::string, _ToType_>
{
    enum { specialized = true };
    static bool convert(std::string* output, const FromType& input)
    {
        std::ostringstream oss;
        if (oss << input)
        {
            (*output) = oss.str();
            return true;
        }
        else
            return false;
    }
};

// convert to string Shortcut
template <class FromType>
std::string convertToString(FromType value)
{
  return getConvertedValue<FromType, std::string>(value);
}

// convert from string
template <class ToType>
struct ConverterSpecialized<std::string, ToType, _FromType_>
{
    enum { specialized = true };
    static bool convert(ToType* output, const std::string& input)
    {
        std::istringstream iss(input);
        if (iss >> (*output))
            return true;
        else
            return false;
    }
};

// convert from string Shortcut
template <class ToType>
ToType convertFromString(std::string str)
{
  return getConvertedValue<std::string, ToType>(str);
}


//////////
// MATH //
//////////
// convert everything to Degree
template <class FromType>
struct ConverterSpecialized<FromType, Ogre::Degree, _ToType_>
{
    enum { specialized = true };
    static bool convert(Ogre::Degree* output, const FromType& input)
    {
        float angle = 0;
        bool success = ConvertValue<FromType, float>(&angle, input);
        (*output) = angle;
        return success;
    }
};

// convert everything to Radian
template <class FromType>
struct ConverterSpecialized<FromType, Ogre::Radian, _ToType_>
{
    enum { specialized = true };
    static bool convert(Ogre::Radian* output, const FromType& input)
    {
        float angle = 0;
        bool success = ConvertValue<FromType, float>(&angle, input);
        (*output) = angle;
        return success;
    }
};


////////////////////
// MATH TO STRING //
////////////////////

// bool to std::string
template <>
struct ConverterSpecialized<bool, std::string, _Explicit_>
{
    enum { specialized = true };
    static bool convert(std::string* output, const bool& input)
    {
        if (input)
          *output = "true";
        else
          *output = "false";
        return false;
    }
};

// Vector2 to std::string
template <>
struct ConverterSpecialized<orxonox::Vector2, std::string, _Explicit_>
{
    enum { specialized = true };
    static bool convert(std::string* output, const orxonox::Vector2& input)
    {
        std::ostringstream ostream;
        if (ostream << input.x << "," << input.y)
        {
            (*output) = ostream.str();
            return true;
        }
        return false;
    }
};

// Vector3 to std::string
template <>
struct ConverterSpecialized<orxonox::Vector3, std::string, _Explicit_>
{
    enum { specialized = true };
    static bool convert(std::string* output, const orxonox::Vector3& input)
    {
        std::ostringstream ostream;
        if (ostream << input.x << "," << input.y << "," << input.z)
        {
            (*output) = ostream.str();
            return true;
        }
        return false;
    }
};

// Vector4 to std::string
template <>
struct ConverterSpecialized<orxonox::Vector4, std::string, _Explicit_>
{
    enum { specialized = true };
    static bool convert(std::string* output, const orxonox::Vector4& input)
    {
        std::ostringstream ostream;
        if (ostream << input.x << "," << input.y << "," << input.z << "," << input.w)
        {
            (*output) = ostream.str();
            return true;
        }
        return false;
    }
};

// Quaternion to std::string
template <>
struct ConverterSpecialized<orxonox::Quaternion, std::string, _Explicit_>
{
    enum { specialized = true };
    static bool convert(std::string* output, const orxonox::Quaternion& input)
    {
        std::ostringstream ostream;
        if (ostream << input.w << "," << input.x << "," << input.y << "," << input.z)
        {
            (*output) = ostream.str();
            return true;
        }
        return false;
    }
};

// ColourValue to std::string
template <>
struct ConverterSpecialized<orxonox::ColourValue, std::string, _Explicit_>
{
    enum { specialized = true };
    static bool convert(std::string* output, const orxonox::ColourValue& input)
    {
        std::ostringstream ostream;
        if (ostream << input.r << "," << input.g << "," << input.b << "," << input.a)
        {
            (*output) = ostream.str();
            return true;
        }
        return false;
    }
};


////////////////////
// STRING TO MATH //
////////////////////

// std::string to bool
template <>
struct ConverterSpecialized<std::string, bool, _Explicit_>
{
    enum { specialized = true };
    static bool convert(bool* output, const std::string& input)
    {
        std::string stripped = getLowercase(removeTrailingWhitespaces(input));
        if (stripped == "true" || stripped == "on" || stripped == "yes")
        {
          *output = true;
          return true;
        }
        else if (stripped == "false" || stripped == "off" || stripped == "no")
        {
          *output = false;
          return true;
        }

        std::istringstream iss(input);
        if (iss >> (*output))
            return true;
        else
            return false;
    }
};

// std::string to Vector2
template <>
struct ConverterSpecialized<std::string, orxonox::Vector2, _Explicit_>
{
    enum { specialized = true };
    static bool convert(orxonox::Vector2* output, const std::string& input)
    {
        unsigned int opening_parenthesis, closing_parenthesis = input.find(')');
        if ((opening_parenthesis = input.find('(')) == std::string::npos) { opening_parenthesis = 0; } else { opening_parenthesis++; }

        SubString tokens(input.substr(opening_parenthesis, closing_parenthesis - opening_parenthesis), ",", SubString::WhiteSpaces, false, '\\', true, '"', true, '\0', '\0', true, '\0');
        if (tokens.size() >= 2)
        {
            if (!ConvertValue(&(output->x), tokens[0]))
                return false;
            if (!ConvertValue(&(output->y), tokens[1]))
                return false;

            return true;
        }
        return false;
    }
};

// std::string to Vector3
template <>
struct ConverterSpecialized<std::string, orxonox::Vector3, _Explicit_>
{
    enum { specialized = true };
    static bool convert(orxonox::Vector3* output, const std::string& input)
    {
        unsigned int opening_parenthesis, closing_parenthesis = input.find(')');
        if ((opening_parenthesis = input.find('(')) == std::string::npos) { opening_parenthesis = 0; } else { opening_parenthesis++; }

        SubString tokens(input.substr(opening_parenthesis, closing_parenthesis - opening_parenthesis), ",", SubString::WhiteSpaces, false, '\\', true, '"', true, '\0', '\0', true, '\0');
        if (tokens.size() >= 3)
        {
            if (!ConvertValue(&(output->x), tokens[0]))
                return false;
            if (!ConvertValue(&(output->y), tokens[1]))
                return false;
            if (!ConvertValue(&(output->z), tokens[2]))
                return false;

            return true;
        }
        return false;
    }
};

// std::string to Vector4
template <>
struct ConverterSpecialized<std::string, orxonox::Vector4, _Explicit_>
{
    enum { specialized = true };
    static bool convert(orxonox::Vector4* output, const std::string& input)
    {
        unsigned int opening_parenthesis, closing_parenthesis = input.find(')');
        if ((opening_parenthesis = input.find('(')) == std::string::npos) { opening_parenthesis = 0; } else { opening_parenthesis++; }

        SubString tokens(input.substr(opening_parenthesis, closing_parenthesis - opening_parenthesis), ",", SubString::WhiteSpaces, false, '\\', true, '"', true, '\0', '\0', true, '\0');
        if (tokens.size() >= 4)
        {
            if (!ConvertValue(&(output->x), tokens[0]))
                return false;
            if (!ConvertValue(&(output->y), tokens[1]))
                return false;
            if (!ConvertValue(&(output->z), tokens[2]))
                return false;
            if (!ConvertValue(&(output->w), tokens[3]))
                return false;

            return true;
        }
        return false;
    }
};

// std::string to Quaternion
template <>
struct ConverterSpecialized<std::string, orxonox::Quaternion, _Explicit_>
{
    enum { specialized = true };
    static bool convert(orxonox::Quaternion* output, const std::string& input)
    {
        unsigned int opening_parenthesis, closing_parenthesis = input.find(')');
        if ((opening_parenthesis = input.find('(')) == std::string::npos) { opening_parenthesis = 0; } else { opening_parenthesis++; }

        SubString tokens(input.substr(opening_parenthesis, closing_parenthesis - opening_parenthesis), ",", SubString::WhiteSpaces, false, '\\', true, '"', true, '\0', '\0', true, '\0');
        if (tokens.size() >= 4)
        {
            if (!ConvertValue(&(output->w), tokens[0]))
                return false;
            if (!ConvertValue(&(output->x), tokens[1]))
                return false;
            if (!ConvertValue(&(output->y), tokens[2]))
                return false;
            if (!ConvertValue(&(output->z), tokens[3]))
                return false;

            return true;
        }
        return false;
    }
};

// std::string to ColourValue
template <>
struct ConverterSpecialized<std::string, orxonox::ColourValue, _Explicit_>
{
    enum { specialized = true };
    static bool convert(orxonox::ColourValue* output, const std::string& input)
    {
        unsigned int opening_parenthesis, closing_parenthesis = input.find(')');
        if ((opening_parenthesis = input.find('(')) == std::string::npos) { opening_parenthesis = 0; } else { opening_parenthesis++; }

        SubString tokens(input.substr(opening_parenthesis, closing_parenthesis - opening_parenthesis), ",", SubString::WhiteSpaces, false, '\\', true, '"', true, '\0', '\0', true, '\0');
        if (tokens.size() >= 3)
        {
            if (!ConvertValue(&(output->r), tokens[0]))
                return false;
            if (!ConvertValue(&(output->g), tokens[1]))
                return false;
            if (!ConvertValue(&(output->b), tokens[2]))
                return false;
            if (tokens.size() >= 4)
            {
                if (!ConvertValue(&(output->a), tokens[3]))
                    return false;
            }
            else
                output->a = 1.0;

            return true;
        }
        return false;
    }
};


///////////////////////////
// Static type detection //
///////////////////////////

/**
    Template class that determines whether type T converts implicitly to type U.
@note
    This allows to detect type conversion at compile time.
    From 'Modern C++ Design' (Alexandrescu 2001).
*/
template <class T, class U>
class StaticConversion
{
    class Small { char dummy[1]; };
    class Big   { char dummy[1024]; };
    static Small Test(U);
    static Big   Test(...);
    static T MakeT();
public:
    enum { exists = sizeof(Test(MakeT())) == sizeof(Small) };
};

#if ORXONOX_COMPILER == ORXONOX_COMPILER_MSVC
#pragma warning(pop)
#endif

#endif /* _Convert_H__ */