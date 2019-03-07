cmake_minimum_required(VERSION 3.5)

function(prepend_each_in_list var prefix)
    set(listVar "")
    foreach(f ${ARGN})
        list(APPEND listVar "${prefix}/${f}")
    endforeach(f)
    set(${var} "${listVar}" PARENT_SCOPE)
endfunction(prepend_each_in_list)

project(JunglesDataStructs)

set(JunglesDataStructs_VERSION_MAJOR 0 CACHE STRING "major version" FORCE)
set(JunglesDataStructs_VERSION_MINOR 1 CACHE STRING "minor version" FORCE)
set(JunglesDataStructs_VERSION_PATCH 0 CACHE STRING "patch version" FORCE)
set(JunglesDataStructs_VERSION
    ${JunglesDataStructs_VERSION_MAJOR}.${JunglesDataStructs_VERSION_MINOR}.${JunglesDataStructs_VERSION_PATCH}
    CACHE STRING "version" FORCE)

set(HEADERS_PATH "${JunglesDataStructs_SOURCE_DIR}/inc")
set(HEADERS
    array.h
    buflist.h
    cyclic_buf.hpp
    delayed_work_manager.hpp
    doubly_linked_list.h
    human_readable_commands_handler.hpp
    string_buf_rx.hpp
    string_buf_tx.hpp
    )

set(SOURCES_PATH "${JunglesDataStructs_SOURCE_DIR}/src")
set(LIB_SOURCES
    array.c
    buflist.c
    doubly_linked_list.c
    string_buf_tx.cpp
    )

prepend_each_in_list(HEADERS_REL "inc" ${HEADERS})

set(INSTALL_INCLUDE_DIR "include/JunglesDataStructs-${JunglesDataStructs_VERSION}"
    CACHE PATH "Installation directory for includes")
set(INSTALL_LIB_DIR "lib/JunglesDataStructs-${JunglesDataStructs_VERSION}"
    CACHE PATH "Installation directory for libraries")
set(INSTALL_BIN_DIR "bin/JunglesDataStructs-${JunglesDataStructs_VERSION}"
    CACHE PATH "Installation directory for executables")
set(INSTALL_CMAKE_DIR "cmake/JunglesDataStructs-${JunglesDataStructs_VERSION}"
    CACHE PATH "Installation directory for CMake files")

set(LIB_COMPONENTS
    array
    buflist
    # cyclic_buf
    # delayed_work_manager
    doubly_linked_list
    # human_readable_commands_handler
    string_buf_tx
    # string_buf_rx
    )

include_directories("${PROJECT_SOURCE_DIR}" "${PROJECT_BINARY_DIR}" "${PROJECT_SOURCE_DIR}/inc")

add_subdirectory(inc)
add_subdirectory(src)

# Add all targets to the build-tree export set
export(TARGETS ${LIB_COMPONENTS} FILE "${PROJECT_BINARY_DIR}/JunglesDataStructs.cmake")

# Export the package for use from the build-tree (this registers the build-tree with a global CMake-registry)
export(PACKAGE JunglesDataStructs)

# Create the JunglesDataStructsConfig.cmake file for the build tree
set(CONF_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}" "${PROJECT_BINARY_DIR}")
configure_file(JunglesDataStructsConfig.cmake.in "${PROJECT_BINARY_DIR}/JunglesDataStructsConfig.cmake" @ONLY)

# Create the JunglesDataStructsConfig.cmake file for the install tree
set(CONF_INCLUDE_DIRS "\${JunglesDataStructs_CMAKE_DIR}/${INSTALL_INCLUDE_DIR}")
configure_file(JunglesDataStructsConfig.cmake.in
    "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/JunglesDataStructsConfig.cmake" @ONLY)

# Create the JunglesDataStructsConfigVersion.cmake file for the install tree and the build tree
configure_file(JunglesDataStructsConfigVersion.cmake.in
    "${PROJECT_BINARY_DIR}/JunglesDataStructsConfigVersion.cmake" @ONLY)

# Install the JunglesDataStructsConfig.cmake and JunglesDataStructsConfigVersion.cmake
install(FILES
    "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/JunglesDataStructsConfig.cmake"
    "${PROJECT_BINARY_DIR}/JunglesDataStructsConfigVersion.cmake"
    DESTINATION "${INSTALL_CMAKE_DIR}" COMPONENT dev)

# Install the export set for use with the install-tree
install(EXPORT JunglesDataStructsTargets DESTINATION "${INSTALL_CMAKE_DIR}" COMPONENT dev)

