#pragma
#include "manipimage.h"
#include "stdio.h"

int main()
{
    tImage im = chargePpm("image1.ppm");
    sauvePpm("im.ppm", im);
    tImage im1 = copieImage(im);
    sauvePpm("im1.ppm", im1);
    return 0;
}