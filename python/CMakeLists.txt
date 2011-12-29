if(PYTHONLIBS_FOUND)
	set(GPLUGIN_PYTHON_SOURCES
		gplugin-python-core.c
	)

	add_library(gplugin-python MODULE
		${GPLUGIN_PYTHON_SOURCES}
		${GPLUGIN_PYTHON_HEADERS}
	)

	include_directories(
		${PYTHONLIBS_INCLUDE_DIRS}
		${PYGOBJECT_INCLUDE_DIRS}
	)

	target_link_libraries(gplugin-python
		${PYTHONLIBS_LIBS}
		${PYGOBJECT_LIBS}
		gplugin
	)
endif(PYTHONLIBS_FOUND)

