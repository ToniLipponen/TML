#include "BufferFlags.h"

#ifdef TML_USE_GLES
    #include "GLES/VertexBuffer.h"
#else
    #include "GL4/VertexBuffer.h"
#endif