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

    user/FreeLogger.cpp
    user/FreeLogger.hpp
    user/LogEvents.hpp
    user/LogFields.hpp
    user/Logger.hpp
    user/Protocol.hpp

    LogLevel.cpp
    LogLevel.hpp
)