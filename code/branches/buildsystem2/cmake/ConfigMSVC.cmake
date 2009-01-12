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
 
###################### MSVC config ########################
# Set the library directories and special options when
# using Visual Studio.
###########################################################

IF (MSVC)
  MESSAGE(STATUS "Running on MSVC. Using customized paths and options.")

  ###################### Libraries ########################

  # Determine library directory
  IF(EXISTS ${CMAKE_SOURCE_DIR}/dependencies/orxonox_vc8)
    SET(MSVC_LIBRARY_DIR "${CMAKE_SOURCE_DIR}/dependencies/orxonox_vc8")
  ELSEIF(EXISTS ${CMAKE_SOURCE_DIR}/../lib_dist/vc8/dependencies/orxonox_vc8)
    SET(MSVC_LIBRARY_DIR "${CMAKE_SOURCE_DIR}/../lib_dist/vc8/dependencies/orxonox_vc8")
  ELSE(EXISTS ${CMAKE_SOURCE_DIR}/dependencies/orxonox_vc8)
    MESSAGE(FATAL_ERROR "Could not find dependency directory for the Visual Studio libraries")
  ENDIF(EXISTS ${CMAKE_SOURCE_DIR}/dependencies/orxonox_vc8)
  SET(ORXONOX_LIBRARY_BIN_DIR ${MSVC_LIBRARY_DIR}/bin)

  # Set variables for the include directories and the libraries
  # Note: When setting ENV${} variables, make sure to use quotes when
  #       having multiple directories.
  SET(ENV{BOOST_ROOT}        ${MSVC_LIBRARY_DIR}/boost-1.35.0)
  SET(ENV{CEGUIDIR}          ${MSVC_LIBRARY_DIR}/cegui-0.6.1)
  SET(ENV{ENETDIR}           ${MSVC_LIBRARY_DIR}/enet-1.2)
  SET(ENV{ALUTDIR}           ${MSVC_LIBRARY_DIR}/freealut-1.1.0)
  SET(ENV{OGGDIR}            ${MSVC_LIBRARY_DIR}/libogg-1.1.3)
  SET(ENV{VORBISDIR}         ${MSVC_LIBRARY_DIR}/libvorbis-1.2.0)
  SET(ENV{OPENALDIR}         ${MSVC_LIBRARY_DIR}/openal-1.1)
  SET(ENV{LUA_DIR}           ${MSVC_LIBRARY_DIR}/lua-5.1.3)
  SET(ENV{OGRE_HOME}        "${MSVC_LIBRARY_DIR}/ogre-1.4.9;${ORXONOX_LIBRARY_BIN_DIR}")
  SET(TCL_INCLUDE_PATH       ${MSVC_LIBRARY_DIR}/tcl-8.5.2/include)
  SET(TCL_LIBRARY            ${MSVC_LIBRARY_DIR}/tcl-8.5.2/lib/tcl85t.lib)
  # Do some hacking to avoid "Tclsh not found" message
  SET(TCL_TCLSH              ${MSVC_LIBRARY_DIR}/tcl-8.5.2/lib/tcl85t.lib)
  SET(ZLIB_INCLUDE_DIR       ${MSVC_LIBRARY_DIR}/zlib-1.2.3/include)
  SET(ZLIB_LIBRARY optimized ${MSVC_LIBRARY_DIR}/zlib-1.2.3/lib/zlib.lib
                   debug     ${MSVC_LIBRARY_DIR}/zlib-1.2.3/lib/zlib_d.lib)
  SET(ZLIB_FOUND TRUE)

  #################### Compiler Flags #####################

  # /MD    Minimal Rebuild
  # /RTC1  Both basic runtime checks
  # /MD[d] Multithreaded [debug] DLL
  # /Zi    Program Database
  # /ZI    Program Database for Edit & Continue
  # /WX    Warning Level X
  # /wdX   Disable specific warning X
  SET(MSVC_CL_FLAGS "
    /D WIN32 /D __WIN32__ /D _WIN32 /D _WINDOWS
    /D BOOST_ALL_DYN_LINK
    /D OIS_DYNAMIC_LIB
    /D ZLIB_WINAPI
    /D LUA_BUILD_AS_DLL
    /D _CRT_SECURE_NO_WARNINGS
    /W3
    /EHsc
    /wd4522
    /wd4251
    /wd4800
  ")
  SET(CMAKE_C_FLAGS                  "${MSVC_CL_FLAGS}")
  SET(CMAKE_CXX_FLAGS                "${MSVC_CL_FLAGS}")

  # Note: ${CMAKE_C_FLAGS} get added to the specific ones
  SET(MSVC_CL_FLAGS_DEBUG            "/MDd /Od  /Zi /Gm /RTC1")
  SET(MSVC_CL_FLAGS_RELEASE          "/MD  /MP2 /D TOLUA_RELEASE")
  SET(CMAKE_C_FLAGS_DEBUG            "${MSVC_CL_FLAGS_DEBUG}")
  SET(CMAKE_CXX_FLAGS_DEBUG          "${MSVC_CL_FLAGS_DEBUG}")
  SET(CMAKE_C_FLAGS_RELEASE          "${MSVC_CL_FLAGS_RELEASE} /O2")
  SET(CMAKE_CXX_FLAGS_RELEASE        "${MSVC_CL_FLAGS_RELEASE} /O2")
  SET(CMAKE_C_FLAGS_RELWITHDEBINFO   "${MSVC_CL_FLAGS_RELEASE} /O2 /Zi")
  SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${MSVC_CL_FLAGS_RELEASE} /O2 /Zi")
  SET(CMAKE_C_FLAGS_MINSIZEREL       "${MSVC_CL_FLAGS_RELEASE} /O1")
  SET(CMAKE_CXX_FLAGS_MINSIZEREL     "${MSVC_CL_FLAGS_RELEASE} /O1")

  ##################### Linker Flags ######################

  SET(MSVC_LINKER_FLAGS                        "")
  SET(CMAKE_EXE_LINKER_FLAGS                   "${MSVC_LINKER_FLAGS}")
  SET(CMAKE_SHARED_LINKER_FLAGS                "${MSVC_LINKER_FLAGS}")

  # Note: ${CMAKE_EXE_LINKER_FLAGS} get added to the specific ones
  SET(MSVC_LINKER_FLAGS_DEBUG                  "/INCREMENTAL:YES")
  SET(MSVC_LINKER_FLAGS_RELEASE                "/INCREMENTAL:NO /OPT:REF /OPT:ICF")
  SET(CMAKE_EXE_LINKER_FLAGS_DEBUG             "${MSVC_LINKER_FLAGS_DEBUG}")
  SET(CMAKE_SHARED_LINKER_FLAGS_DEBUG          "${MSVC_LINKER_FLAGS_DEBUG}")
  SET(CMAKE_EXE_LINKER_FLAGS_RELEASE           "${MSVC_LINKER_FLAGS_RELEASE}")
  SET(CMAKE_SHARED_LINKER_FLAGS_RELEASE        "${MSVC_LINKER_FLAGS_RELEASE}")
  SET(CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO    "${MSVC_LINKER_FLAGS_RELEASE}")
  SET(CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO "${MSVC_LINKER_FLAGS_RELEASE}")
  SET(CMAKE_EXE_LINKER_FLAGS_MINSIZEREL        "${MSVC_LINKER_FLAGS_RELEASE}")
  SET(CMAKE_SHARED_LINKER_FLAGS_MINSIZEREL     "${MSVC_LINKER_FLAGS_RELEASE}")

  ######################### Misc ##########################

  # Set tolua working directory because lua.dll is not where tolua is
  SET(TOLUA_PARSER_WORKING_DIRECTORY ${ORXONOX_LIBRARY_BIN_DIR})
ENDIF (MSVC)
