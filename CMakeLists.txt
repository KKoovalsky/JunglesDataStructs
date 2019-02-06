cmake_minimum_required(VERSION 3.5)

project(JunglesDataStructs)

include(${JunglesDataStructs_SOURCE_DIR}/JunglesDataStructsCommon.cmake)

if(UNIT_TEST_LOCAL)
else()
    include(${JunglesDataStructs_SOURCE_DIR}/JunglesDataStructs.cmake)
endif()
