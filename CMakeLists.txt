cmake_minimum_required(VERSION 3.5)

project(JunglesDataStructs-tests)
set(PRJ_NAME ${CMAKE_PROJECT_NAME})

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} -Wall)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -Wall)

set(EXT_DEPS ${CMAKE_SOURCE_DIR}/ext_deps)
set(UNITY_DIR ${EXT_DEPS}/unity/src)

set(HEADERS_PATH "${CMAKE_SOURCE_DIR}/inc")
set(SOURCES_PATH "${CMAKE_SOURCE_DIR}/src")
set(LIB_SOURCES
    array.c
    buflist.c
    doubly_linked_list.c
    string_buf_tx.cpp
    )

include_directories("${HEADERS_PATH}" ${UNITY_DIR})

file(GLOB SOURCES ${CMAKE_SOURCE_DIR}/tests/*.c*)

add_executable(${PRJ_NAME} ${SOURCES} ${UNITY_DIR}/unity.c)

add_custom_target(run-test
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./${PRJ_NAME}
)
