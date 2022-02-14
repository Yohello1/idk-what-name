#include <stdio.h>
#define STB_TRUETYPE_IMPLEMENTATION // force following include to generate implementation
#include "stb_truetype.h"

// gcc -o hello text_test.c -lm && ./hello

char buffer[24 << 20];

#define vert 20
#define horz 150

// const int vert = 40;
// const int horz = 150;

unsigned char screen[vert][horz];

int main(int arg, char **argv)
{
    stbtt_fontinfo font;
    int i, j, ascent, baseline, ch = 0;
    float scale, xpos = 2;              // leave a little padding in case the character extends left
    char *text = "Hi I am a bagguette"; // intentionally misspelled to show 'lj' brokenness

    fread(buffer, 1, 1000000, fopen("font/Hack-Regular.ttf", "rb"));
    stbtt_InitFont(&font, buffer, 0);

    scale = stbtt_ScaleForPixelHeight(&font, 15);
    stbtt_GetFontVMetrics(&font, &ascent, 0, 0);
    baseline = (int)(ascent * scale);

    while (text[ch])
    {
        // Cords for each thing
        int advance, lsb, x0, y0, x1, y1;

        // Gets the x_pos
        float x_shift = xpos - (float)floor(xpos);

        // Gets the data for this specific chacter
        stbtt_GetCodepointHMetrics(&font, text[ch], &advance, &lsb);

        // How big is it gonna be huh?
        stbtt_GetCodepointBitmapBoxSubpixel(&font, text[ch], scale, scale, x_shift, 0, &x0, &y0, &x1, &y1);

        // Actually encode it?
        stbtt_MakeCodepointBitmapSubpixel(&font, &screen[baseline + y0][(int)xpos + x0], x1 - x0, y1 - y0, horz, scale, scale, x_shift, 0, text[ch]);

        // note that this stomps the old data, so where character boxes overlap (e.g. 'lj') it's wrong
        // because this API is really for baking character bitmaps into textures. if you want to render
        // a sequence of characters, you really need to render each bitmap to a temp buffer, then
        // "alpha blend" that into the working buffer

        // This is some recursive shid, dont touch it OK
        xpos += (advance * scale);
        if (text[ch + 1])
            xpos += scale * stbtt_GetCodepointKernAdvance(&font, text[ch], text[ch + 1]);
        ++ch;
    }

    // for (j = 0; j < vert; ++j)
    // {
    //     for (i = 0; i < horz; ++i)
    //     {
    //         if (screen[j][i] >> 5 != ' ')
    //         {
    //             // putchar(" ");
    //             printf("#");
    //         }
    //         else
    //         {
    //             printf(" ");

    //             // putchar("#");
    //         }
    //         // putchar(" .:ioVM@"[screen[j][i] >> 5]);
    //     }
    //     putchar('\n');
    // }

    printf("Hello \n");
    putchar(screen[10][10]);
    printf("\n");

    // for(int i = 0; i < vert*horz; i++)
    // {
    //     printf("%c", screen[i]);
    // }

    return 0;
}