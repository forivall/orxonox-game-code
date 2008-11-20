#    UseTolua.cmake - CMake Module to generate LUA Bindings with tolua
#    Copyright (C) 2008  Adrian Friedli
#
#    This program is free software; you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation; either version 2 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program; if not, write to the Free Software
#    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA

SET(_tolua_executable_name toluaexe_orxonox)
GET_TARGET_PROPERTY(TOLUA_EXECUTABLE "${_tolua_executable_name}" LOCATION)

MACRO(INCLUDE_DIRECTORIES_QUOTES)
  FOREACH(_tolua_quote_dir ${ARGN})
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -iquote ${_tolua_quote_dir}")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -iquote ${_tolua_quote_dir}")
  ENDFOREACH(_tolua_quote_dir)
ENDMACRO(INCLUDE_DIRECTORIES_QUOTES)

MACRO(TOLUA _tolua_package _tolua_srcfiles_varname)
  # TODO: cleaner arguments handling
  SET(_tolua_inputfiles ${ARGN})
  LIST(REMOVE_ITEM _tolua_inputfiles "INPUTFILES")

  SET(_tolua_pkgfile "${CMAKE_CURRENT_BINARY_DIR}/tolua.pkg")
  SET(_tolua_cxxfile "${CMAKE_CURRENT_BINARY_DIR}/tolua_bind.cc")
  SET(_tolua_hfile   "${CMAKE_CURRENT_BINARY_DIR}/tolua_bind.h")
  SET(${_tolua_srcfiles_varname} ${${_tolua_srcfiles_varname}} "${_tolua_cxxfile}")

  # TODO: check secureness of this temporary file
  FILE(REMOVE "${_tolua_pkgfile}")
  FOREACH(_tolua_inputfile ${_tolua_inputfiles})
    FILE(APPEND "${_tolua_pkgfile}" "\$cfile \"${_tolua_inputfile}\"\n")
  ENDFOREACH(_tolua_inputfile)

  ADD_CUSTOM_COMMAND(
    OUTPUT "${_tolua_cxxfile}" "${_tolua_hfile}"
    COMMAND "${TOLUA_EXECUTABLE}" -n "${_tolua_package}"
                                  -w "${CMAKE_CURRENT_SOURCE_DIR}"
                                  -o "${_tolua_cxxfile}"
                                  -H "${_tolua_hfile}"
                                  -s "${TOLUA_PARSER_SOURCE}"
                                     "${_tolua_pkgfile}"
    DEPENDS "${_tolua_executable_name}" ${_tolua_inputfiles} ${TOLUA_PARSER_DEPENDENCIES}
    WORKING_DIRECTORY "${LIBRARY_OUTPUT_PATH}"
  )
ENDMACRO(TOLUA)
