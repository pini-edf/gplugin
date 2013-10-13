set(GPLUGIN_UTILS_SOURCES
	gplugin-file-tree.c
)

set(GPLUGIN_UTILS_HEADERS
	gplugin-file-tree.h
)

add_library(gplugin-utils STATIC
	${GPLUGIN_UTILS_SOURCES}
	${GPLUGIN_UTILS_HEADERS}
)

target_link_libraries(gplugin-utils
	${GLIB_LIBRARIES}
)

