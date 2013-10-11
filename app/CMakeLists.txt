# Single package CMakeLists.txt script.
cmake_minimum_required(VERSION 3.0)
cmake_policy(SET CMP0048 NEW)
cmake_policy(SET CMP0011 NEW)
project(intro_c-intro VERSION 0.1.0 LANGUAGES C ASM) # ASM Fortran C CXX

set(PROJECT_PARENT intro_c)
set(PROJECT_URL bitbucket.org/thebridge0491/${PROJECT_PARENT})

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
	message(FATAL_ERROR "Error: cd build ; [DEFS] cmake [OPTS] ..")
endif()

set(prefix ${CMAKE_INSTALL_PREFIX})
option(BUILD_TESTING "Build the testing tree." ON)
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
	set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel ..." FORCE)
	set(CMAKE_CONFIGURATION_TYPES Release CACHE STRING "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel ..." FORCE)
endif()

#configure_file(${PROJECT_SOURCE_DIR}/src/${PROJECT_NAME}.pc.in
#	${PROJECT_BINARY_DIR}/lib/pkgconfig/${PROJECT_NAME}.pc @ONLY)

include(${PROJECT_SOURCE_DIR}/targets.cmake OPTIONAL)

find_package(PkgConfig REQUIRED)
pkg_check_modules(TESTFRWK REQUIRED check)
pkg_check_modules(PCRE REQUIRED libpcre)
pkg_check_modules(GLIB REQUIRED glib-2.0)
pkg_check_modules(LOG4C log4c)
if(NOT LOG4C_FOUND)
	set(LOG4C_CFLAGS "")
	set(LOG4C_LDFLAGS log4c)
endif(NOT LOG4C_FOUND)
pkg_check_modules(YAML REQUIRED yaml-0.1)
pkg_check_modules(YAJL REQUIRED yajl)
pkg_check_modules(JSONC REQUIRED json-c)


file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/bin ${PROJECT_BINARY_DIR}/lib/pkgconfig ${PROJECT_BINARY_DIR}/share/doc/${PROJECT_NAME})
#file(COPY ${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/resources DESTINATION ${PROJECT_BINARY_DIR})

set(proj_artifacts ${PROJECT_NAME} )

#[=[
add_library(objlib OBJECT src/${PROJECT_PARENT}/intro)
set_target_properties(objlib PROPERTIES POSITION_INDEPENDENT_CODE 1)
# target_compile_definitions(objlib PRIVATE " ")
target_compile_options(objlib PRIVATE -Wall -pedantic -std=c99 -m64 -I${PROJECT_SOURCE_DIR}/src ${PCRE_CFLAGS} ${GLIB_CFLAGS})
target_include_directories(objlib PRIVATE include)
target_link_libraries(objlib PUBLIC -Wl,-rpath,'$ORIGIN/' PRIVATE ${PCRE_LDFLAGS} ${GLIB_LDFLAGS})

add_library(${PROJECT_NAME} SHARED $<TARGET_OBJECTS:objlib>)
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION} SOVERSION ${PROJECT_VERSION_MAJOR})
if(NOT CMAKE_SYSTEM_NAME MATCHES Darwin)
	target_link_libraries(${PROJECT_NAME} PUBLIC -Wl,--enable-new-dtags)
endif(NOT CMAKE_SYSTEM_NAME MATCHES Darwin)
target_link_libraries(${PROJECT_NAME} PUBLIC -Wl,-rpath,'$ORIGIN/' PRIVATE ${PCRE_LDFLAGS} ${GLIB_LDFLAGS})

add_library(${PROJECT_NAME}_stat STATIC $<TARGET_OBJECTS:objlib>)
set_target_properties(${PROJECT_NAME}_stat PROPERTIES OUTPUT_NAME ${PROJECT_NAME})
]=]

if(BUILD_TESTING)
	list(APPEND proj_artifacts ts_main_${PROJECT_NAME})
	add_executable(ts_main_${PROJECT_NAME} tests/tc_collections tests/ts_main)
	set_target_properties(ts_main_${PROJECT_NAME} PROPERTIES
		EXCLUDE_FROM_ALL true OUTPUT_NAME ts_main)
	# target_compile_definitions(ts_main_${PROJECT_NAME} PRIVATE " ")
	target_compile_options(ts_main_${PROJECT_NAME} PRIVATE -Wall -pedantic -std=c99 -m64 ${TESTFRWK_CFLAGS} ${GLIB_CFLAGS} -I${prefix}/include)
	target_include_directories(ts_main_${PROJECT_NAME} PRIVATE include)
	target_link_libraries(ts_main_${PROJECT_NAME} PUBLIC -Wl,-rpath,'$ORIGIN/' PRIVATE ${TESTFRWK_LDFLAGS} ${GLIB_LDFLAGS} -L${prefix}/lib -l${PROJECT_PARENT}-util) # -L${PROJECT_BINARY_DIR} -l${PROJECT_NAME}) # -l:lib${PROJECT_NAME}.a
endif(BUILD_TESTING)
add_executable(${PROJECT_NAME} src/${PROJECT_PARENT}/intro src/${PROJECT_PARENT}/person src/${PROJECT_PARENT}/main)
set_target_properties(${PROJECT_NAME} PROPERTIES EXCLUDE_FROM_ALL false
	RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
# target_compile_definitions(${PROJECT_NAME} PRIVATE " ")
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -pedantic -std=c99 -m64 -I${PROJECT_SOURCE_DIR}/src ${PCRE_CFLAGS} ${GLIB_CFLAGS} ${LOG4C_CFLAGS} ${YAML_CFLAGS} ${YAJL_CFLAGS} ${JSONC_CFLAGS} -I${prefix}/include)
target_include_directories(${PROJECT_NAME} PRIVATE include)
target_link_libraries(${PROJECT_NAME} PUBLIC -Wl,-rpath,'$ORIGIN/' PRIVATE ${PCRE_LDFLAGS} ${GLIB_LDFLAGS} ${LOG4C_LDFLAGS} ${YAML_LDFLAGS} ${YAJL_LDFLAGS} ${JSONC_LDFLAGS} -lm -L${prefix}/lib -l${PROJECT_PARENT}-util -l${PROJECT_PARENT}-practice) # -L${PROJECT_BINARY_DIR} -l${PROJECT_NAME})

add_custom_target(run
	COMMAND bin/${PROJECT_NAME} || true)

find_program(DEBUGGER NAMES ddd) # ddd --gdb; lldb; gdb
if(DEBUGGER)
	add_custom_target(debug
		COMMAND ${DEBUGGER} --gdb bin/${PROJECT_NAME} || true)
endif(DEBUGGER)

find_program(VALGRIND NAMES valgrind)
if(VALGRIND)
	add_custom_target(valgrind
		COMMAND ${VALGRIND} --verbose --tool=memcheck bin/${PROJECT_NAME} || true)
endif(VALGRIND)


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

install(TARGETS ${PROJECT_NAME} # ${PROJECT_NAME}_stat
	RUNTIME DESTINATION bin
	ARCHIVE DESTINATION lib
	LIBRARY DESTINATION lib
	INCLUDES DESTINATION include/${PROJECT_PARENT})
#[=[
install(FILES ${PROJECT_BINARY_DIR}/lib/pkgconfig/${PROJECT_NAME}.pc
	DESTINATION lib/pkgconfig)
]=]
install(DIRECTORY ${PROJECT_BINARY_DIR}/include ${PROJECT_BINARY_DIR}/share
	DESTINATION .)
