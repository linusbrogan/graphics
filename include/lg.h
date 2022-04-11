#ifndef LG_H
#define LG_H

#define LG_USE_3D

#ifdef LG_USE_3D
#include "../lib/lg3d.c"
#else
#include "../lib/lg.c"
#endif

#endif
