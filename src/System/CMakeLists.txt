cmake_minimum_required(VERSION 3.0)
project(tml-system VERSION ${TML_API_VERSION})

add_library(tml-system ${TML_LIBRARY_TYPE}
        Serializer/Deserializer.cpp
        Serializer/Serializer.cpp
        File.cpp
        Compression.cpp
        Clipboard.cpp
        Image.cpp
        Logger.cpp)

if(WIN32)
    target_link_libraries(tml-system glfw lunasvg fmt fastlz)
else()
    target_link_libraries(tml-system glfw lunasvg fmt fastlz rt)
endif()

set_target_properties(tml-system PROPERTIES VERSION ${TML_API_VERSION} SOVERSION ${TML_API_VERSION})
