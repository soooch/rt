#ifndef RTX_H
#define RTX_H

#include "vector.h"
#include "scene.h"
#include "camera.h"

// Takes in a triangle, and a line with a position and a direction vector
// If there is a collision, it will return the distance value of the line to
// the triangle. If there is no collision, it will return -1. The direction vector
// must be normalized for this to work, and must point in the direction of the
// triangle, or else it will count as no collision.
double collision(triangle t, vector pos, vector dir) {
    // Let's find the equation of the plane parallel to the triangle, in Ax + By + Cz = D form
    // We know that the normal vector give us A, B, and C
    // TODO: switch to prebaked normals
    //printf("Normal: ");
    //printVector(normal);
    double A = t.normal.x;
    double B = t.normal.y;
    double C = t.normal.z;
    double D = A * t.a.x + B * t.a.y + C * t.a.z;
    //printf("%f\n", D);
    //D = A * triB.x + B * triB.y + C * triB.z;
    //printf("%f\n", D);

    // We parametrized the line to be pos + dir * t, with t being a real number
    // parameter, so we have to find t with this equation which we derived from
    // substituting x,y,z with the components of the parametrized line, and then
    // we solved for t.
    double param = (D - A * pos.x - B * pos.y - C * pos.z) / (A * dir.x + B * dir.y + C * dir.z);
    //printf("Param: %f\n", param);
    if (param < 0) {
        return -1;
    }

    // Inter is the intersection of the plane and the point
    vector inter = addVector(pos, multVector(dir, param));
    /*
    // points from point A to inter
    vector interDirection = subtractVector(inter, triA);
    // Points from B to C (triangle vertices)
    vector cbDirection = subtractVector(triC, triB);
    */

    // Here we use Kavi's Algorithm: www.wikipedia.org/Kavis_Algorithm.html
    vector abDirection = subtractVector(t.b, t.a);
    vector acDirection = subtractVector(t.c, t.a);
    vector bcDirection = subtractVector(t.c, t.b);

    double angleBAC = angleVector(abDirection, acDirection);
    double angleABC = angleVector(multVector(abDirection, -1), bcDirection);
    //double angleACB = angleVector(acDirection, bcDirection);

    double angleIAB = angleVector(subtractVector(inter, t.a), abDirection);
    double angleIAC = angleVector(subtractVector(inter, t.a), acDirection);
    double angleIBC = angleVector(subtractVector(inter, t.b), bcDirection);
    double angleIBA = angleVector(subtractVector(inter, t.b), multVector(abDirection, -1));
    //printf("Intersection to plane: ");
    //printVector(inter);
    if (angleIAB > angleBAC || angleIAC > angleBAC || angleIBC > angleABC || angleIBA > angleABC) {
        return -1;
    }
    return param;

    //printf("%f %f %f \n", angleBAC, angleABC, angleACB);
    //printf("Angle sum: %f", angleBAC + angleABC + angleACB);
}

pixel multPixel(pixel a, pixel b) {
    pixel c;
    c.r = a.r * b.r;
    c.g = a.g * b.g;
    c.b = a.b * b.b;
    return c;
}

pixel ray(vector position, vector direction, scene * cur_scene, int bounces) {
    pixel final = {0, 0, 0};
    if (bounces <= 0) {
        return final;
    }
    double closest_dist = -1;
    triangle * closest_tri;
    object * closest_obj;
    for (int i=0; i<cur_scene->obj_count; i++) {
        for (int j = 0; j < cur_scene->objects[i]->tri_count; j++) {
            double dist = collision(*cur_scene->objects[i]->triangles[j], position, direction);
            if (dist > 0) {
                // TODO: actually take into account obj characteristics
                // TODO: figure out how to get other objects illuminated
                if (dist < closest_dist || closest_dist < 0) {
                    closest_tri = cur_scene->objects[i]->triangles[j];
                    closest_obj = cur_scene->objects[i];
                    closest_dist = dist;
                }
            }
        }
    }
    if (closest_dist > 0) {
    if (closest_obj->emissive) {
        return closest_obj->color;
    }
    vector new_pos = addVector(position, multVector(normalizeVector(direction), closest_dist));
    final = multPixel(
            closest_obj->color, 
            ray(new_pos, 
                perturbVector(bounceVector(direction, closest_tri->normal), closest_obj->diffuse), 
                //bounceVector(direction, closest_tri->normal), 
                cur_scene, bounces-1)
            );
    }
    return final;
}

pixel pixelAverage(pixel * pixels, int rpp) {
    // average pixels
    pixel retpix = {0, 0, 0};

    // add all squares
    for (int i=0; i<rpp; i++) {
        retpix.r += pixels[i].r * pixels[i].r;
        retpix.g += pixels[i].g * pixels[i].g;
        retpix.b += pixels[i].b * pixels[i].b;
    }

    // mean values
    retpix.r /= rpp;
    retpix.g /= rpp;
    retpix.b /= rpp;

    // sqrt all values
    retpix.r = sqrt(retpix.r);
    retpix.g = sqrt(retpix.g);
    retpix.b = sqrt(retpix.b);

    return retpix;
}
#endif
