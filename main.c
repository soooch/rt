#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#include "vector.h"
#include "bmpGen.h"
#include "camera.h"
#include "scene.h"
#include "vector.h"
#include "oldrtx.h"

#define PI 3.1415926535

int main(){
    const int rpp = 50; // rays per pixel
    const int width = 200; //Width and height of the image generated
    const int height = 200;

    camera cam;

    cam.fov.x = 90; //Field of view of the y direction in degrees
    cam.fov.y = 90;

    cam.pos.x = -3;
    cam.pos.y = 0;
    cam.pos.z = -3;

    cam.dir.x = 30;
    cam.dir.y = 5;

    cam.range = .65;

    scene * scene = sceneGen("scene1.scene");

    unsigned char image[height][width][3];
    char* imageFileName = "results/rtx.bmp";
    angle fov_rad;
    angle dir_rad;
    fov_rad.x = (cam.fov.x/180)*PI;
    dir_rad.x = (cam.dir.x/180)*PI;
    fov_rad.y = (cam.fov.y/180)*PI;
    dir_rad.y = (cam.dir.y/180)*PI;
    printf(" 00%% [          ]");
    int num_pix = 0;
    int progress = 0;
    int progress_plus = 1;
    #pragma omp parallel for collapse(2)
    for (int i=0; i<height; i++){
        for (int j=0; j<width; j++){
            pixel pixels[rpp];
            for (int k=0; k<rpp; k++) {
                double yaw = ((double)((double)j/(double)width)-.5)*fov_rad.x + dir_rad.x;
                double pitch = ((double)((double)i/(double)height)-.5)*fov_rad.y + dir_rad.y;
                vector direction = spVector(1, yaw, pitch);
                pixels[k] = ray(cam.pos, normalizeVector(perturbVector(direction, .01)), scene, 10);
            }
            pixel final_pixel = pixelAverage(pixels, rpp);
            image[i][j][2] = expose(final_pixel.r, cam.range);
            image[i][j][1] = expose(final_pixel.g, cam.range);
            image[i][j][0] = expose(final_pixel.b, cam.range);
			num_pix++;
            progress = (double)((double)num_pix/(double)(width*height))*10;
            printf("\r %d0%% [", progress);
            if (progress >= progress_plus) {
                for (int i = 1; i < progress + 1; i++) {
                    printf("█");
                }
                progress_plus = progress + 1;
            }
        }
    }
    printf("\r 100%% [██████████]");
    generateBitmapImage((unsigned char *)image, height, width, imageFileName);
    printf("\nImage generated and stored at %s\n", imageFileName);
    return 0;
}
