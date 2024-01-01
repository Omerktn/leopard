add_executable(logMutex logMutex.cpp)

set_target_properties(logMutex PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    LINKER_LANGUAGE CXX
)

target_link_libraries(logMutex PRIVATE 
Threads::Threads
Boost::program_options
libcore
libcommon
liblogger
libconcurrent
)#