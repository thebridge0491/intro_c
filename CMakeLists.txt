# Multi-package project CMakeLists.txt script.
cmake_minimum_required(VERSION 3.16)
cmake_policy(SET CMP0048 NEW)
cmake_policy(SET CMP0011 NEW)
project(intro_c VERSION 0.1.0 LANGUAGES NONE) # LANGUAGES C[XX] OBJC[XX] ASM Fortran

set(PROJECT_URL bitbucket.org/thebridge0491/${PROJECT_NAME})

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
	message(FATAL_ERROR "Error: [DEFS] cmake -B <dir> [OPTS]")
endif()

set(prefix ${CMAKE_INSTALL_PREFIX})
option(BUILD_TESTING "Build the testing tree." ON)
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
	set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel ..." FORCE)
	set(CMAKE_CONFIGURATION_TYPES Release CACHE STRING "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel ..." FORCE)
endif()

if(BUILD_TESTING)
	enable_testing()
endif(BUILD_TESTING)

if(NOT DEFINED CPACK_SOURCE_GENERATOR)
	set(CPACK_SOURCE_GENERATOR TGZ ZIP)
endif()
set(CPACK_SOURCE_PACKAGE_FILE_NAME ${PROJECT_NAME}-${PROJECT_VERSION})
include(CPack RESULT_VARIABLE CPACK_FOUND)
set(CPack_CMake_INCLUDED 0)
list(APPEND CPACK_SOURCE_IGNORE_FILES build/ dist/ temp/ \\\\.log$ \\\\.s$ \\\\.so$ .*~ \\\\.DS_Store$)
include(CPack RESULT_VARIABLE CPACK_FOUND)

if(NOT TARGET uninstall)
	configure_file(${CMAKE_SOURCE_DIR}/targets-uninstall.cmake.in
		${CMAKE_BINARY_DIR}/targets-uninstall.cmake IMMEDIATE @ONLY)
	add_custom_target(uninstall
		COMMAND ${CMAKE_COMMAND} -P ${CMAKE_BINARY_DIR}/targets-uninstall.cmake
		COMMENT "Uninstall artifacts" VERBATIM)
endif()

find_package(Doxygen)
if(DOXYGEN_FOUND)
	add_custom_target(doc)
endif(DOXYGEN_FOUND)

find_program(LINT_COMMAND NAMES cppcheck DOC
	"Path to the lint program for performing code analysis")
if(LINT_COMMAND)
	add_custom_target(lint)
endif(LINT_COMMAND)

find_program(COVERAGE_COMMAND NAMES lcov DOC
	"Path to the coverage program for performing code coverage inspection")
if(COVERAGE_COMMAND)
	add_custom_target(report)
endif(COVERAGE_COMMAND)

foreach(subdir IN ITEMS common cforeign api app)
	add_subdirectory(${subdir})
endforeach(subdir)
