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
 #    Reto Grieder
 #  Description:
 #    Configures the external libraries. Whenever possible, the find scripts
 #    from the CMake module path are used, but that required some adjustments
 #    for certain libraries (Boost, OpenAL, TCL)
 #
 
INCLUDE(CompareVersionStrings)
INCLUDE(FindPackageHandleStandardArgs)

################ Misc Options ###################

OPTION(LIBRARY_USE_PACKAGE_IF_SUPPORTED
       "When set to false CMake will only look in the standard paths for libraries" ON)


############## Platform Scripts #################

# Scripts for specific library and CMake config
INCLUDE(LibraryConfigTardis)
INCLUDE(LibraryConfigApple)
INCLUDE(LibraryConfigMSVC)
INCLUDE(LibraryConfigMinGW)

# User script
SET(LIBRARY_CONFIG_USER_SCRIPT "" CACHE FILEPATH
    "Specify a CMake script if you wish to write your own library path config.
     See LibraryConfigTardis.cmake or LibraryConfigMinGW.cmake for examples.")
IF(LIBRARY_CONFIG_USER_SCRIPT)
  IF(EXISTS ${CMAKE_MODULE_PATH}/${LIBRARY_CONFIG_USER_SCRIPT}.cmake)
    INCLUDE(${LIBRARY_CONFIG_USER_SCRIPT})
  ELSEIF(EXISTS ${LIBRARY_CONFIG_USER_SCRIPT})
    INCLUDE(${LIBRARY_CONFIG_USER_SCRIPT})
  ELSEIF(EXISTS ${CMAKE_MODULE_PATH}/${LIBRARY_CONFIG_USER_SCRIPT})
    INCLUDE(${CMAKE_MODULE_PATH}/${LIBRARY_CONFIG_USER_SCRIPT})
  ENDIF()
ENDIF(LIBRARY_CONFIG_USER_SCRIPT)


############### Library finding #################
# Performs the search and sets the variables    #

FIND_PACKAGE(OGRE  1.4 EXACT REQUIRED)
FIND_PACKAGE(ENet  1.1       REQUIRED)
FIND_PACKAGE(Ogg             REQUIRED)
FIND_PACKAGE(Vorbis          REQUIRED)
FIND_PACKAGE(ALUT            REQUIRED)
FIND_PACKAGE(ZLIB            REQUIRED)
IF(WIN32)
  FIND_PACKAGE(DirectX       REQUIRED)
ENDIF(WIN32)

##### CEGUI #####
# We make use of the CEGUI script module called CEGUILua.
# However there is a small issue with that: We use Tolua, a C++ binding
# generator ourselves. And we also have to use our bindings in the same
# lua state is CEGUILua's. Unfortunately this implies that both lua runtime
# version are equal or else you get segmentation faults.
# In order to match the Lua versions we decided to ship CEGUILua in our
# repository, mainly because there is no way to determine which version of
# Lua CEGUILua was linked against (you'd have to specify yourself) and secondly
# because we can then choose the Lua version. Future plans might involve only
# accepting Lua 5.1.

# Insert all internally supported CEGUILua versions here
SET(CEGUILUA_INTERNAL_SUPPORT 0.5.0 0.6.0 0.6.1 0.6.2)
OPTION(CEGUILUA_USE_EXTERNAL_LIBRARY "Force the use of external CEGUILua library" OFF)
FIND_PACKAGE(CEGUI 0.5 REQUIRED)

##### Lua #####
IF(CEGUILUA_USE_EXTERNAL_LIBRARY)
  COMPARE_VERSION_STRINGS(${CEGUI_VERSION} "0.6" _version_comparison)
  IF(version_comparison LESS 0)
    SET(LUA_VERSION_REQUEST 5.0)
  ELSE()
    SET(LUA_VERSION_REQUEST 5.1)
  ENDIF()
ELSE()
  SET(LUA_VERSION_REQUEST 5)
ENDIF()
FIND_PACKAGE(Lua ${LUA_VERSION_REQUEST} EXACT REQUIRED)

##### OpenAL #####
FIND_PACKAGE(OpenAL REQUIRED)
# ALUT's headers include openal headers, but like <AL/al.h>, not <al.h>
# Unfortunately this is not the case on all systems, so FindOpenAL.cmake
# specifies the directory/AL, which now causes problems with ALUT.
IF(DEFINED OPENAL_FOUND_FIRST_TIME)
  SET(OPENAL_FOUND_FIRST_TIME FALSE CACHE INTERNAL "")
ELSE()
  SET(OPENAL_FOUND_FIRST_TIME TRUE CACHE INTERNAL "")
  STRING(REGEX REPLACE "^(.*)/AL$" "\\1" _openal_dir_2 ${OPENAL_INCLUDE_DIR})
  IF(_openal_dir_2)
    SET(OPENAL_INCLUDE_DIR ${OPENAL_INCLUDE_DIR} ${_openal_dir_2} CACHE STRING "" FORCE)
  ENDIF()
ENDIF()
# Notfiy user
FIND_PACKAGE_HANDLE_STANDARD_ARGS(OpenAL DEFAULT_MSG OPENAL_LIBRARY OPENAL_INCLUDE_DIR)
# Hide variables created by the script
MARK_AS_ADVANCED(OPENAL_INCLUDE_DIR OPENAL_LIBRARY)

##### TCL #####
# We only require TCL, so avoid confusing user about other TCL stuff by
# applying a little workaround
SET(Tclsh_FIND_QUIETLY TRUE)
FIND_PACKAGE(TCL 8.4 REQUIRED QUIET)
# Display messages separately
SET(TCL_FIND_QUIETLY FALSE)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(TCL DEFAULT_MSG TCL_LIBRARY TCL_INCLUDE_PATH)

##### Boost #####
# Expand the next statement if newer boost versions than 1.36.1 are released
SET(Boost_ADDITIONAL_VERSIONS 1.37 1.37.0)
FIND_PACKAGE(Boost 1.34 REQUIRED thread filesystem)
# With MSVC, automatic linking is performed for boost. So wee need to tell
# the linker where to find them. Also note that when running FindBoost for the
# first time, it will set ${Boost_LIBRARIES} to "" but afterwards to the libs.
IF (MSVC)
  # Little bit hacky, but Boost_LIBRARY_DIRS doesn't get set right when having
  # debug and optimized libraries.
  GET_FILENAME_COMPONENT(BOOST_LINK_DIR "${Boost_THREAD_LIBRARY_RELEASE}" PATH)
  LINK_DIRECTORIES(${BOOST_LINK_DIR})
ENDIF (MSVC)


####### Static/Dynamic linking options ##########

# On Windows dynamically linked libraries need some special treatment
# You may want to edit these settings if you provide your own libraries
# Note: Default option in the libraries vary, but our default option is dynamic
IF(WIN32)
  OPTION(LINK_BOOST_DYNAMIC "Link Boost dynamically on Windows" TRUE)
  OPTION(LINK_CEGUI_DYNAMIC "Link CEGUI dynamicylly on Windows" TRUE)
  OPTION(LINK_ENET_DYNAMIC  "Link ENet dynamically on Windows" TRUE)
  OPTION(LINK_OGRE_DYNAMIC  "Link OGRE dynamically on Windows" TRUE)
  OPTION(LINK_TCL_DYNAMIC   "Link TCL dynamically on Windows" TRUE)
  OPTION(LINK_ZLIB_DYNAMIC  "Link ZLib dynamically on Windows" TRUE)
  COMPARE_VERSION_STRINGS("${LUA_VERSION}" "5.1" _version_comparison)
  IF(_version_comparison LESS 0)
    OPTION(LINK_LUA_DYNAMIC "Link Lua dynamically on Windows" FALSE)
  ELSE(_version_comparison LESS 0)
    OPTION(LINK_LUA_DYNAMIC "Link Lua dynamically on Windows" TRUE)
  ENDIF(_version_comparison LESS 0)
ENDIF(WIN32)
