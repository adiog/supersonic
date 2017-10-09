CMAKE_MINIMUM_REQUIRED(VERSION 3.0)
PROJECT(supersonic)

SET(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_FLAGS -O2)


INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/deps)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/deps/abseil)

ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/deps/googletest)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/deps/googletest/googletest/include)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/deps/googletest/googlemock/include)


MACRO(COLLECT_PRODUCTION_CC returnVariable rootDirectory)
    FILE(GLOB_RECURSE dirtyList ${rootDirectory}/**.cc)
    SET(${returnVariable})
    FOREACH(file ${dirtyList})
        IF(NOT file MATCHES ".*_test.cc")
            LIST(APPEND ${returnVariable} ${file})
        ENDIF()
    ENDFOREACH()
ENDMACRO()

MACRO(COLLECT_TEST_CC returnVariable rootDirectory)
    FILE(GLOB_RECURSE dirtyList ${rootDirectory}/**.cc)
    SET(${returnVariable})
    FOREACH(file ${dirtyList})
        IF(file MATCHES ".*_test.cc")
            LIST(APPEND ${returnVariable} ${file})
        ENDIF()
    ENDFOREACH()
ENDMACRO()


FILE(GLOB_RECURSE ProtoFiles "${CMAKE_CURRENT_SOURCE_DIR}/**.proto")

SET(PROTOBUF_FILES ${CMAKE_CURRENT_BINARY_DIR}/generated)
INCLUDE_DIRECTORIES(${PROTOBUF_FILES})

MACRO(GENERATE_PROTOBUF proto_files)
    MESSAGE("-- Protobuf -- Generating files")
    FILE(MAKE_DIRECTORY ${PROTOBUF_FILES})
    SET(SUPERSONIC_PROTO_FILES)
    FOREACH (proto ${proto_files})
        MESSAGE("-- Protobuf -- Generating ${proto}")
        STRING(REPLACE ".proto" ".pb.cc" protoExtension ${proto})
        STRING(REPLACE ${CMAKE_CURRENT_SOURCE_DIR} ${PROTOBUF_FILES} protoOutput ${protoExtension})
        ADD_CUSTOM_COMMAND(
                OUTPUT
                    ${protoOutput}
                DEPENDS
                    ${proto}
                COMMAND
                    protoc -I ${CMAKE_CURRENT_SOURCE_DIR} --cpp_out=${PROTOBUF_FILES} ${proto}
        )
        LIST(APPEND SUPERSONIC_PROTO_FILES ${protoOutput})
    ENDFOREACH ()
    MESSAGE("-- Protobuf -- Generating files - done")
ENDMACRO()

GENERATE_PROTOBUF("${ProtoFiles}")
ADD_LIBRARY(supersonic_proto ${SUPERSONIC_PROTO_FILES})


ADD_LIBRARY(rune supersonic/utils/utf/rune.c)


FILE(GLOB_RECURSE supersonicHeaders ${CMAKE_CURRENT_SOURCE_DIR}/supersonic/**.h)
COLLECT_PRODUCTION_CC(supersonicSources ${CMAKE_CURRENT_SOURCE_DIR}/supersonic)
ADD_LIBRARY(supersonic ${supersonicSources} ${supersonicHeaders})
SET(PROJECT_LIBRARIES glog gflags pthread dl)
TARGET_LINK_LIBRARIES(supersonic supersonic_proto protobuf abseil re2 boost_system boost_timer rune ${PROJECT_LIBRARIES})


COLLECT_TEST_CC(supersonicTests ${CMAKE_CURRENT_SOURCE_DIR}/supersonic)
ADD_EXECUTABLE(supersonicTest ${CMAKE_CURRENT_SOURCE_DIR}/test_utils/supersonic_test_main.cc ${supersonicTests})
TARGET_LINK_LIBRARIES(supersonicTest supersonic gtest gmock)
