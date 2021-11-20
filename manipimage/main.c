#include "manipimage.h"
#include "stdio.h"

int main()
{
    char cwd[256];
    if (getcwd(cwd, sizeof(cwd)) != 0) {
        printf("Current working dir: %s\n", cwd);
    }

    tImage im = chargePpm1("image1.ppm");
    sauvePpm("im.ppm", im);
    tImage im1 = copieImage(im);
    sauvePpm("im1.ppm", im1);
    return 0;
}