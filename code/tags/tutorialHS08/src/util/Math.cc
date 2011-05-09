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
    @file Math.cc
    @brief Implementation of several math-functions.
*/

#include <OgrePlane.h>

#include "Math.h"
#include "Convert.h"

/**
    @brief Function for writing a Radian to a stream.
*/
std::ostream& operator<<(std::ostream& out, const orxonox::Radian& radian)
{
    out << radian.valueRadians();
    return out;
}

/**
    @brief Function for reading a Radian from a stream.
*/
std::istream& operator>>(std::istream& in, orxonox::Radian& radian)
{
    float temp;
    in >> temp;
    radian = temp;
    return in;
}

/**
    @brief Function for writing a Degree to a stream.
*/
std::ostream& operator<<(std::ostream& out, const orxonox::Degree& degree)
{
    out << degree.valueDegrees();
    return out;
}

/**
    @brief Function for reading a Degree from a stream.
*/
std::istream& operator>>(std::istream& in, orxonox::Degree& degree)
{
    float temp;
    in >> temp;
    degree = temp;
    return in;
}


/**
    @brief Gets the angle between my viewing direction and the direction to the position of the other object.
    @param myposition My position
    @param mydirection My viewing direction
    @param otherposition The position of the other object
    @return The angle

    @example
    If the other object is exactly in front of me, the function returns 0.
    If the other object is exactly behind me, the function returns pi.
    If the other object is exactly right/left to me (or above/below), the function returns pi/2.
*/
float getAngle(const orxonox::Vector3& myposition, const orxonox::Vector3& mydirection, const orxonox::Vector3& otherposition)
{
    orxonox::Vector3 distance = otherposition - myposition;
    float distancelength = distance.length();
    if (distancelength == 0)
        return 0;
    else
        return acos(clamp<float>(mydirection.dotProduct(distance) / distancelength, -1, 1));
}

/**
    @brief Gets the 2D viewing direction (up/down, left/right) to the position of the other object.
    @param myposition My position
    @param mydirection My viewing direction
    @param myorthonormal My orthonormalvector (pointing upwards through my head)
    @param otherposition The position of the other object
    @return The viewing direction

    @example
    If the other object is exactly in front of me, the function returns Vector2(0, 0).
    If the other object is exactly at my left, the function returns Vector2(-1, 0).
    If the other object is exactly at my right, the function returns Vector2(1, 0).
    If the other object is only a bit at my right, the function still returns Vector2(1, 0).
    If the other object is exactly above me, the function returns Vector2(0, 1).
*/
orxonox::Vector2 get2DViewdirection(const orxonox::Vector3& myposition, const orxonox::Vector3& mydirection, const orxonox::Vector3& myorthonormal, const orxonox::Vector3& otherposition)
{
    orxonox::Vector3 distance = otherposition - myposition;

    // project difference vector on our plane
    orxonox::Vector3 projection = Ogre::Plane(mydirection, myposition).projectVector(distance);

    float projectionlength = projection.length();
    if (projectionlength == 0) return orxonox::Vector2(0, 0);
    float angle = acos(clamp<float>(myorthonormal.dotProduct(projection) / projectionlength, -1, 1));

    if ((mydirection.crossProduct(myorthonormal)).dotProduct(distance) > 0)
        return orxonox::Vector2(sin(angle), cos(angle));
    else
        return orxonox::Vector2(-sin(angle), cos(angle));
}

/**
    @brief Gets the 2D viewing direction (up/down, left/right) to the position of the other object, multiplied with the viewing distance to the object (0� = 0, 180� = 1).
    @param myposition My position
    @param mydirection My viewing direction
    @param myorthonormal My orthonormalvector (pointing upwards through my head)
    @param otherposition The position of the other object
    @return The viewing direction

    @example
    If the other object is exactly in front of me, the function returns Vector2(0, 0).
    If the other object is exactly at my left, the function returns Vector2(-0.5, 0).
    If the other object is exactly at my right, the function returns Vector2(0.5, 0).
    If the other object is only a bit at my right, the function still returns Vector2(0.01, 0).
    If the other object is exactly above me, the function returns Vector2(0, 0.5).
*/
orxonox::Vector2 get2DViewcoordinates(const orxonox::Vector3& myposition, const orxonox::Vector3& mydirection, const orxonox::Vector3& myorthonormal, const orxonox::Vector3& otherposition)
{
    orxonox::Vector3 distance = otherposition - myposition;

    // project difference vector on our plane
    orxonox::Vector3 projection = Ogre::Plane(mydirection, myposition).projectVector(distance);

    float projectionlength = projection.length();
    if (projectionlength == 0) return orxonox::Vector2(0, 0);
    float angle = acos(clamp<float>(myorthonormal.dotProduct(projection) / projectionlength, -1, 1));

    float distancelength = distance.length();
    if (distancelength == 0) return orxonox::Vector2(0, 0);
    float radius = acos(clamp<float>(mydirection.dotProduct(distance) / distancelength, -1, 1)) / Ogre::Math::PI;

    if ((mydirection.crossProduct(myorthonormal)).dotProduct(distance) > 0)
        return orxonox::Vector2(sin(angle) * radius, cos(angle) * radius);
    else
        return orxonox::Vector2(-sin(angle) * radius, cos(angle) * radius);
}

/**
    @brief Returns the predicted position I have to aim at, if I want to hit a moving target with a moving projectile.
    @param myposition My position
    @param projectilespeed The speed of my projectile
    @param targetposition The position of my target
    @param targetvelocity The velocity of my target
    @return The predicted position

    The function predicts the position based on a linear velocity of the target. If the target changes speed or direction, the projectile will miss.
*/
orxonox::Vector3 getPredictedPosition(const orxonox::Vector3& myposition, float projectilespeed, const orxonox::Vector3& targetposition, const orxonox::Vector3& targetvelocity)
{
    float squaredProjectilespeed = projectilespeed * projectilespeed;
    orxonox::Vector3 distance = targetposition - myposition;
    float a = distance.squaredLength();
    float b = 2 * (distance.x + distance.y + distance.z) * (targetvelocity.x + targetvelocity.y + targetvelocity.z);
    float c = targetvelocity.squaredLength();

    float temp = 4*squaredProjectilespeed*c + a*a - 4*b*c;
    if (temp < 0)
        return orxonox::Vector3::ZERO;

    temp = sqrt(temp);
    float time = (temp + a) / (2 * (squaredProjectilespeed - b));
    return (targetposition + targetvelocity * time);
}

unsigned long getUniqueNumber()
{
    static unsigned long aNumber = 135;
    return aNumber++;
}

std::string getUniqueNumberStr()
{
    return convertToString(getUniqueNumber());
}