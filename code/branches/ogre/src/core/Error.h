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
 *      Nicolas Perrenoud
 *   Co-authors:
 *      ...
 *
 */

/**
    @file Error.h
    @brief Definition of the Error class.
*/

#ifndef _Error_H__
#define _Error_H__

#include "CorePrereqs.h"

#include <string>

namespace orxonox
{
	class _CoreExport Error
	{
	public:
		Error();
		Error(std::string errorMsg, int errorCode = 0);
		Error(int errorCode, std::string errorMsg = "");
	private:

	};
}

#endif /* _Error_H__ */
