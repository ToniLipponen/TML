include(CMakeDependentOption)

option(TML_BUILD_STATIC "Build TML as a static library" OFF)
option(TML_DEBUG_OPENGL_CALLS "Applies error checking around OpenGL function calls." OFF)
option(TML_ASSERT "Applies asserts around important functions." OFF)
option(TML_USE_GLES "Use OpenGL ES 3.1 instead of OpenGL 3.3 core and up." OFF)
option(TML_USE_DSA "Use OpenGL DSA functions" OFF)
option(TML_BUILD_MONOLITHIC "Build a single library file" OFF)
option(TML_USE_SSE "Use SSE4.1" OFF)

############################# Modules ##################################
cmake_dependent_option(TML_BUILD_MODULE_AUDIO       "Build the audio module"        ON "NOT TML_BUILD_MONOLITHIC" OFF)
cmake_dependent_option(TML_BUILD_MODULE_WINDOW      "Build the window module"       ON "NOT TML_BUILD_MONOLITHIC" OFF)
cmake_dependent_option(TML_BUILD_MODULE_GRAPHICS    "Build the graphics module"     ON "NOT TML_BUILD_MONOLITHIC; TML_BUILD_MODULE_WINDOW" OFF)
cmake_dependent_option(TML_BUILD_MODULE_NETWORK     "Build the network module"      ON "NOT TML_BUILD_MONOLITHIC" OFF)
########################################################################