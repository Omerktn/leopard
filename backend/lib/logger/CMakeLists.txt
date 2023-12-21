add_library(liblogger STATIC
    formattedText/Decoder.hpp
    formattedText/FormattedTextEvent.hpp
    formattedText/Formatter.cpp
    formattedText/Formatter.hpp

    server/Decoder.cpp
    server/Decoder.hpp
    server/Server.cpp
    server/Server.hpp
    server/Utils.cpp
    server/Utils.hpp
)

#set_target_properties(liblogger PROPERTIES LINKER_LANGUAGE CXX)