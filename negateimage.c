#include<stdio.h>
#include<stdlib.h>
#include<omp.h>
#include<gd.h>

glImagePtr* img;

int main()
{
    img = gdImageCreateFromPng(fopen("input.png", "r"));

    int width  = gdImageSX(img);
    int height = gdImageSY(img);

    int x,y;
    int color, avg;

    double t = omp_get_wtime();
    // schedule -> default(don't include schedule clause), static, dynamic, guided
    #pragma omp parallel for private(x,y,color,avg) schedule(guided,50) num_threads(4)
    for(x=0; x<w; x++)
        for(y=0; y<h; y++)
        {
            color = gdImageGetPixel(img, x, y);
            avg = (gdImageReg(img, color)+gdImageGreen(img, color)+gdImageBlue(img, color))/3;
            color = gdImageColorAllocate(img, avg, avg, avg);

            #pragma omp critical
            gdImageSetPixel(img, x, y, color);
        }
    
    gdImagePng(img, fopen("output.png", "w"));

    printf("time taken = %lf\n", omp_get_wtime()-t);

    return 0;
}