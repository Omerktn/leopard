add_subdirectory(concurrent)

add_library(libcommon STATIC
    Buffer.hpp
    Chrono.hpp
    FixedDecimal.hpp
    FixedString.hpp
    Functional.hpp
    Math.hpp
    StringOperations.hpp
    Sugars.hpp
    Types.cpp
    Types.hpp
    TypeTraits.hpp
    Utility.hpp
    Variant.hpp
)