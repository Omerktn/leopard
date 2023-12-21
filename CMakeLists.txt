cmake_minimum_required (VERSION 3.8)

project(leopard VERSION 0.1.0)

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -std=c++20")
set (CMAKE_CXX_FLAGS_DEBUG "-O0 -g") #  -fno-omit-frame-pointer -fsanitize=address
set (CMAKE_CXX_FLAGS_RELEASE "-O3")

find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    message("CCache found and enabled.")
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
else()
    message("CCache cannot be found.")
endif(CCACHE_FOUND)

add_compile_definitions("DEBUGBUILD=$<CONFIG:Debug>")
add_compile_definitions("PROJECTVERSION=${CMAKE_PROJECT_VERSION}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

add_executable (leoback backend/src/main.cpp)

set_target_properties(leoback PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    LINKER_LANGUAGE CXX
)

set(Boost_USE_STATIC_LIBS ON)

include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)

find_package(Boost COMPONENTS program_options REQUIRED)
find_package(Threads)
find_package(RapidJSON)

#target_include_directories(leoback PUBLIC
#${CMAKE_SOURCE_DIR}/backend/lib/)

include_directories(${CMAKE_SOURCE_DIR}/backend/lib/)
include_directories(${json_SOURCE_DIR}/include)

target_link_libraries(leoback PRIVATE 
Threads::Threads
Boost::program_options
nlohmann_json::nlohmann_json
libcore
libcommon
liblogger
libconcurrent
#libclient
#libws
#liblogger
#libgame
)#

add_subdirectory(backend/lib)

