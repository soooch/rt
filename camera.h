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
    double range;
} camera;

typedef struct PIXEL {
    double r,g,b;
} pixel;

unsigned char expose(double color, double range) {
    int exposed_pix = (double)((double)(1 / range) * (double)255 * (double)color);
    if (exposed_pix > 255) {exposed_pix = 255;};
    return (unsigned char)exposed_pix;
}


#endif
