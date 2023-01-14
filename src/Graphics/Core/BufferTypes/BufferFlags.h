#pragma once

#define BUFFER_USAGE_FLAG GL_STATIC_DRAW

/** Currently not used for anything. This is just left over from my experiments with mapped buffers. */
#define BUFFER_MAP_FLAGS (GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT | GL_MAP_INVALIDATE_BUFFER_BIT)