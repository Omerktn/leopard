add_subdirectory(bbofilter)
add_subdirectory(marketdataconnector)

add_library(libcomponent STATIC
    Component.cpp
    Component.hpp
)

target_link_libraries(libcomponent libcore libcommon libcoreio)