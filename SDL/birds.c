#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

SDL_Renderer *renderer;
SDL_Window *window;
SDL_Event event;

int mouseX;
int mouseY;

struct bird{
        float x;
        float y;
        float vx;
        float vy;
    };
int lcgval = 45;
int lcg() {
    lcgval = (91283476*lcgval)%318278957;
    return lcgval;
}
int main()
{
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(1024, 1024, 0, &window, &renderer);
    SDL_RenderSetLogicalSize(renderer, 1024, 1024);
    SDL_RenderClear(renderer);


    struct bird birds[1000];
    for(int i = 0; i < 1000; i++) {
        birds[i].x = abs(lcg() % 1024);
        birds[i].y = abs(lcg() % 1024);
        birds[i].vx = abs(lcg() % 1024);
        birds[i].vy = abs(lcg() % 1024);
    }
    printf("%f\n", birds[lcg() % 1000].x);
    bool notClosed = true;
    SDL_SetRenderDrawColor(renderer, 255,255,255,255);
    while(notClosed) {
        for(int i = 0; i < 1000;i++) {
            birds[i].x = mouseX+10;
            birds[i].y = mouseY+10;
            SDL_RenderDrawPoint(renderer, birds[i].x, birds[i].y);
        }
        printf("%i\n",mouseX);
        printf("%i\n", mouseY);
        SDL_RenderPresent(renderer);

        while(SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                notClosed = false;
                break;
            case SDL_MOUSEMOTION:
                mouseX = event.motion.x;
                mouseY = event.motion.y;
                break;
            }
        }
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;

}
