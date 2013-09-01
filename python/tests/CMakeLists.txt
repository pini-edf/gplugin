if(BUILD_PYTHON)

macro(add_python_gtest target)
	add_executable(${target} ${target}.c)
	target_link_libraries(${target} ${GLIB_LIBRARIES} gplugin)
	add_dependencies(${target} gplugin-python)

	get_target_property(_output_name ${target} RUNTIME_OUTPUT_NAME)
	if(NOT ${_output_name})
		get_target_property(_output_name ${target} LOCATION)
	endif(NOT ${_output_name})

	list(APPEND PYTHON_TESTS ${_output_name})
endmacro(add_python_gtest)

add_definitions(
	-DPYTHON_LOADER_DIR="${CMAKE_BINARY_DIR}/python"
	-DPYTHON_PLUGIN_DIR="${CMAKE_CURRENT_SOURCE_DIR}/plugins"
	-DGI_TYPELIB_PATH="${CMAKE_BINARY_DIR}/gplugin"
)

add_python_gtest(test-python-loader)

set(GTESTER_PYTHON_TESTS "${PYTHON_TESTS}")
set(GTESTER_PYTHON_LOG "test-gplugin-python.xml")
set(GTESTER_PYTHON_JUNIT "test-gplugin-python-junit.xml")

add_custom_command(
	COMMAND ${GTESTER} -k --verbose -o ${GTESTER_PYTHON_LOG} ${PYTHON_TESTS}
	OUTPUT ${GTESTER_PYTHON_LOG}
	DEPENDS ${PYTHON_TESTS}
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

add_custom_command(
	COMMAND ${XSLTPROC} -o ${GTESTER_PYTHON_JUNIT} --nonet
	        ${CMAKE_SOURCE_DIR}/tests/gtester-junit.xsl
	        ${GTESTER_PYTHON_LOG}
	OUTPUT ${GTESTER_PYTHON_JUNIT}
	DEPENDS ${GTESTER_PYTHON_LOG}
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

add_custom_target(python-tests ALL
	DEPENDS ${GTESTER_PYTHON_LOG} ${GTESTER_PYTHON_JUNIT}
)

endif(BUILD_PYTHON)

