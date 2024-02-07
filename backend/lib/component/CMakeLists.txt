add_subdirectory(bbofilter)
add_subdirectory(marketdataconnector)

add_library(libcomponent STATIC
    Component.cpp
    Component.hpp
    Configuration.cpp
    Configuration.hpp
    Evaluation.hpp
)

target_link_libraries(libcomponent libcore libcommon libcoreio)