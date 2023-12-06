add_subdirectory(io)

add_library(libcore STATIC
    Core.cpp
    Core.hpp
)

target_link_libraries(libcore libcommon libcoreio libcomponent)
