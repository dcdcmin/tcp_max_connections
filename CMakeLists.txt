cmake_minimum_required(VERSION 3.5)

project(flood)

set(SYS_LIBS )
if(APPLE)
	list(APPEND SYS_LIBS -lm -lpthread)
    ADD_DEFINITIONS(
        -DMACOS -g -Wunused-variable -Wunused-parameter -Wunused-function -Wunused
        -Wno-deprecated -Woverloaded-virtual -Wwrite-strings
    )
elseif(UNIX)
	list(APPEND SYS_LIBS -lm -lpthread)
    ADD_DEFINITIONS(
        -DUNIX -g -fPIC -Wunused-variable -Wunused-parameter -Wunused-function -Wunused
        -Wno-deprecated -Woverloaded-virtual -Wwrite-strings
    )
else()
    ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS -DHAVE_STRUCT_TIMESPEC)
endif()

set(BIN_DIR ${PROJECT_SOURCE_DIR}/bin)

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR} )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BIN_DIR} )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${BIN_DIR} )

foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
    string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
    set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${BIN_DIR} )
    set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${BIN_DIR} )
    set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${BIN_DIR} )
    message(STATUS CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG})
endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )

link_directories(${BIN_DIR})
include_directories(${PROJECT_SOURCE_DIR}/common)

add_subdirectory(src)


