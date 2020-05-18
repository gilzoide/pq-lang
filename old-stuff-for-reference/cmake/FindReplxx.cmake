# FindReplxx
# -------------
# Locate replxx library. This module defines:
#
#   REPLXX_FOUND         - true if replxx library was found
#   REPLXX_LIBRARIES     - replxx library
#   REPLXX_INCLUDE_DIRS  - where to find replxx.hxx and replxx.h
#   REPLXX_VERSION       - the version of replxx found (parsed from header file)

find_path(REPLXX_INCLUDE_DIRS NAMES replxx.hxx replxx.h PATH_SUFFIXES replxx/include)
find_library(REPLXX_LIBRARIES NAMES replxx PATH_SUFFIXES replxx/lib)

if(REPLXX_INCLUDE_DIRS AND EXISTS "${REPLXX_INCLUDE_DIRS}/replxx.h")
	file(STRINGS "${REPLXX_INCLUDE_DIRS}/replxx.h" _version_line REGEX "REPLXX_VERSION ")
	string(REGEX MATCH "[0-9]+\\.[0-9]+\\.[0-9]+" REPLXX_VERSION ${_version_line})
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Replxx
	REQUIRED_VARS REPLXX_LIBRARIES REPLXX_INCLUDE_DIRS
	VERSION_VAR REPLXX_VERSION)

mark_as_advanced(REPLXX_INCLUDE_DIRS REPLXX_LIBRARIES)
