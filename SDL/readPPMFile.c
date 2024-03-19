#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

void writeImageTest();
void readImageTest();

int main(void)
{
    time_t t;
    srand((unsigned) time(&t));
    // writeImageTest();
    readImageTest();
    return EXIT_SUCCESS;
}

void writeImageTest()
{
    const int dimx = 800, dimy = 800;
    int i, j;
    FILE *fp = fopen("jeux-video/testData/imageLoad.ppm", "wb"); /* b - binary mode */
    (void) fprintf(fp, "P6\n%d %d\n255\n", dimx, dimy);
    for (j = 0; j < dimy; ++j)
    {
        for (i = 0; i < dimx; ++i)
        {
            static unsigned char color[3];
            color[0] = i % 256;  /* red */
            color[1] = j % 256;  /* green */
            color[2] = (i * j) % 256;  /* blue */
            (void) fwrite(color, 1, 3, fp);
        }
    }
  (void) fclose(fp);
}

void readImageTest()
{
    int width, height;
    FILE *fp = fopen("jeux-video/testData/imageLoad.ppm", "r");
    char magicNumber[3];
    // fgets(magicNumber, 3, fp);
    fscanf(fp, "%s", magicNumber);
    printf("The magic number: %s\n", magicNumber);
    if(strcmp(magicNumber, "P6") != 0)
    {
        printf("File is not a ppm, or otherwise accepted format\n");
        abort();
    }
    printf("continuing\n");

    width = -1;
    fscanf(fp, "%i", &width);
    fscanf(fp, "%i", &height);
    printf("width & height: %i %i\n", width, height);

    char maxSizeSTR[3];
    fscanf(fp, "%s[\n]", maxSizeSTR);
    printf("max size int: %s\n", maxSizeSTR);
    if(strcmp(maxSizeSTR, "255") != 0)
    {
        printf("File's data is not formatted in expected way\n");
        abort();
    }


    uint8_t *data = malloc(width*height); // or whaterver size
    fread(data,width*height,1,fp);

    printf("data: %i", data[rand() % (height*width)]);


}
