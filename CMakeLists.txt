cmake_minimum_required(VERSION 3.0)
project(supersonic)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS -O0)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

SET(PROJECT_LIBRARIES glog gflags pthread dl)

file(GLOB_RECURSE ProtoFiles "${CMAKE_CURRENT_SOURCE_DIR}/*.proto")

SET(PROTOBUF_FILES ${CMAKE_CURRENT_BINARY_DIR}/generated)
include_directories(${PROTOBUF_FILES})

MACRO(GENERATE_PROTOBUF proto_files)
    MESSAGE("-- Protobuf -- Generating files")
    FILE(MAKE_DIRECTORY ${PROTOBUF_FILES})
    FOREACH (proto ${proto_files})
        MESSAGE("-- Protobuf -- Generating ${proto}")
        EXECUTE_PROCESS(COMMAND protoc -I ${CMAKE_CURRENT_SOURCE_DIR} --cpp_out=${PROTOBUF_FILES} ${proto})
    ENDFOREACH ()
    MESSAGE("-- Protobuf -- Generating files - done")
ENDMACRO()

GENERATE_PROTOBUF("${ProtoFiles}")

ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/deps/googletest)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps/googletest/googletest/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps/googletest/googlemock/include)

FILE(GLOB_RECURSE SUPERSONIC_PROTO_FILES ${CMAKE_CURRENT_BINARY_DIR}/generated/**.cc)
ADD_LIBRARY(supersonic_proto ${SUPERSONIC_PROTO_FILES})
#SET_TARGET_PROPERTIES(supersonic_proto PROPERTIES LINKER_LANGUAGE CXX)

ADD_LIBRARY(rune supersonic/utils/utf/rune.c)

FILE(GLOB_RECURSE PROJECT_MAIN ${CMAKE_CURRENT_SOURCE_DIR}/supersonic/**.cc)
ADD_EXECUTABLE(supersonic ${PROJECT_MAIN})
TARGET_LINK_LIBRARIES(supersonic ${PROJECT_LIBRARIES} gtest gmock supersonic_proto protobuf re2 boost_system boost_timer rune)
