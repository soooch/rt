//Testing environment, use make test to create this. This is used as an override for the
//main file, thats all it does.

#include "vector.h"
#include "triangle.h"

int main() {
    triangle t;
    t.a[0] = 0;
    t.a[1] = 0;
    t.a[2] = .1;
    t.b[0] = 1;
    t.b[1] = 0;
    t.b[2] = 0;
    t.c[0] = 0;
    t.c[1] = 1;
    t.c[2] = .1;
    vector v = {0, 0, -1};
    vector pos = {.3, .3, .3};
    printf("%f\n", collision(t, pos, v));
}
