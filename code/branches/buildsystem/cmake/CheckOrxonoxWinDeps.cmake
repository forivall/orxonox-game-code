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
 #      ...
 #   Co-authors:
 #      ...
 #
 
################# Win check ##################
# Set the library directory when using precompiled tarballs for
# the dependencies under windows
###########################################

IF (MINGW)
  MESSAGE(STATUS "Running on MinGW. Using customized paths.")
  SET(MINGW_LIBRARY_DIR "${CMAKE_SOURCE_DIR}/../libs")
  #SET(Boost_ADDITIONAL_VERSIONS "1.36")
  SET(BOOST_INCLUDEDIR "${MINGW_LIBRARY_DIR}/boost_1_34_1")
  SET(BOOST_LIBRARYDIR "${MINGW_LIBRARY_DIR}/boost_1_34_1/stage/lib")
  SET(ENV{ENETDIR} "${MINGW_LIBRARY_DIR}/enet-1.1")
  SET(ENV{ALUTDIR} "${MINGW_LIBRARY_DIR}/freealut-1.1.0")
  SET(ENV{OPENALDIR} "${MINGW_LIBRARY_DIR}/openal-0.0.8/common")
  SET(ENV{LUA_DIR} "${MINGW_LIBRARY_DIR}/lua-5.1.3")
  SET(ENV{OGGVORBISDIR} "${MINGW_LIBRARY_DIR}/oggvorbis-1.1.3-1.2.0")
  SET(TCL_INCLUDE_PATH "${MINGW_LIBRARY_DIR}/tcl8.5.2/generic")
  SET(TCL_LIBRARY_PATH "${MINGW_LIBRARY_DIR}/tcl8.5.2/win")
  SET(ENV{CEGUIDIR} "${MINGW_LIBRARY_DIR}/cegui-0.6.1")
  SET(ENV{OGREDIR} "${MINGW_LIBRARY_DIR}/ogre/OgreMain;${MINGW_LIBRARY_DIR}/ogre/Samples/Common/bin/Release")
  SET(ZLIB_INCLUDE_DIR "${MINGW_LIBRARY_DIR}/zlib/include")
  SET(ZLIB_LIB_DIR "${MINGW_LIBRARY_DIR}/zlib")
  IF (NOT ENV{DXSDK_DIR})
    SET(ENV{DXSDK_DIR} "${MINGW_LIBRARY_DIR}/DXSDK")
  ENDIF (NOT ENV{DXSDK_DIR})
ENDIF (MINGW)

#MARK_AS_ADVANCED(MINGW_LIBRARY_DIR)
