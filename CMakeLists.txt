cmake_minimum_required(VERSION 2.8)

###############################################################################
# Project Info
###############################################################################
project(gidbits C)

set(GIDBITS_MAJOR_VERSION 0)
set(GIDBITS_MICRO_VERSION 0)
set(GIDBITS_MINOR_VERSION 1)
set(GIDBITS_EXTRA_VERSION dev)

set(GIDBITS_VERSION ${GIDBITS_MAJOR_VERSION}.${GIDBITS_MINOR_VERSION}.${GIDBITS_MINOR_VERSION}${GIDBITS_EXTRA_VERSION})

###############################################################################
# Dependencies
###############################################################################
include(FindPkgConfig)

set(REQ_GLIB_VER 2.16.0)

pkg_check_modules(GLIB REQUIRED
	glib-2.0>=${REQ_GLIB_VER}
	gobject-2.0>=${REQ_GLIB_VER}
)

# we separate gmodule out so our test aren't linked to it
pkg_check_modules(GMODULE REQUIRED
	gmodule-2.0>=${REQ_GLIB_VER}
)

find_program(GTESTER
	NAMES gtester
	DOC "gtester executable"
)

###############################################################################
# Build Info
###############################################################################
add_definitions(
	-g -g3 -Wall
)

include_directories(
	${CMAKE_SOURCE_DIR}
	${CMAKE_BINARY_DIR}                # for gidbits-version.h
	${CMAKE_BINARY_DIR}/gidbits        # for gidbits.h
	${GLIB_INCLUDE_DIRS}
	${GMODULE_INCLUDE_DIRS}
)

link_directories(
	${GLIB_LIBRARY_DIRS}
	${GMODULE_LIBRARY_DIRS}
)

###############################################################################
# Config Files
###############################################################################
configure_file(gidbits.pc.in ${CMAKE_BINARY_DIR}/gidbits.pc @ONLY)

###############################################################################
# Subdirectories
###############################################################################
add_subdirectory(gidbits)
add_subdirectory(tests)

