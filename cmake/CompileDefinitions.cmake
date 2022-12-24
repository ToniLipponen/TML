if(TML_BUILD_STATIC)
    add_compile_definitions(TML_STATIC)
    set(TML_LIBRARY_TYPE STATIC)
else()
    if(WIN32 OR Windows)
        add_compile_definitions(TML_EXPORTING)
    endif()

    set(TML_LIBRARY_TYPE SHARED)
endif()

if(NOT TML_ASSERT)
    add_compile_definitions(TML_NO_DEBUGGING=1)
endif()

if(NOT TML_DEBUG_OPENGL_CALLS)
    add_compile_definitions(TML_NO_GL_DEBUGGING=1)
endif()

if(TML_USE_GLES)
    add_compile_definitions(TML_USE_GLES GLAD_GLES2)
endif()

if(TML_USE_DSA)
    add_compile_definitions(TML_USE_DSA=1)
endif()