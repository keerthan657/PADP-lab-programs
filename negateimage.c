#include<stdio.h>
#include<stdlib.h>
#include<omp.h>
#include<gd.h>

int main()
{
    gdImagePtr img;    
    img = gdImageCreateFromPng(fopen("download.png", "r"));

    int width = gdImageSX(img);
    int height = gdImageSY(img);
    int x, y, color, avg;

    #pragma omp parallel for num_threads(4) private(x,y,color,avg) schedule(guided,50)
    for(x=0; x<width; x++)
    {
        for(y=0; y<height; y++)
        {
            color = gdImageGetPixel(img, x,y);
            avg = (gdImageRed(img,color)+gdImageGreen(img,color)+gdImageBlue(img,color))/3;
            color = gdImageColorAllocate(img, avg,avg,avg);
            
            #pragma omp critical
            gdImageSetPixel(img, x,y,color);
        }
    }
    
    gdImagePng(img, fopen("output.png", "w"));
    gdImageDestroy(img);
    return 0;
}
