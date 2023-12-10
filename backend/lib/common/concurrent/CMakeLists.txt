add_library(libconcurrent STATIC
    Queue.hpp
)
set_target_properties(libconcurrent PROPERTIES LINKER_LANGUAGE CXX)