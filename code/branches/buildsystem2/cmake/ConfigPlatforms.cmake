 #
 #   ORXONOX - the hottest 3D action shooter ever to exist
 #                    > www.orxonox.net <
 #
 #
 #   License notice:
 #
 #   This program is free software; you can redistribute it and/or
 #   modify it under the terms of the GNU General Public License
 #   as published by the Free Software Foundation; either version 2
 #   of the License, or (at your option) any later version.
 #
 #   This program is distributed in the hope that it will be useful,
 #   but WITHOUT ANY WARRANTY; without even the implied warranty of
 #   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 #   GNU General Public License for more details.
 #
 #   You should have received a copy of the GNU General Public License
 #   along with this program; if not, write to the Free Software
 #   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 #
 #   Author:
 #      Reto Grieder
 #   Co-authors:
 #      ...
 #
 
# If you want to set specific options for your platform, simply
# create a file called "ConfigUser.cmake" in the binary folder
# (see at the bottom of the file)

############ Misc Default Options ###############

SET(LIBRARY_DEBUG_POSTFIX "_d")

OPTION(EXTRA_WARNINGS "Enable some extra warnings (pollutes the output heavily)")
IF(EXTRA_WARNINGS)
  SET(ORXONOX_WARNING_FLAGS "-Wextra --Wno-unsued-parameter")
ELSE(EXTRA_WARNINGS)
  SET(ORXONOX_WARNING_FLAGS "-Wall")
ENDIF(EXTRA_WARNINGS)

MARK_AS_ADVANCED(
  LIBRARY_DEBUG_POSTFIX
  ORXONOX_WARNINGS_FLAGS
)

###### Default Compiler/Linker Options ##########
# Most people use GCC to compile orxonox, so use that as default

SET(CMAKE_C_FLAGS   "$ENV{CFLAGS}   ${ORXONOX_WARNING_FLAGS} -fPIC")
SET(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} ${ORXONOX_WARNING_FLAGS} -fPIC")
# These flags are added to the flags above
SET(CMAKE_C_FLAGS_DEBUG            "    -g -ggdb")
SET(CMAKE_CXX_FLAGS_DEBUG          "    -g -ggdb")
SET(CMAKE_C_FLAGS_RELEASE          "-O3          -DNDEBUG")
SET(CMAKE_CXX_FLAGS_RELEASE        "-O3          -DNDEBUG")
SET(CMAKE_C_FLAGS_RELWITHDEBINFO   "-O2 -g -ggdb -DNDEBUG")
SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g -ggdb -DNDEBUG")
SET(CMAKE_C_FLAGS_MINSIZEREL       "-Os          -DNDEBUG")
SET(CMAKE_CXX_FLAGS_MINSIZEREL     "-Os          -DNDEBUG")

SET(CMAKE_LD_FLAGS "$ENV{LDFLAGS}")
SET(CMAKE_EXE_LINKER_FLAGS    " --no-undefined")
SET(CMAKE_SHARED_LINKER_FLAGS " --no-undefined")
SET(CMAKE_MODULE_LINKER_FLAGS " --no-undefined")

########## Plaform Specific Config ##############

# Set the platform specific options and paths
INCLUDE(ConfigTardis)
INCLUDE(ConfigMSVC)
INCLUDE(ConfigMinGW)
# User can create his own file if required
IF(EXISTS ${CMAKE_BINARY_DIR}/ConfigUser.cmake)
  INCLUDE(${CMAKE_BINARY_DIR}/ConfigUser)
ENDIF(EXISTS ${CMAKE_BINARY_DIR}/ConfigUser.cmake)
