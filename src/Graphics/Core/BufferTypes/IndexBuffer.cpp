#include "BufferFlags.h"

#ifdef TML_USE_GLES
    #include "GLES/IndexBuffer.h"
#else
    #include "GL4/IndexBuffer.h"
#endif