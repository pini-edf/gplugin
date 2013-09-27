option(
	BUILD_PERL
	"Whether or not to build the Perl plugin loader"
	"Off"
)

if(BUILD_PERL)
	find_package(PerlLibs)

	if(PERLLIBS_FOUND)
		set(GPLUGIN_PERL_SOURCES
			gplugin-perl-core.c
			gplugin-perl-plugin-loader.c
		)

		set(GPLUGIN_PERL_HEADERS
			gplugin-perl-plugin-loader.h
		)

		add_library(gplugin-perl MODULE
			${GPLUGIN_PERL_SOURCES}
			${GPLUGIN_PERL_HEADERS}
		)

		execute_process(COMMAND ${PERL_EXECUTABLE} -MExtUtils::Embed -e ccopts
						OUTPUT_VARIABLE PERL_CFLAGS)

		message(STATUS Perl CFLAGS ${PERL_CFLAGS})
		add_definitions(${PERL_EXTRA_C_FLAGS} ${PERL_CFLAGS})

		set_target_properties(gplugin-perl PROPERTIES PREFIX "")

		include_directories(${PERL_INCLUDE_PATH})
		target_link_libraries(gplugin-perl ${PERL_LIBRARY})

		install(TARGETS gplugin-perl DESTINATION lib/gplugin)
	else(PERLLIBS_FOUND)
		message(FATAL_ERROR "Failed to find the Perl libraries")
	endif(PERLLIBS_FOUND)
endif(BUILD_PERL)

