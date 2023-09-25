# Including external libraries
add_subdirectory(external/glfw)
add_subdirectory(external/lunasvg)
add_subdirectory(external/glad)
add_subdirectory(external/fastlz)
add_subdirectory(external/fmt)

# Options for external libraries
include(cmake/GLFW.cmake)
include(cmake/lunasvg.cmake)
include(cmake/fmt.cmake)
