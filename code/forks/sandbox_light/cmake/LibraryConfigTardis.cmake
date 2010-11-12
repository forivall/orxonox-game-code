 #
 #             ORXONOX - the hottest 3D action shooter ever to exist
 #                             > www.orxonox.net <
 #
 #        This program is free software; you can redistribute it and/or
 #         modify it under the terms of the GNU General Public License
 #        as published by the Free Software Foundation; either version 2
 #            of the License, or (at your option) any later version.
 #
 #       This program is distributed in the hope that it will be useful,
 #        but WITHOUT ANY WARRANTY; without even the implied warranty of
 #        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 #                 GNU General Public License for more details.
 #
 #   You should have received a copy of the GNU General Public License along
 #      with this program; if not, write to the Free Software Foundation,
 #     Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 #
 #
 #  Author:
 #    Adrian Friedli
 #  Description:
 #    Sets the correct find script paths on the ETH Tardis workstations.
 #    Also sets the TARDIS variable.
 #

IF(UNIX AND NOT APPLE)
  IF(EXISTS /etc/hostname)
    FILE(STRINGS /etc/hostname HOSTNAME LIMIT_COUNT 1)
    IF(${HOSTNAME} MATCHES "^tardis-[a-z][0-9][0-9]$")
      SET (TARDIS ON)
    ENDIF()
  ENDIF()
ENDIF()

IF(TARDIS)
  MESSAGE(STATUS "Running on D-ITET isg.ee Tardis Computer. Using customized paths.")

  SET(CMAKE_C_COMPILER "gcc-4.1.1")
  SET(CMAKE_CXX_COMPILER "g++-4.1.1")

  # Note: When setting ENV${} variables, make sure to use quotes when
  #       having multiple directories.
  SET(BOOST_INCLUDEDIR "/usr/pack/boost-1.36.0-sd/include/boost-1_36")
  SET(BOOST_LIBRARYDIR "/usr/pack/boost-1.36.0-sd/i686-debian-linux4.0/lib")
  SET(ENV{LUA_DIR}     "/usr/pack/lua-5.1.4-sd;/usr/pack/lua-5.1.4-sd/i686-debian-linux4.0")
ENDIF(TARDIS)