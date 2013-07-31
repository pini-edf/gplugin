option(
	BUILD_PYTHON
	"Whether or not to build the Python plugin loader"
	"Off"
)

if(BUILD_PYTHON)
	find_package(PythonLibs)

	if(PYTHON_LIBS_FOUND)
		pkg_check_modules(PYGOBJECT REQUIRED pygobject-3.0>=3.0.0)

		set(GPLUGIN_PYTHON_SOURCES
			gplugin-python-core.c
			gplugin-python-plugin.c
			gplugin-python-plugin-loader.c
		)

		set(GPLUGIN_PYTHON_HEADERS
			gplugin-python-plugin.h
			gplugin-python-plugin-loader.h
		)

		add_library(gplugin-python MODULE
			${GPLUGIN_PYTHON_SOURCES}
			${GPLUGIN_PYTHON_HEADERS}
		)

		include_directories(
			${PYTHON_INCLUDE_DIR}
			${PYGOBJECT_INCLUDE_DIRS}
		)

		target_link_libraries(gplugin-python
			${PYTHON_LIBS}
			${PYGOBJECT_LIBS}
			gplugin
		)
	endif(PYTHON_LIBS_FOUND)
endif(BUILD_PYTHON)

