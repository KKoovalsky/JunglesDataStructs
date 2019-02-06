set(JunglesDataStructs_VERSION_MAJOR 0 CACHE STRING "major version" FORCE)
set(JunglesDataStructs_VERSION_MINOR 1 CACHE STRING "minor version" FORCE)
set(JunglesDataStructs_VERSION_PATCH 0 CACHE STRING "patch version" FORCE)
set(JunglesDataStructs_VERSION
    ${JunglesDataStructs_VERSION_MAJOR}.${JunglesDataStructs_VERSION_MINOR}.${JunglesDataStructs_VERSION_PATCH}
    CACHE STRING "version" FORCE)

set(HEADERS_PATH "${JunglesDataStructs_SOURCE_DIR}/inc")
set(HEADERS 
    ${HEADERS_PATH}/array.h
    ${HEADERS_PATH}/buflist.h
    ${HEADERS_PATH}/cyclic_buf.hpp
    ${HEADERS_PATH}/delayed_work_manager.hpp
    ${HEADERS_PATH}/doubly_linked_list.h
    ${HEADERS_PATH}/human_readable_commands_handler.hpp
    ${HEADERS_PATH}/string_buf_rx.hpp
    ${HEADERS_PATH}/string_buf_tx.hpp
)

set(SOURCES_PATH "${JunglesDataStructs_SOURCE_DIR}/src")
set(LIB_SOURCES
    array.c
    buflist.c
    doubly_linked_list.c
    string_buf_tx.cpp
)
