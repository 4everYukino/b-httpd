cmake_minimum_required(VERSION 3.11)
project(bHTTP CXX)

SET(THIRD_DIR third-src)

# Add spdlog

ADD_SUBDIRECTORY(${THIRD_DIR}/spdlog)

INCLUDE_DIRECTORIES(${THIRD_DIR}/spdlog/include)

# Add JSONCPP

ADD_SUBDIRECTORY(${THIRD_DIR}/jsoncpp)

INCLUDE_DIRECTORIES(${THIRD_DIR}/jsoncpp/include)

# Find Boost

find_package(Boost)
if (NOT Boost_FOUND)
    message(FATAL_ERROR "Not found Boost, please install Boost first.")
endif()

# Add source files

AUX_SOURCE_DIRECTORY(./http SOURCE_LIST)
AUX_SOURCE_DIRECTORY(./utils SOURCE_LIST)

# Debug messages

message("Source files: ${SOURCE_LIST}")

# Target executable

add_executable(bHTTP ${SOURCE_LIST})

# Link to libs

target_link_libraries(bHTTP ${Boost_LIBRARIES} jsoncpp_lib spdlog)
