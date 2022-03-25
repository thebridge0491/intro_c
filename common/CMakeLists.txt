# Single package CMakeLists.txt script.
cmake_minimum_required(VERSION 3.16)
cmake_policy(SET CMP0048 NEW)
cmake_policy(SET CMP0011 NEW)
project(intro_c-util VERSION 0.1.0 LANGUAGES C ASM) # C[XX] OBJC[XX] ASM Fortran

set(PROJECT_PARENT intro_c)
set(PROJECT_URL bitbucket.org/thebridge0491/${PROJECT_PARENT})

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
	message(FATAL_ERROR "Error: [DEFS] cmake -B <dir> [OPTS]")
endif()

set(prefix ${CMAKE_INSTALL_PREFIX})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
#set(CMAKE_SKIP_BUILD_RPATH false)
set(CMAKE_BUILD_WITH_INSTALL_RPATH true)
set(CMAKE_INSTALL_RPATH "\$ORIGIN/:${prefix}/lib")
option(BUILD_TESTING "Build the testing tree." ON)
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
	set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel ..." FORCE)
	set(CMAKE_CONFIGURATION_TYPES Release CACHE STRING "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel ..." FORCE)
endif()

configure_file(${PROJECT_SOURCE_DIR}/src/${PROJECT_NAME}.pc.in
	${PROJECT_BINARY_DIR}/lib/pkgconfig/${PROJECT_NAME}.pc @ONLY)

include(${PROJECT_SOURCE_DIR}/targets.cmake OPTIONAL)

find_package(PkgConfig REQUIRED)
pkg_check_modules(TESTFRWK REQUIRED check)
pkg_check_modules(PCRE REQUIRED libpcre)
pkg_check_modules(GLIB REQUIRED glib-2.0)


file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/bin ${PROJECT_BINARY_DIR}/tests ${PROJECT_BINARY_DIR}/src/${PROJECT_PARENT} ${PROJECT_BINARY_DIR}/lib/pkgconfig ${PROJECT_BINARY_DIR}/share/doc/${PROJECT_NAME})
file(COPY ${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/resources DESTINATION ${PROJECT_BINARY_DIR})

set(proj_artifacts ${PROJECT_NAME} ${PROJECT_NAME}_stat)


add_library(objlib OBJECT src/${PROJECT_PARENT}/util.c)
set_target_properties(objlib PROPERTIES POSITION_INDEPENDENT_CODE 1)
# target_compile_definitions(objlib PRIVATE " ")
target_compile_options(objlib PRIVATE -Wall -pedantic -std=c99 -I${PROJECT_SOURCE_DIR}/src ${PCRE_CFLAGS} ${GLIB_CFLAGS})
target_include_directories(objlib PRIVATE include)
target_link_libraries(objlib PUBLIC PRIVATE ${PCRE_LDFLAGS} ${GLIB_LDFLAGS})

add_library(${PROJECT_NAME}_stat STATIC $<TARGET_OBJECTS:objlib>)
set_target_properties(${PROJECT_NAME}_stat PROPERTIES OUTPUT_NAME ${PROJECT_NAME})

add_library(${PROJECT_NAME} SHARED $<TARGET_OBJECTS:objlib>)
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION} SOVERSION ${PROJECT_VERSION_MAJOR})
if(NOT CMAKE_SYSTEM_NAME MATCHES Darwin)
	target_link_libraries(${PROJECT_NAME} PUBLIC -Wl,--enable-new-dtags)
endif(NOT CMAKE_SYSTEM_NAME MATCHES Darwin)
target_link_libraries(${PROJECT_NAME} PUBLIC PRIVATE ${PCRE_LDFLAGS} ${GLIB_LDFLAGS})


if(BUILD_TESTING)
	list(APPEND proj_artifacts ts_main_${PROJECT_NAME})
	add_executable(ts_main_${PROJECT_NAME} tests/tc_new.c tests/ts_main.c)
	set_target_properties(ts_main_${PROJECT_NAME} PROPERTIES
		EXCLUDE_FROM_ALL true RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/tests OUTPUT_NAME ts_main)
	# target_compile_definitions(ts_main_${PROJECT_NAME} PRIVATE " ")
	target_compile_options(ts_main_${PROJECT_NAME} PRIVATE -Wall -pedantic -std=c99 ${TESTFRWK_CFLAGS} ${GLIB_CFLAGS})
	target_include_directories(ts_main_${PROJECT_NAME} PRIVATE include)
	target_link_libraries(ts_main_${PROJECT_NAME} PUBLIC PRIVATE ${TESTFRWK_LDFLAGS} -L${PROJECT_BINARY_DIR}/lib -l${PROJECT_NAME}) # -l:lib${PROJECT_NAME}.a
endif(BUILD_TESTING)


# [MATCHES | STREQUAL]
if(CMAKE_CONFIGURATION_TYPES MATCHES Debug OR CMAKE_BUILD_TYPE MATCHES Debug)
	message("-- [CMAKE_CONFIGURATION_TYPES|CMAKE_BUILD_TYPE] matches" [${CMAKE_CONFIGURATION_TYPES}|${CMAKE_BUILD_TYPE}])
	add_definitions(-UNDEBUG)
	foreach(tgt IN ITEMS ${proj_artifacts})
		target_compile_definitions(${tgt} PRIVATE DEBUG)
		target_compile_options(${tgt} PRIVATE -g3 -O0 --coverage)
		target_link_libraries(${tgt} PRIVATE --coverage)
	endforeach(tgt)
else()
	message("-- [CMAKE_CONFIGURATION_TYPES|CMAKE_BUILD_TYPE] matches" [${CMAKE_CONFIGURATION_TYPES}|${CMAKE_BUILD_TYPE}])
	add_definitions(-UDEBUG)
	foreach(tgt IN ITEMS ${proj_artifacts})
		target_compile_definitions(${tgt} PRIVATE NDEBUG)
		target_compile_options(${tgt} PRIVATE -O3)
	endforeach(tgt)
endif()

install(TARGETS ${PROJECT_NAME} ${PROJECT_NAME}_stat
	RUNTIME DESTINATION bin
	ARCHIVE DESTINATION lib
	LIBRARY DESTINATION lib
	INCLUDES DESTINATION include/${PROJECT_PARENT})

install(FILES ${PROJECT_BINARY_DIR}/lib/pkgconfig/${PROJECT_NAME}.pc
	DESTINATION lib/pkgconfig)

install(DIRECTORY ${PROJECT_BINARY_DIR}/include ${PROJECT_BINARY_DIR}/share
	DESTINATION .)
if(NOT CMAKE_SYSTEM_NAME MATCHES Windows)
    install(CODE "execute_process(COMMAND sh \"-xc\" \"pkgconf --with-path=${prefix}/lib/pkgconfig --list-all | grep ${PROJECT_NAME}\")")
endif(NOT CMAKE_SYSTEM_NAME MATCHES Windows)
