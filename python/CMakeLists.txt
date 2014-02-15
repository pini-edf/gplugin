option(
	BUILD_PYTHON2
	"Whether or not to build the Python 2.7 plugin loader"
	"On"
)

option(
	BUILD_PYTHON3
	"Whether or not to build the Python 3.x plugin loader"
	"On"
)

if(BUILD_PYTHON2 AND BUILD_PYTHON3)
	message(FATAL_ERROR "You cannot enable both Python 2.7 and Python 3.x plugin loaders.")
endif(BUILD_PYTHON2 AND BUILD_PYTHON3)

if(BUILD_PYTHON2 OR BUILD_PYTHON3)
	set(GPLUGIN_PYTHON_SOURCES
		gplugin-python-core.c
		gplugin-python-loader.c
		gplugin-python-plugin.c
		gplugin-python-utils.c
	)

	set(GPLUGIN_PYTHON_HEADERS
		gplugin-python-loader.h
		gplugin-python-plugin.h
		gplugin-python-utils.h
	)
endif(BUILD_PYTHON2 OR BUILD_PYTHON3)

if(BUILD_PYTHON2)
	pkg_check_modules(PYTHON2 REQUIRED python-2.7)

	pkg_check_modules(PYGOBJECT REQUIRED pygobject-3.0>=3.0.0)

	add_library(gplugin-python2 MODULE
		${GPLUGIN_PYTHON_SOURCES}
		${GPLUGIN_PYTHON_HEADERS}
	)

	add_library(gplugin-python2-static STATIC
		${GPLUGIN_PYTHON_SOURCES}
		${GPLUGIN_PYTHON_HEADERS}
	)

	set_target_properties(gplugin-python2 PROPERTIES PREFIX "")

	set_property(TARGET gplugin-python2 gplugin-python2-static APPEND PROPERTY INCLUDE_DIRECTORIES
		${PYTHON2_INCLUDE_DIRS}
		${PYGOBJECT_INCLUDE_DIRS}
	)

	target_link_libraries(gplugin-python2
		${PYTHON2_LIBRARIES}
		${PYGOBJECT_LIBRARIES}
		gplugin
	)

	target_link_libraries(gplugin-python2-static
		${PYTHON2_LIBRARIES}
		${PYGOBJECT_LIBRARIES}
		gplugin
	)

	install(TARGETS gplugin-python2 DESTINATION lib/gplugin)
endif(BUILD_PYTHON2)

if(BUILD_PYTHON3)
	pkg_check_modules(PYTHON3 REQUIRED python3)

	pkg_check_modules(PYGOBJECT REQUIRED pygobject-3.0>=3.0.0)

	add_library(gplugin-python3 MODULE
		${GPLUGIN_PYTHON_SOURCES}
		${GPLUGIN_PYTHON_HEADERS}
	)

	add_library(gplugin-python3-static STATIC
		${GPLUGIN_PYTHON_SOURCES}
		${GPLUGIN_PYTHON_HEADERS}
	)

	set_target_properties(gplugin-python3 PROPERTIES PREFIX "")

	set_property(TARGET gplugin-python3 gplugin-python3-static APPEND PROPERTY INCLUDE_DIRECTORIES
		${PYTHON3_INCLUDE_DIRS}
		${PYGOBJECT_INCLUDE_DIRS}
	)

	target_link_libraries(gplugin-python3
		${PYTHON3_LIBRARIES}
		${PYGOBJECT_LIBRARIES}
		gplugin
	)

	target_link_libraries(gplugin-python3-static
		${PYTHON3_LIBRARIES}
		${PYGOBJECT_LIBRARIES}
		gplugin
	)

	install(TARGETS gplugin-python3 DESTINATION lib/gplugin)
endif(BUILD_PYTHON3)

add_subdirectory(tests)

