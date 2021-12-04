# Targets CMake script.

if(BUILD_TESTING)
	enable_testing()
	add_test(NAME buildtest_${PROJECT_NAME}
		#COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target ts_main_${PROJECT_NAME}
		COMMAND ${CMAKE_MAKE_PROGRAM} -C ${CMAKE_BINARY_DIR} ts_main_${PROJECT_NAME})
	add_test(NAME test_${PROJECT_NAME}
		COMMAND ${PROJECT_BINARY_DIR}/tests/ts_main || true)
	set_tests_properties(test_${PROJECT_NAME} PROPERTIES
		DEPENDS buildtest_${PROJECT_NAME})
	set_property(TEST test_${PROJECT_NAME} APPEND PROPERTY ENVIRONMENT LD_LIBRARY_PATH=${PROJECT_BINARY_DIR}/lib)
endif(BUILD_TESTING)

if(NOT CPACK_FOUND)
	if(NOT CPACK_SOURCE_GENERATOR)
		set(CPACK_SOURCE_GENERATOR TGZ ZIP)
	endif()
	set(CPACK_SOURCE_PACKAGE_FILE_NAME ${PROJECT_NAME}-${PROJECT_VERSION})
	include(CPack RESULT_VARIABLE CPACK_FOUND)
	set(CPack_CMake_INCLUDED 0)
	list(APPEND CPACK_SOURCE_IGNORE_FILES build/ dist/ temp/ \\\\.log$ \\\\.s$ \\\\.so$ .*~ \\\\.DS_Store$)
	include(CPack RESULT_VARIABLE CPACK_FOUND)
endif(NOT CPACK_FOUND)

if(NOT TARGET uninstall)
	configure_file(${PROJECT_SOURCE_DIR}/targets-uninstall.cmake.in
		${PROJECT_BINARY_DIR}/targets-uninstall.cmake IMMEDIATE @ONLY)
	add_custom_target(uninstall
		COMMAND ${CMAKE_COMMAND} -P ${PROJECT_BINARY_DIR}/targets-uninstall.cmake
		COMMENT "Uninstall artifacts" VERBATIM)
endif()

if(NOT DEFINED DOXYGEN_FOUND)
	find_package(Doxygen)
endif(NOT DEFINED DOXYGEN_FOUND)
if(DOXYGEN_FOUND)
	add_custom_target(doc_${PROJECT_NAME}
		COMMAND ${DOXYGEN_EXECUTABLE} ../Doxyfile_*.txt || true
		COMMENT "Generating API documentation with Doxygen" VERBATIM
		WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/build)
	add_custom_command(TARGET doc_${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E remove_directory share/doc/${PROJECT_NAME}/html || true
		COMMAND ${CMAKE_COMMAND} -E rename ${PROJECT_SOURCE_DIR}/build/html share/doc/${PROJECT_NAME}/html || true
		WORKING_DIRECTORY ${PROJECT_BINARY_DIR})
	if(TARGET doc)
		add_dependencies(doc doc_${PROJECT_NAME})
	endif()
endif(DOXYGEN_FOUND)

find_program(LINT_COMMAND NAMES cppcheck DOC
	"Path to the lint program for performing code analysis")
if(LINT_COMMAND)
	#set(LINTOPTS --enable=information --report-progress --quiet --force --suppress=missingIncludeSystem --std=c99 --std=posix -Iinclude -I${PROJECT_SOURCE_DIR}/src)
	set(LINTOPTS --enable=all --report-progress --quiet --force --std=c99 --std=posix -Iinclude -I${PROJECT_SOURCE_DIR}/src)
	add_custom_target(lint_${PROJECT_NAME}
		COMMAND ${LINT_COMMAND} ${LINTOPTS} ${PROJECT_SOURCE_DIR}/src || true)
	if(TARGET lint)
		add_dependencies(lint lint_${PROJECT_NAME})
	endif()
endif(LINT_COMMAND)

find_program(COVERAGE_COMMAND NAMES lcov DOC
	"Path to the coverage program for performing code coverage inspection")
find_program(VIEWCOVERAGE_COMMAND genhtml DOC
	"Path to the HTML view generator for coverage data files")
if(COVERAGE_COMMAND)
	add_custom_target(report_${PROJECT_NAME}
		# read coverage data w/ lcov -c -d . -o .coverage ... *.gcda
		COMMAND ${COVERAGE_COMMAND} --capture -d . -o .coverage --gcov-tool ${PROJECT_SOURCE_DIR}/llvm-gcov.sh || true
		COMMAND ${VIEWCOVERAGE_COMMAND} -o cov .coverage || true
		COMMENT "Report code coverage" VERBATIM)
	#[=[
	file(GLOB_RECURSE gcdafiles_found LIST_DIRECTORIES false *.gcda)
	# read coverage data w/ [llvm-cov] gcov -f -b -n *.gcda
	foreach(gcdafile IN LISTS ${gcdafiles_found})
		add_custom_command(TARGET report_${PROJECT_NAME} PRE_BUILD
			COMMAND llvm-cov gcov -f -b -n ${gcdafile} || true)
	endforeach(gcdafile)
	]=]
	if(TARGET report)
		add_dependencies(report report_${PROJECT_NAME})
	endif()
endif(COVERAGE_COMMAND)
