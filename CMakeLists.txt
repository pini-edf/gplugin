###############################################################################
# CMake Initialization
###############################################################################
cmake_minimum_required(VERSION 2.8.4)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules)

###############################################################################
# Project Info
###############################################################################
project(gplugin C)

set(GPLUGIN_MAJOR_VERSION 0)
set(GPLUGIN_MINOR_VERSION 0)
set(GPLUGIN_MICRO_VERSION 3)
set(GPLUGIN_EXTRA_VERSION dev)

set(GPLUGIN_VERSION ${GPLUGIN_MAJOR_VERSION}.${GPLUGIN_MINOR_VERSION}.${GPLUGIN_MICRO_VERSION}${GPLUGIN_EXTRA_VERSION})

###############################################################################
# Options
###############################################################################
option(
	BUILD_GIR
	"Whether or not to build a GObject Introspection type library"
	"On"
)

option(
	BUILD_PYTHON
	"Whether or not to build the Python plugin loader"
	"On"
)

###############################################################################
# Dependencies
###############################################################################
include(FindPkgConfig)

set(REQ_GLIB_VER 2.28.0)

pkg_check_modules(GLIB REQUIRED
	glib-2.0>=${REQ_GLIB_VER}
	gobject-2.0>=${REQ_GLIB_VER}
)

# we separate gmodule out so our test aren't linked to it
pkg_check_modules(GMODULE REQUIRED
	gmodule-2.0>=${REQ_GLIB_VER}
)

find_program(GLIB_GENMARSHAL
	NAMES glib-genmarshal
	DOC "glib-genmarshal executable"
)
mark_as_advanced(GLIB_GENMARSHAL)

find_program(GLIB_MKENUMS
	NAMES glib-mkenums
	DOC "glib-mkenums executable"
)
mark_as_advanced(GLIB_MKENUMS)

find_program(GTESTER
	NAMES gtester
	DOC "gtester executable"
)
mark_as_advanced(GTESTER)

# Python
if(BUILD_PYTHON)
	include(FindPythonLibs)

	if(PYTHONLIBS_FOUND)
		pkg_check_modules(PYGOBJECT REQUIRED pygobject-3.0>=3.0.0)
	endif(PYTHONLIBS_FOUND)
endif(BUILD_PYTHON)

###############################################################################
# Build Info
###############################################################################
add_definitions(
	-g -g3 -Wall
)

include_directories(
	${CMAKE_SOURCE_DIR}
	${CMAKE_BINARY_DIR}                # for built headers/sources
	${CMAKE_BINARY_DIR}/gplugin        # for gplugin.h
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
configure_file(gplugin.pc.in ${CMAKE_BINARY_DIR}/gplugin.pc @ONLY)

###############################################################################
# Subdirectories
###############################################################################
add_subdirectory(gplugin)
add_subdirectory(gplugin-gtk)
add_subdirectory(python)
add_subdirectory(tests)

###############################################################################
# Install stuff
###############################################################################
# documentation
install(FILES ChangeLog INSTALL README DESTINATION share/doc/gplugin)

# pkg-config files
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/gplugin.pc DESTINATION lib/pkgconfig)

