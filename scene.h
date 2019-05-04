#ifndef SCENE_H
#define SCENE_H
#include "vector.h"
#include "camera.h"

//The most basic type of thing we will be doing. To be improved upon if we feel like it
//First off, do the easy stuff, then do the more complicated stuff that sooch is talking about
//The surface will be defined by three points, so it is a triangle. The classic shape
//The front of the surface will be defined as the counterclockwise direction, so a triangle
//with coords a = (0,0,0), b = (0,1,0), c = (1,1,1), then you will be looking at the front
//of it when you are looking in the -z direction
typedef struct TRIANGLE {
    vector a;
    vector b;
    vector c;
    vector normal;
    pixel color;
    double diffuse;
    int emissive;
} triangle;

typedef struct BOX {
    vector min;
    vector max;
    int is_leaf;
    struct BOX * children[2];
    int tri_count;                 // usually 12
    triangle * triangles[];
} box;

typedef struct SCENE {
    int obj_count;
    object * objects[];
} scene;

// Returns the normal vector of triangle t
vector normalTriangle(triangle t) {
    vector i = {t.b.x - t.a.x, t.b.y - t.a.y, t.b.z - t.a.z};
    vector j = {t.c.x - t.a.x, t.c.y - t.a.y, t.c.z - t.a.z};
    vector n = normalizeVector(crossVector(i, j));
    return n;
}

// Takes in a triangle and a vector, and transforms that triangle in the direction of the vector
void translateObject(object * obj, vector v) {
    for (int i = 0; i < obj->tri_count; i++) {
        obj->triangles[i]->a.x += v.x;
        obj->triangles[i]->b.x += v.x;
        obj->triangles[i]->c.x += v.x;
        obj->triangles[i]->a.y += v.y;
        obj->triangles[i]->b.y += v.y;
        obj->triangles[i]->c.y += v.y;
        obj->triangles[i]->a.z += v.z;
        obj->triangles[i]->b.z += v.z;
        obj->triangles[i]->c.z += v.z;
    }
}

object * objGen(char * obj_filename) {
    FILE * objfile = fopen(obj_filename, "r");

    pixel color;
    double diffuse;
    int emissive;
    vector translate;
    int vertex_count;
    int tri_count;
    // reading in custom header from .object file
    fscanf(objfile, "%lf %lf %lf %lf %d %lf %lf %lf %d %d", &color.r, &color.g, &color.b, &diffuse, &emissive, &translate.x, &translate.y, &translate.z, &vertex_count, &tri_count);

    object * obj = malloc(sizeof(object) + tri_count * sizeof(triangle*));
    obj->color = color;
    obj->diffuse = diffuse;
    obj->emissive = emissive;
    obj->tri_count = tri_count;
    // make an array of all vertexes
    vector vertexes[vertex_count];
    for (int i = 0; i < vertex_count; i++) {
        fscanf(objfile, "%lf %lf %lf *", &vertexes[i].x, &vertexes[i].y, &vertexes[i].z);
    }
    // make the appropriate triangles using the instructions and vertex array
    for (int i = 0; i < tri_count; i++) {
        int tri, a, b, c;
        fscanf(objfile, "%d %d %d %d *", &tri, &a, &b, &c);
        obj->triangles[i] = malloc(sizeof(triangle));
        obj->triangles[i]->a = vertexes[a];
        obj->triangles[i]->b = vertexes[b];
        obj->triangles[i]->c = vertexes[c];
        obj->triangles[i]->normal = normalTriangle(*obj->triangles[i]);
    }
    fclose(objfile);
    translateObject(obj, translate);
    return obj;
}

scene * sceneGen(char * scene_filename) {
    FILE * scenefile = fopen(scene_filename, "r");
    int obj_count;
    fscanf(scenefile, "%d *", &obj_count);
    scene * scene = malloc(sizeof(scene) + obj_count * sizeof(object*));
    scene->obj_count = obj_count;
    char obj_filename[20];
    for (int i = 0; i < obj_count; i++) {
        fscanf(scenefile, "%s *", obj_filename);
        scene->objects[i] = objGen(obj_filename);
    }
    fclose(scenefile);
    return scene;
}
#endif
