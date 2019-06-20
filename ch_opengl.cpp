#include "ch_opengl.h"

#define DEFINE_GL_FUNCTIONS(type, func) type func = nullptr;
GL_BINDINGS(DEFINE_GL_FUNCTIONS);
#undef DEFINE_GL_FUNCTIONS