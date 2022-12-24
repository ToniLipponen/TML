
add_subdirectory(src/System)
set(BUILT_MODULES_LIST ${BUILT_MODULES_LIST} tml-system)

if(TML_BUILD_MODULE_AUDIO)
    add_subdirectory(src/Audio)
    set(BUILT_MODULES_LIST ${BUILT_MODULES_LIST} tml-audio)
endif()

if(TML_BUILD_MODULE_GRAPHICS)
    add_subdirectory(src/Graphics)
    set(BUILT_MODULES_LIST ${BUILT_MODULES_LIST} tml-graphics)
endif()

if(TML_BUILD_MODULE_INTERFACE)
    add_subdirectory(src/Interface)
    set(BUILT_MODULES_LIST ${BUILT_MODULES_LIST} tml-interface)
endif()

if(TML_BUILD_MODULE_NETWORK)
    add_subdirectory(src/Network)
    set(BUILT_MODULES_LIST ${BUILT_MODULES_LIST} tml-network)
endif()

if(TML_BUILD_MODULE_WINDOW)
    add_subdirectory(src/Window)
    set(BUILT_MODULES_LIST ${BUILT_MODULES_LIST} tml-window)
endif()

add_library(tml src/TOKEN_SOURCE_FILE.cpp ${HEADER_FILES})
target_link_libraries(tml ${BUILT_MODULES_LIST})

if(UNIX)
    install(TARGETS ${BUILT_MODULES_LIST} DESTINATION /usr/lib)
    install(DIRECTORY include/TML DESTINATION /usr/include)
endif()