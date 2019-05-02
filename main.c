#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#include "vector.h"
#include "bmpGen.h"
#include "camera.h"
#include "scene.h"
#include "vector.h"
#include "rtx.h"

#define PI 3.1415926535

int main(){
    const int rpp = 15; // rays per pixel
    const int width = 1000; //Width and height of the image generated
    const int height = 1000;

    camera cam;

    cam.fov.x = 110; //Field of view of the y direction in degrees
    cam.fov.y = 110;

    cam.pos.x = -2;
    cam.pos.y = 0;
    cam.pos.z = -1;

    cam.dir.x = 80;
    cam.dir.y = 5;

    scene * scene = sceneGen("scene1.scene");

    unsigned char image[height][width][3];
    char* imageFileName = "results/rtx.bmp";
    pixel pixels[rpp];
    angle fov_rad;
    angle dir_rad;
    fov_rad.x = (cam.fov.x/180)*PI;
    dir_rad.x = (cam.dir.x/180)*PI;
    fov_rad.y = (cam.fov.y/180)*PI;
    dir_rad.y = (cam.dir.y/180)*PI;
    #pragma omp parallel for collapse(2)
    for (int i=0; i<height; i++){
        for (int j=0; j<width; j++){
            for (int k=0; k<rpp; k++) {
                double yaw = ((((k/rpp)+j)/(double)width)-.5)*fov_rad.x + dir_rad.x;
                double pitch = ((((k/rpp)+i)/(double)height)-.5)*fov_rad.y + dir_rad.y;
                vector direction = spVector(1, yaw, pitch);
                pixels[k] = ray(cam.pos, direction, scene, 20);
            }
            pixel final_pixel = pixelAverage(pixels, rpp);
            image[i][j][2] = final_pixel.r * 255;
            image[i][j][1] = final_pixel.g * 255;
            image[i][j][0] = final_pixel.b * 255;
        }
    }

    generateBitmapImage((unsigned char *)image, height, width, imageFileName);
    printf("Image generated and stored at %s\n", imageFileName);
    return 0;
}
