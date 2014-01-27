if(BUILD_GJS)

macro(add_gjs_gtest target)
	add_executable(${target} ${target}.c)
	target_link_libraries(${target}
		${GLIB_LIBRARIES} ${GJS_LIBRARIES} gplugin
	)
	add_dependencies(${target} gplugin-gjs)

	get_target_property(_output_name ${target} RUNTIME_OUTPUT_NAME)
	if(NOT ${_output_name})
		get_target_property(_output_name ${target} LOCATION)
	endif(NOT ${_output_name})

	list(APPEND GJS_TESTS ${_output_name})
endmacro(add_gjs_gtest)

add_definitions(
	-DGJS_LOADER_DIR="${CMAKE_BINARY_DIR}/gjs"
	-DGJS_PLUGIN_DIR="${CMAKE_CURRENT_SOURCE_DIR}/plugins"
)

add_gjs_gtest(test-gjs-loader)
target_link_libraries(test-gjs-loader gplugin-loader-tests)

set(GTESTER_GJS_TESTS "${GJS_TESTS}")
set(GTESTER_GJS_LOG "test-gplugin-gjs.xml")
set(GTESTER_GJS_JUNIT "test-gplugin-gjs-junit.xml")

add_custom_command(
	COMMAND ${GTESTER} -k --verbose -o ${GTESTER_GJS_LOG} ${GJS_TESTS}
	OUTPUT ${GTESTER_GJS_LOG}
	DEPENDS gplugin gplugin-gjs
	        ${GJS_TESTS} ${CMAKE_CURRENT_SOURCE_DIR}/plugins
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

add_custom_command(
	COMMAND ${XSLTPROC} -o ${GTESTER_GJS_JUNIT} --nonet
	        ${CMAKE_SOURCE_DIR}/xsl/gtester-junit.xsl
	        ${GTESTER_GJS_LOG}
	OUTPUT ${GTESTER_GJS_JUNIT}
	DEPENDS ${GTESTER_GJS_LOG}
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

add_custom_target(gjs-tests ALL
	DEPENDS ${GTESTER_GJS_LOG} ${GTESTER_GJS_JUNIT} ${GJS_TESTS}
)

endif(BUILD_GJS)

