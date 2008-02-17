# Find OGRE includes and library
#
# This module defines
#  OGRE_INCLUDE_DIR
#  OGRE_LIBRARIES, the libraries to link against to use OGRE.
#  OGRE_LIB_DIR, the location of the libraries
#  OGRE_FOUND, If false, do not try to use OGRE
#
# Copyright � 2007, Matt Williams
# Modified by Nicolas Schlumberger to make it work on the Tardis-Infrastucture of the ETH Zurich
#
# Redistribution and use is allowed according to the terms of the BSD license.

IF (OGRE_LIBRARIES AND OGRE_INCLUDE_DIR)
    SET(OGRE_FIND_QUIETLY TRUE) # Already in cache, be silent
ENDIF (OGRE_LIBRARIES AND OGRE_INCLUDE_DIR)

IF (WIN32) #Windows
    MESSAGE(STATUS "Looking for OGRE")
    SET(OGRE_INCLUDE_DIR ../libs/ogre/OgreMain/include)
    SET(OGRE_LIB_DIR${CMAKE_CURRENT_SOURCE_DIR}/../libs/ogre/Samples/Common/bin/Release)
    SET(OGRE_LIBRARIES debug OgreMain_d optimized OgreMain)
ELSE (WIN32) #Unix
    FIND_PACKAGE(PkgConfig)
    PKG_SEARCH_MODULE(OGRE OGRE /usr/include/OGRE /usr/pack/ogre-1.4.5-sd/i686-debian-linux3.1/lib/pkgconfig/OGRE.pc) # tardis specific hack
    SET(OGRE_INCLUDE_DIR ${OGRE_INCLUDE_DIRS})
    SET(OGRE_LIB_DIR ${OGRE_LIBDIR})
    SET(OGRE_LIBRARIES ${OGRE_LIBRARIES} CACHE STRING "")
ENDIF (WIN32)

#Do some preparation
SEPARATE_ARGUMENTS(OGRE_INCLUDE_DIR)
SEPARATE_ARGUMENTS(OGRE_LIBRARIES)

SET(OGRE_INCLUDE_DIR ${OGRE_INCLUDE_DIR} CACHE PATH "")
SET(OGRE_LIBRARIES ${OGRE_LIBRARIES} CACHE STRING "")
SET(OGRE_LIB_DIR ${OGRE_LIB_DIR} CACHE PATH "")

IF (OGRE_INCLUDE_DIR AND OGRE_LIBRARIES)
    SET(OGRE_FOUND TRUE)
ENDIF (OGRE_INCLUDE_DIR AND OGRE_LIBRARIES)

IF (OGRE_FOUND)
    IF (NOT OGRE_FIND_QUIETLY)
        MESSAGE(STATUS "  libraries : ${OGRE_LIBRARIES} from ${OGRE_LIB_DIR}")
        MESSAGE(STATUS "  includes  : ${OGRE_INCLUDE_DIR}")
    ENDIF (NOT OGRE_FIND_QUIETLY)
ELSE (OGRE_FOUND)
    IF (OGRE_FIND_REQUIRED)
        MESSAGE(FATAL_ERROR "Could not find OGRE")
    ENDIF (OGRE_FIND_REQUIRED)
ENDIF (OGRE_FOUND)
