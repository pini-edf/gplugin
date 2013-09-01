option(
	BUILD_PYTHON
	"Whether or not to build the Python plugin loader"
	"On"
)

if(BUILD_PYTHON)
	set(Python_ADDITIONAL_VERSIONS "2.7")

	find_package(PythonLibs)

	if(PYTHONLIBS_FOUND)
		pkg_check_modules(PYGOBJECT REQUIRED pygobject-3.0>=3.0.0)

		set(GPLUGIN_PYTHON_SOURCES
			gplugin-python-core.c
			gplugin-python-plugin.c
			gplugin-python-plugin-loader.c
			gplugin-python-utils.c
		)

		set(GPLUGIN_PYTHON_HEADERS
			gplugin-python-plugin.h
			gplugin-python-plugin-loader.h
			gplugin-python-utils.h
		)

		add_library(gplugin-python MODULE
			${GPLUGIN_PYTHON_SOURCES}
			${GPLUGIN_PYTHON_HEADERS}
		)

		add_definitions(-DGPLUGIN_PYTHON_LIBRARY="${PYTHON_LIBRARY}")

		set_target_properties(gplugin-python PROPERTIES PREFIX "")

		include_directories(
			${PYTHON_INCLUDE_DIR}
			${PYGOBJECT_INCLUDE_DIRS}
		)

		target_link_libraries(gplugin-python
			${PYTHON_LIBRARIES}
			${PYGOBJECT_LIBS}
			gplugin
		)

		install(TARGETS gplugin-python DESTINATION lib/gplugin)

		add_subdirectory(tests)
	endif(PYTHONLIBS_FOUND)
endif(BUILD_PYTHON)

