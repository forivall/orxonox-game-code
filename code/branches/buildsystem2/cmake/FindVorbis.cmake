# - Try to find ogg/vorbis
# Once done this will define
#
#  VORBIS_FOUND - system has vorbis
#  VORBIS_INCLUDE_DIR
#  VORBIS_LIBRARY
#  VORBISENC_LIBRARY
#  VORBISFILE_LIBRARY
#
# $VORBISDIR is an environment variable used
# for finding vorbis.
#
# Several changes and additions by Fabian 'x3n' Landau
# Most of all rewritten by Adrian Friedli
#                 > www.orxonox.net <

INCLUDE(FindPackageHandleStandardArgs)
INCLUDE(HandleLibraryTypes)

FIND_PATH(VORBIS_INCLUDE_DIR vorbis/codec.h
  PATHS $ENV{VORBISDIR}
  PATH_SUFFIXES include
)
FIND_LIBRARY(VORBIS_LIBRARY_OPTIMIZED
  NAMES vorbis
  PATHS $ENV{VORBISDIR}
  PATH_SUFFIXES lib
)
FIND_LIBRARY(VORBIS_LIBRARY_DEBUG
  NAMES vorbisd vorbis_d
  PATHS $ENV{VORBISDIR}
  PATH_SUFFIXES lib
)
FIND_LIBRARY(VORBISFILE_LIBRARY_OPTIMIZED
  NAMES vorbisfile
  PATHS $ENV{VORBISDIR}
  PATH_SUFFIXES lib
)
FIND_LIBRARY(VORBISFILE_LIBRARY_DEBUG
  NAMES vorbisfiled vorbisfile_d
  PATHS $ENV{VORBISDIR}
  PATH_SUFFIXES lib
)

# Handle the REQUIRED argument and set VORBIS_FOUND
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Vorbis DEFAULT_MSG
  VORBIS_LIBRARY_OPTIMIZED
  VORBISFILE_LIBRARY_OPTIMIZED
  VORBIS_INCLUDE_DIR
)

# Collect optimized and debug libraries
HANDLE_LIBRARY_TYPES(VORBIS)
HANDLE_LIBRARY_TYPES(VORBISFILE)
SET(VORBIS_LIBRARIES ${VORBIS_LIBRARY} ${VORBISFILE_LIBRARY})

MARK_AS_ADVANCED(
  VORBIS_INCLUDE_DIR
  VORBIS_LIBRARY_OPTIMIZED
  VORBIS_LIBRARY_DEBUG
  VORBISFILE_LIBRARY_OPTIMIZED
  VORBISFILE_LIBRARY_DEBUG
)
