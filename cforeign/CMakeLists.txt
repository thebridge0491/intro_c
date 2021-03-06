# Single package CMakeLists.txt script.
cmake_minimum_required(VERSION 3.0)
cmake_policy(SET CMP0048 NEW)
cmake_policy(SET CMP0011 NEW)
project(intro_c-cforeign VERSION 0.1.0 LANGUAGES C ASM) # ASM Fortran C CXX

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

configure_file(${PROJECT_SOURCE_DIR}/src/${PROJECT_NAME}.pc.in
	${PROJECT_BINARY_DIR}/lib/pkgconfig/${PROJECT_NAME}.pc @ONLY)

include(${PROJECT_SOURCE_DIR}/targets.cmake OPTIONAL)

find_package(PkgConfig REQUIRED)
pkg_check_modules(TESTFRWK REQUIRED check)
pkg_check_modules(GLIB REQUIRED glib-2.0)


file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/bin ${PROJECT_BINARY_DIR}/lib/pkgconfig ${PROJECT_BINARY_DIR}/share/doc/${PROJECT_NAME})
file(COPY ${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/resources DESTINATION ${PROJECT_BINARY_DIR})

set(proj_artifacts ${PROJECT_NAME} ${PROJECT_NAME}_stat)

add_custom_target(classicffi_asm_s ALL
	${CMAKE_C_COMPILER} -Iinclude -I${PROJECT_SOURCE_DIR}/src -Wall -pedantic -std=c99 -m64 -S -masm=att -fPIC -x c -o classic_asm.s ${PROJECT_SOURCE_DIR}/src/${PROJECT_PARENT}/classic_asm.c.bak
	COMMAND ${CMAKE_C_COMPILER} -Iinclude -I${PROJECT_SOURCE_DIR}/src -Wall -pedantic -std=f2003 -m64 -S -masm=att -fPIC -x f95 -o classic_f90.s ${PROJECT_SOURCE_DIR}/src/${PROJECT_PARENT}/classic_f90.f90.bak
	SOURCES ${PROJECT_SOURCE_DIR}/src/${PROJECT_PARENT}/classic_asm.c.bak ${PROJECT_SOURCE_DIR}/src/${PROJECT_PARENT}/classic_f90.f90.bak
)


add_library(objffi OBJECT ${PROJECT_BINARY_DIR}/classic_asm.s ${PROJECT_BINARY_DIR}/classic_f90.s src/${PROJECT_PARENT}/classic_c)
set_target_properties(objffi PROPERTIES POSITION_INDEPENDENT_CODE 1)
# target_compile_definitions(objffi PRIVATE " ")
target_compile_options(objffi PRIVATE -Wall -pedantic -std=c99 -m64 -I${PROJECT_SOURCE_DIR}/src ${GLIB_CFLAGS})
target_include_directories(objffi PRIVATE include)
target_link_libraries(objffi PUBLIC -Wl,-rpath,'$ORIGIN/' PRIVATE ${GLIB_LDFLAGS})

add_library(${PROJECT_NAME} SHARED $<TARGET_OBJECTS:objffi>)
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION} SOVERSION ${PROJECT_VERSION_MAJOR})
if(NOT CMAKE_SYSTEM_NAME MATCHES Darwin)
	target_link_libraries(${PROJECT_NAME} PUBLIC -Wl,--enable-new-dtags)
endif(NOT CMAKE_SYSTEM_NAME MATCHES Darwin)
target_link_libraries(${PROJECT_NAME} PUBLIC -Wl,-rpath,'$ORIGIN/' PRIVATE ${GLIB_LDFLAGS})

add_library(${PROJECT_NAME}_stat STATIC $<TARGET_OBJECTS:objffi>)
set_target_properties(${PROJECT_NAME}_stat PROPERTIES OUTPUT_NAME ${PROJECT_NAME})


if(BUILD_TESTING)
	list(APPEND proj_artifacts ts_main_${PROJECT_NAME})
	add_executable(ts_main_${PROJECT_NAME} tests/tc_classic tests/ts_main)
	set_target_properties(ts_main_${PROJECT_NAME} PROPERTIES
		EXCLUDE_FROM_ALL true OUTPUT_NAME ts_main)
	# target_compile_definitions(ts_main_${PROJECT_NAME} PRIVATE " ")
	target_compile_options(ts_main_${PROJECT_NAME} PRIVATE -Wall -pedantic -std=c99 -m64 ${TESTFRWK_CFLAGS} ${GLIB_CFLAGS} -I${prefix}/include)
	target_include_directories(ts_main_${PROJECT_NAME} PRIVATE include)
	target_link_libraries(ts_main_${PROJECT_NAME} PUBLIC -Wl,-rpath,'$ORIGIN/' PRIVATE ${TESTFRWK_LDFLAGS} -L${prefix}/lib -l${PROJECT_PARENT}-util -L${PROJECT_BINARY_DIR} -l${PROJECT_NAME}) # -l:lib${PROJECT_NAME}.a
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
