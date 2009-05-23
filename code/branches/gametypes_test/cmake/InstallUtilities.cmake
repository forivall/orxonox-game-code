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
 #    Helps configuring the INSTALL command in more global way.
 #  Parameters:
 #    Specifiy the targets as additional parameter.
 #

FUNCTION(ORXONOX_INSTALL)
  INSTALL(TARGETS ${ARGN}
    RUNTIME DESTINATION ${ORXONOX_RUNTIME_INSTALL_PATH}
    LIBRARY DESTINATION ${ORXONOX_LIBRARY_INSTALL_PATH}
    #ARCHIVE DESTINATION ${ORXONOX_ARCHIVE_INSTALL_PATH}
  )
ENDFUNCTION(ORXONOX_INSTALL)
