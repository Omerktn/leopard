add_library(liblogger STATIC
    server/Server.hpp
)

set_target_properties(liblogger PROPERTIES LINKER_LANGUAGE CXX)