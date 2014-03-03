option(
	BUILD_PYTHON
	"Whether or not to build the Python 3.x plugin loader"
	"On"
)

if(BUILD_PYTHON)
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

	pkg_check_modules(PYTHON3 REQUIRED python3)

	pkg_check_modules(PYGOBJECT REQUIRED pygobject-3.0>=3.0.0)

	add_library(gplugin-python MODULE
		${GPLUGIN_PYTHON_SOURCES}
		${GPLUGIN_PYTHON_HEADERS}
	)

	add_library(gplugin-python-static STATIC
		${GPLUGIN_PYTHON_SOURCES}
		${GPLUGIN_PYTHON_HEADERS}
	)

	set_target_properties(gplugin-python PROPERTIES PREFIX "")

	set_property(TARGET gplugin-python gplugin-python-static APPEND PROPERTY INCLUDE_DIRECTORIES
		${PYTHON3_INCLUDE_DIRS}
		${PYGOBJECT_INCLUDE_DIRS}
	)

	target_link_libraries(gplugin-python
		${PYTHON3_LIBRARIES}
		${PYGOBJECT_LIBRARIES}
		gplugin
	)

	target_link_libraries(gplugin-python-static
		${PYTHON3_LIBRARIES}
		${PYGOBJECT_LIBRARIES}
		gplugin
	)

	install(TARGETS gplugin-python DESTINATION lib/gplugin)
endif(BUILD_PYTHON)

add_subdirectory(tests)

