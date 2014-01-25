if(BUILD_SEED)

macro(add_seed_gtest target)
	add_executable(${target} ${target}.c)
	target_link_libraries(${target}
		${GLIB_LIBRARIES} ${SEED_LIBRARIES} gplugin
	)
	add_dependencies(${target} gplugin-seed)

	get_target_property(_output_name ${target} RUNTIME_OUTPUT_NAME)
	if(NOT ${_output_name})
		get_target_property(_output_name ${target} LOCATION)
	endif(NOT ${_output_name})

	list(APPEND SEED_TESTS ${_output_name})
endmacro(add_seed_gtest)

add_definitions(
	-DSEED_LOADER_DIR="${CMAKE_BINARY_DIR}/seed"
	-DSEED_PLUGIN_DIR="${CMAKE_CURRENT_SOURCE_DIR}/plugins"
)

add_seed_gtest(test-seed-loader)
target_link_libraries(test-seed-loader gplugin-loader-tests)

set(GTESTER_SEED_TESTS "${SEED_TESTS}")
set(GTESTER_SEED_LOG "test-gplugin-seed.xml")
set(GTESTER_SEED_JUNIT "test-gplugin-seed-junit.xml")

add_custom_command(
	COMMAND ${GTESTER} -k --verbose -o ${GTESTER_SEED_LOG} ${SEED_TESTS}
	OUTPUT ${GTESTER_SEED_LOG}
	DEPENDS gplugin gplugin-seed
	        ${SEED_TESTS} ${CMAKE_CURRENT_SOURCE_DIR}/plugins
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

add_custom_command(
	COMMAND ${XSLTPROC} -o ${GTESTER_SEED_JUNIT} --nonet
	        ${CMAKE_SOURCE_DIR}/xsl/gtester-junit.xsl
	        ${GTESTER_SEED_LOG}
	OUTPUT ${GTESTER_SEED_JUNIT}
	DEPENDS ${GTESTER_SEED_LOG}
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

add_custom_target(seed-tests ALL
	DEPENDS ${GTESTER_SEED_LOG} ${GTESTER_SEED_JUNIT} ${SEED_TESTS}
)

endif(BUILD_SEED)

