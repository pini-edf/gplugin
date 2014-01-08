macro(add_test_plugin plugin)
	add_library(${plugin} MODULE ${plugin}.c)
	set_target_properties(${plugin} PROPERTIES PREFIX "")
	target_link_libraries(${plugin} ${GLIB_LIBRARIES} gplugin)
endmacro(add_test_plugin)

add_subdirectory(plugins)
add_subdirectory(bad-plugins)
add_subdirectory(dynamic-type)
add_subdirectory(id-collision)

add_subdirectory(load-on-query-pass)
add_subdirectory(load-on-query-fail)

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

add_gtest(test-core)
add_gtest(test-option-group)
add_gtest(test-plugin-manager-paths)
add_gtest(test-plugin-info)
add_gtest(test-signals)

add_gtest(test-dynamic-type)
add_dependencies(test-dynamic-type dynamic-type-provider dynamic-type-user)

add_gtest(test-id-collision)
add_dependencies(test-id-collision id-collision1 id-collision2)

#######################################
# Load On Query
#######################################
add_gtest(test-load-on-query)
add_dependencies(test-load-on-query load-on-query-pass load-on-query-fail)
add_definitions(
	-DTEST_LOAD_ON_QUERY_PASS_DIR="${CMAKE_CURRENT_BINARY_DIR}/load-on-query-pass"
	-DTEST_LOAD_ON_QUERY_FAIL_DIR="${CMAKE_CURRENT_BINARY_DIR}/load-on-query-fail"
)

#######################################
# Native Loader
#######################################
add_gtest(test-native-loader)
add_dependencies(test-native-loader
	basic-plugin
	dependent-plugin
	broken-dependent-plugin
	query-error

	load-exception
	load-failed
	unload-failed
)
target_link_libraries(test-native-loader gplugin-loader-tests)

add_gtest(test-version-compare)

add_definitions(-DTEST_DIR="${CMAKE_CURRENT_BINARY_DIR}/plugins/")
add_definitions(-DTEST_BAD_DIR="${CMAKE_CURRENT_BINARY_DIR}/bad-plugins/")
add_definitions(-DTEST_DYNAMIC_DIR="${CMAKE_CURRENT_BINARY_DIR}/dynamic-type/")
add_definitions(-DTEST_ID_DIR="${CMAKE_CURRENT_BINARY_DIR}/id-collision/")
add_definitions(-DTEST_LOAD_DIR="${CMAKE_CURRENT_BINARY_DIR}/load-fail/")

set(GTESTER_TESTS "${TESTS}")
set(GTESTER_LOG "testgplugin.xml")
set(GTESTER_JUNIT "testgplugin-junit.xml")

add_custom_command(
	COMMAND ${GTESTER} -k --verbose -o ${GTESTER_LOG} ${GTESTER_TESTS}
	OUTPUT ${GTESTER_LOG}
	DEPENDS ${TESTS}
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

add_custom_command(
	COMMAND ${XSLTPROC} -o ${GTESTER_JUNIT} --nonet
			${CMAKE_SOURCE_DIR}/xsl/gtester-junit.xsl
			${GTESTER_LOG}
	OUTPUT ${GTESTER_JUNIT}
	DEPENDS ${GTESTER_LOG}
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

add_custom_target(tests ALL
	DEPENDS ${GTESTER_LOG} ${GTESTER_JUNIT}
)


