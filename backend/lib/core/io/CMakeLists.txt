add_library(libcoreio STATIC
    AnyEvent.hpp
    Publisher.cpp
    Publisher.hpp
    Receiver.hpp
)

target_link_libraries(libcoreio libcommon)