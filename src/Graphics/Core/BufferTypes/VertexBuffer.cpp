#include "../../../Headers/GLHeader.h"
#include <TML/Graphics/Core/Buffers.h>

#if defined(TML_USE_GLES) || defined(TML_DONT_USE_DSA)
    #include "GLES/VertexBuffer.h"
#else
    #include "GL4/VertexBuffer.h"
#endif