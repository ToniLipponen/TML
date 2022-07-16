file(GLOB_RECURSE MONOLITHIC_SRC src/*.cpp)

add_library(tml-monolithic ${TML_LIBRARY_TYPE} ${MONOLITHIC_SRC} ${HEADER_FILES})
target_link_libraries(tml-monolithic glfw lunasvg fastlz glad)
set_target_properties(tml-monolithic
        PROPERTIES
        VERSION
        ${TML_API_VERSION} SOVERSION ${TML_API_VERSION})

if(UNIX)
    install(TARGETS tml-monolithic DESTINATION /usr/lib)
    install(DIRECTORY include/TML DESTINATION /usr/include)
endif()