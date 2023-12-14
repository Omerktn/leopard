add_library(libconcurrent STATIC
    BufferQueue.hpp
    ElementQueue.hpp
)
set_target_properties(libconcurrent PROPERTIES LINKER_LANGUAGE CXX)