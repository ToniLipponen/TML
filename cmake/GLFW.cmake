set(GLFW_LIBRARY_TYPE OBJECT)
set(GLFW_INSTALL no)
set(GLFW_BUILD_DOCS no)
set(GLFW_BUILD_TESTS no)
set(GLFW_BUILD_EXAMPLES no)

if(UNIX)
    set(GLFW_BUILD_X11 yes)
endif()