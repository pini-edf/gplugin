if(BUILD_PYTHON)
	set(GPLUGIN_PYTHON_SOURCES
		gplugin-python-core.c
		gplugin-python-plugin-loader.c
	)

	set(GPLUGIN_PYTHON_HEADERS
		gplugin-python-plugin-loader.h
	)

	add_library(gplugin-python MODULE
		${GPLUGIN_PYTHON_SOURCES}
		${GPLUGIN_PYTHON_HEADERS}
	)

	include_directories(
		${PYTHON_INCLUDE_DIRS}
		${PYGOBJECT_INCLUDE_DIRS}
	)

	target_link_libraries(gplugin-python
		${PYTHON_LIBS}
		${PYGOBJECT_LIBS}
		gplugin
	)
endif(BUILD_PYTHON)

