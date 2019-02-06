set(INC_DEST "include/JunglesDataStructs-${JunglesDataStructs_VERSION}")
set(LIB_DEST "lib/JunglesDataStructs-${JunglesDataStructs_VERSION}")

set(LIB_COMPONENTS
    array buflist cyclic_buf delayed_work_manager doubly_linked_list human_readable_commands_handler string_buf_tx
    string_buf_rx
    )

if(NOT JunglesDataStructs_FIND_COMPONENTS)
    set(SOURCES ${LIB_SOURCES})
else()
    foreach(comp ${JunglesDataStructs_FIND_COMPONENTS})
        if(not "${comp}" IN_LIST LIB_COMPONENTS)
            if(JunglesDataStructs_FIND_REQUIRED_${comp} OR JunglesDataStructs_FIND_REQUIRED)
                message(FATAL_ERROR "Could not find component ${comp} in JunglesDataStructs")
            endif()
        else()
            if(${comp}.c IN_LIST LIB_SOURCES)
                list(APPEND SOURCES ${SOURCES_PATH}/${comp}.c)
            elseif(${comp}.cpp IN_LIST LIB_SOURCES)
                list(APPEND SOURCES ${SOURCES_PATH}/${comp}.cpp)
            endif()
        endif()
    endforeach(comp)
endif()

add_library(JunglesDataStructs ${HEADERS} ${SOURCES})
# We must include proper directories in different way for find_package and for compiling as a library
target_include_directories(JunglesDataStructs PUBLIC
    $<BUILD_INTERFACE:${JunglesDataStructs_SOURCE_DIR}/inc> # for headers when building
    $<INSTALL_INTERFACE:${INCLUDE_DEST}> # for client in install mode
    )

install(TARGETS JunglesDataStructs DESTINATION ${LIB_DEST})
install(FILES ${HEADERS} DESTINATION ${INC_DEST})

install(TARGETS JunglesDataStructs EXPORT JunglesDataStructs DESTINATION "${LIB_DEST}")
install(EXPORT JunglesDataStructs DESTINATION "${LIB_DEST}")

install(FILES JunglesDataStructsConfig.cmake DESTINATION ${LIB_DEST})
