#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <stdio.h>
 
int main(int argc, char *argv[])
{
 
    // returns zero on success else non-zero
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("error initializing SDL: %s\n", SDL_GetError());
    }
    SDL_Window* win = SDL_CreateWindow("GAME", // creates a window
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       240, 240, SDL_WINDOW_FULLSCREEN);
    printf("window created\n");
    // triggers the program that controls
    // your graphics hardware and sets flags
    Uint32 render_flags = SDL_RENDERER_SOFTWARE;
 
    // creates a renderer to render our images
    SDL_Renderer* rend = SDL_CreateRenderer(win, -1, render_flags);
    printf("Renderer created\n");
    // creates a surface to load an image into the main memory
    SDL_Surface* surface;
 
    // please provide a path for your image
    surface = IMG_Load("/data/mario.png");
    printf("Image loaded\n");
 
    // loads image to our graphics hardware memory.
    SDL_Texture* tex = SDL_CreateTextureFromSurface(rend, surface);
    printf("Texture created\n");
    // clears main-memory
    SDL_FreeSurface(surface);
 
    // let us control our image position
    // so that we can move it with our keyboard.
    SDL_Rect dest;
    SDL_Rect screen;
    screen.x = 0;
    screen.y = 0;
    screen.w = 240;
    screen.h = 240;
 
    // connects our texture with dest to control position
    SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);
 
    // sets initial x-position of object
    dest.x = (240 - dest.w) / 2;
 
    // sets initial y-position of object
    dest.y = (240 - dest.h) / 2;
 
    // controls animation loop
    int close = 0;
 
    // speed of box
    int speed = 300;

    printf("Reached loop\n");
 
    // animation loop
    while (!close) {
        SDL_Event event;
 
        // Events management
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
 
            case SDL_QUIT:
                // handling of close button
                close = 1;
                break;
 
            case SDL_KEYDOWN:
                // keyboard API for key pressed
                switch (event.key.keysym.scancode) {
                case SDL_SCANCODE_W:
                case SDL_SCANCODE_UP:
                    dest.y -= 4;
                    break;
                case SDL_SCANCODE_A:
                case SDL_SCANCODE_LEFT:
                    dest.x -= 4;
                    break;
                case SDL_SCANCODE_S:
                case SDL_SCANCODE_DOWN:
                    dest.y += 4;
                    break;
                case SDL_SCANCODE_D:
                case SDL_SCANCODE_RIGHT:
                    dest.x += 4;
                    break;
                default:
                    break;
                }
            }
        }
 
        // right boundary
        if (dest.x + dest.w > 240)
            dest.x = 240 - dest.w;
 
        // left boundary
        if (dest.x < 0)
            dest.x = 0;
 
        // bottom boundary
        if (dest.y + dest.h > 240)
            dest.y = 240 - dest.h;
 
        // upper boundary
        if (dest.y < 0)
            dest.y = 0;
 
        // clears the screen
        SDL_RenderClear(rend);
        SDL_SetRenderDrawColor(rend, 100, 100, 100, 255);
        SDL_RenderFillRect(rend, &screen);
        SDL_RenderCopy(rend, tex, NULL, &dest);
        printf("Clear/Copy render\n");
 
        // triggers the double buffers
        // for multiple rendering
        SDL_RenderPresent(rend);
        printf("Presenting render\n");
 
        // calculates to 60 fps
        SDL_Delay(1000 / 60);
    }
 
    // destroy texture
    SDL_DestroyTexture(tex);
 
    // destroy renderer
    SDL_DestroyRenderer(rend);
 
    // destroy window
    SDL_DestroyWindow(win);
     
    // close SDL
    SDL_Quit();
    printf("SDL quit\n");
 
    return 0;
}