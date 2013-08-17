macro(add_test_plugin plugin)
	add_library(${plugin} MODULE ${plugin}.c)
	set_target_properties(${plugin} PROPERTIES PREFIX "")
	target_link_libraries(${plugin} ${GLIB_LIBRARIES} gplugin)
endmacro(add_test_plugin)

add_subdirectory(plugins)
add_subdirectory(bad-plugins)

macro(add_gtest target)
	add_executable(${target} ${target}.c)
	target_link_libraries(${target} ${GLIB_LIBRARIES} gplugin)

	get_target_property(_output_name ${target} RUNTIME_OUTPUT_NAME)
	if(${_output_name})
	else(${_output_name})
		get_target_property(_output_name ${target} LOCATION)
	endif(${_output_name})

	list(APPEND TESTS ${_output_name})
endmacro(add_gtest)

add_gtest(test-plugin-manager-paths)
add_gtest(test-plugin-info)

add_gtest(test-native-loader)
add_dependencies(test-native-loader
	basic-plugin
	dependent-plugin
	broken-dependent-plugin
	multiple-authors
	query-error
	load-error
	unload-error
)

add_gtest(test-version-compare)

add_definitions(-DTEST_DIR="${CMAKE_CURRENT_BINARY_DIR}/plugins/")
add_definitions(-DTEST_BAD_DIR="${CMAKE_CURRENT_BINARY_DIR}/bad-plugins/")

set(GTESTER_TESTS "${TESTS}")
set(GTESTER_LOG "testgplugin.xml")
set(GTESTER_JUNIT "testgplugin-junit.xml")

add_custom_command(
	COMMAND ${GTESTER} -k --verbose -o ${GTESTER_LOG} ${GTESTER_TESTS}
	OUTPUT ${GTESTER_LOG}
	DEPENDS ${GTESTER_TESTS}
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

add_custom_command(
	COMMAND ${XSLTPROC} -o ${GTESTER_JUNIT} --nonet
			${CMAKE_CURRENT_SOURCE_DIR}/gtester-junit.xsl
			${GTESTER_LOG}
	OUTPUT ${GTESTER_JUNIT}
	DEPENDS ${GTESTER_LOG}
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

add_custom_target(tests ALL
	DEPENDS ${GTESTER_LOG} ${GTESTER_JUNIT}
)


