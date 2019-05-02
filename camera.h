#ifndef CAMERA_H
#define CAMERA_H

#include "vector.h"
typedef struct ANGLE {
    double x,y;
} angle;

typedef struct CAMERA {
    angle fov;
    angle dir; // offset from straight ahead
    vector pos;
} camera;

typedef struct PIXEL {
    double r,g,b;
} pixel;

#endif
