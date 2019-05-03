#ifndef VECTOR_H
#define VECTOR_H

#include <stdio.h>
#include <math.h>
#include <time.h>

// Struct for vector, it's just one point cause that's what a vector is
typedef struct VECTOR {
    double x;
    double y;
    double z;
} vector;

// Returns the scalar product of A * m where A is a vector and m is a scalar
vector multVector(vector A, double mult) {
    vector v;
    v.x = A.x * mult;
    v.y = A.y * mult;
    v.z = A.z * mult;
    return v;
}

// Returns the dot product of vectors A and B
double dotVector(vector A, vector B) {
    return A.x * B.x + A.y * B.y + A.z * B.z;
}

// Returns the cross product of A x B
vector crossVector(vector A, vector B) {
    vector v;
    v.x = A.y * B.z - A.z * B.y;
    v.y = -(A.x * B.z - A.z * B.x);
    v.z = A.x * B.y - A.y * B.x;
    return v;
}

// Returns A + B
vector addVector(vector A, vector B) {
    vector v;
    v.x = A.x + B.x;
    v.y = A.y + B.y;
    v.z = A.z + B.z;
    return v;
}

// Returns A - B
vector subtractVector(vector A, vector B) {
    return addVector(A, multVector(B, -1));
}

// Returns the magnitude of the vector
double normVector(vector A) {
    return sqrt(A.x * A.x + A.y * A.y + A.z * A.z);
}

//Returns the normalized vector of A, which has the same direction, but
//has a magnitude of 1
vector normalizeVector(vector A) {
    double norm = normVector(A);
    return multVector(A, 1 / norm);
}

vector randVector() {
    vector randV = {rand() % 10, rand() % 10, rand() % 10};
    return normalizeVector(randV);
}

// should offset vector by up to 90 degrees to any side. rand is double that
// represents randomness from 0 to 1
// yaw and pitch up to PI/2
// multiplied by rand
vector perturbVector(vector A, double diffuse) {
    vector rand_vector = randVector();
    vector cross_vector = normalizeVector(crossVector(A, rand_vector));
    double s = ((double)(rand() % 1001)) / 1000;
    double r = ((double)(rand() % 1001)) / 1000;
    double rand_theta = (((double)(rand() % 1001))/1000)*.5*M_PI*diffuse;
    double h = cos(rand_theta);
    double phi = 2 * M_PI * s;
    double z = h + (1-h) * r;
    double sinT = sqrt(1-z*z);
    double x = cos(phi) * sinT;
    double y = sin(phi) * sinT;
    return addVector(addVector(multVector(rand_vector, x), multVector(cross_vector, y)), multVector(A, z));
}

vector bounceVector(vector ray, vector normal) {
    return subtractVector(ray, (multVector(normal, dotVector(ray, normal) * 2)));
}

// Returns the distance between vector A and B, always positive
double dist(vector A, vector B) {
    vector v;
    v.x = A.x - B.x;
    v.y = A.y - B.y;
    v.z = A.z - B.z;
    return normVector(v);
}

// Returns the angle in radians between vectors A and B
double angleVector(vector A, vector B) {
    A = normalizeVector(A);
    B = normalizeVector(B);
    return acos(dotVector(A,B));
}

// Returns a vector when spherical coordinates are given
// Angles must be given in radians
// Spherical coordinates here will be defined as follows:
// r is the magnitude of the created vector
// yaw is the direction of the vector in the x-z plane,
// with 0 pointing in the x direction, pi / 2 in the z direction, etc...
// pitch will determine the direction in the y position. When pitch is
// pi/2, the vector points in the +y direction, and vice versa
vector spVector(double r, double yaw, double pitch) {
    vector v;
    v.x = r * sin(yaw) * cos(pitch);
    v.y = r * sin(pitch);
    v.z = r * cos(yaw) * cos(pitch);
    return v;
}

// Prints the contents of a vector in the format
// (x, y, z)
void printVector(vector A) {
    printf("(%lf, %lf, %lf)\n", A.x, A.y, A.z);
}

#endif
