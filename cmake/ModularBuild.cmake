add_subdirectory(src/Audio)
add_subdirectory(src/Graphics)
add_subdirectory(src/Network)
add_subdirectory(src/System)
add_subdirectory(src/Window)

if(TML_BUILD_EXPERIMENTAL)
    add_subdirectory(src/Interface)
endif()

add_library(tml src/TOKEN_SOURCE_FILE.cpp ${HEADER_FILES})
target_link_libraries(tml
        tml-audio
        tml-graphics
        tml-interface
        tml-network
        tml-system
        tml-window)

if(UNIX)
    install(TARGETS tml-audio tml-graphics tml-window tml-system tml-network DESTINATION /usr/lib)
    install(DIRECTORY include/TML DESTINATION /usr/include)
endif()