add_library(libmarketdataconnector STATIC
    Config.cpp
    Config.hpp
    MarketDataConnector.hpp
)

target_link_libraries(libmarketdataconnector libcomponent)