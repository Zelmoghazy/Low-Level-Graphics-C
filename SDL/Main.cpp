#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#define SCREEN_WIDTH 1280 
#define SCREEN_HEIGHT 720

#define COLOR_RED        (SDL_Color){255, 0, 0, 255}
#define COLOR_GREEN      (SDL_Color){0, 255, 0, 255}
#define COLOR_BLUE       (SDL_Color){0, 0, 255, 255}
#define COLOR_WHITE      (SDL_Color){255, 255, 255, 255}
#define COLOR_BLACK      (SDL_Color){0, 0, 0, 255}
#define COLOR_YELLOW     (SDL_Color){255, 255, 0, 255}
#define COLOR_CYAN       (SDL_Color){0, 255, 255, 255}
#define COLOR_MAGENTA    (SDL_Color){255, 0, 255, 255}
#define COLOR_ORANGE     (SDL_Color){255, 165, 0, 255}
#define COLOR_PURPLE     (SDL_Color){128, 0, 128, 255}
#define COLOR_PINK       (SDL_Color){255, 192, 203, 255}
#define COLOR_GRAY       (SDL_Color){128, 128, 128, 255}
#define COLOR_LIGHT_GRAY (SDL_Color){211, 211, 211, 255}
#define COLOR_DARK_GRAY  (SDL_Color){169, 169, 169, 255}
#define COLOR_BROWN      (SDL_Color){139, 69, 19, 255}
#define COLOR_NAVY       (SDL_Color){0, 0, 128, 255}
#define COLOR_LIME       (SDL_Color){0, 255, 0, 255}
#define COLOR_TEAL       (SDL_Color){0, 128, 128, 255}
#define COLOR_MAROON     (SDL_Color){128, 0, 0, 255}
#define COLOR_OLIVE      (SDL_Color){128, 128, 0, 255}


bool running;
int lastx = 0;
int lasty = 0;

int x1 = 100;
int y1 = 100;
int x2 = 200;
int y2 = 200;
int x3 = 600;
int y3 = 100;

typedef struct Entity{
    SDL_Texture *texture;
    SDL_Rect currentFrame;
    float x_pos;
    float y_pos;
}Entity;

void renderPixel(SDL_Renderer *renderer, int* pixels,SDL_Color color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g,color.b, color.a);
    for (int x = 0; x < SCREEN_HEIGHT*SCREEN_WIDTH; x++)
    {
        if(pixels[x] == 1)
        {
            SDL_RenderDrawPoint(renderer, x%SCREEN_WIDTH,x/SCREEN_WIDTH);
        }
    }
    
}

Entity initEntity(SDL_Texture *texture, float x_pos, float y_pos){
    Entity entity;
    entity.texture = texture;
    entity.x_pos = x_pos;
    entity.y_pos = y_pos;
    entity.currentFrame.x = 0;
    entity.currentFrame.y = 0;
    entity.currentFrame.w = 469;
    entity.currentFrame.h = 314;
    return entity;
}

void renderEntity(Entity *entity, SDL_Renderer *renderer){
    SDL_Rect destRect;
    destRect.x = entity->x_pos/2;
    destRect.y = entity->y_pos/2;
    destRect.w = entity->currentFrame.w/2;
    destRect.h = entity->currentFrame.h/2;
    SDL_RenderCopy(renderer, entity->texture, &entity->currentFrame, &destRect);
}

void pollWindow(SDL_Event *event)
{
    switch (event->type)
    {
    /* Close Window */
    case SDL_QUIT:
        running = false;
        break;
    /* Mouse Clicks */
    case SDL_MOUSEBUTTONDOWN:
        if (event->button.button == SDL_BUTTON_LEFT)
        {
        }
        if (event->button.button == SDL_BUTTON_RIGHT)
        {
        }
        break;

    /* Default */
    default:
        break;
    }
}

void destroy(SDL_Window *window, SDL_Renderer *renderer){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void clearScreen(SDL_Renderer *renderer , SDL_Color color){
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(renderer);
}

SDL_Rect newRect(float x_pos, float y_pos, float width, float height){
    SDL_Rect rect = {x_pos, y_pos, width, height};
    return rect;
}

void renderRect(SDL_Renderer *renderer, SDL_Color color, SDL_Rect *rect){
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, rect);
}

void pollRect(SDL_Rect *rect, SDL_Event *event)
{

    switch (event->type)
    {
    /* Keys */
    case SDL_KEYDOWN:
        switch (event->key.keysym.sym)
        {
        case SDLK_ESCAPE:
                running = false;
                break;
        case SDLK_UP:
                rect->y -= 10;
                break;
        case SDLK_DOWN:
                rect->y += 10;
                break;
        case SDLK_RIGHT:
                rect->x += 10;
                break;
        case SDLK_LEFT:
                rect->x -= 10;
                break;
        case SDLK_SPACE:
                break;
        }
        break;

    /* Mouse Clicks */
    case SDL_MOUSEBUTTONDOWN:
        if (event->button.button == SDL_BUTTON_LEFT)
        {
            x3 = event->button.x;
            y3 = event->button.y;
        }
        if (event->button.button == SDL_BUTTON_RIGHT)
        {
            x2 = event->button.x;
            y2 = event->button.y;
        }
        break;

    /* Default */
    default:
        break;
    }
}

void DrawCircle(SDL_Renderer * renderer, int32_t centreX, int32_t centreY, int32_t radius, SDL_Color color)
{
   SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
   const int32_t diameter = (radius * 2);

   int32_t x = (radius - 1);
   int32_t y = 0;
   int32_t tx = 1;
   int32_t ty = 1;
   int32_t error = (tx - diameter);

   while (x >= y)
   {
      //  Each of the following renders an octant of the circle
      SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);
      SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);
      SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
      SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);
      SDL_RenderDrawPoint(renderer, centreX + y, centreY - x);
      SDL_RenderDrawPoint(renderer, centreX + y, centreY + x);
      SDL_RenderDrawPoint(renderer, centreX - y, centreY - x);
      SDL_RenderDrawPoint(renderer, centreX - y, centreY + x);

      if (error <= 0)
      {
         ++y;
         error += ty;
         ty += 2;
      }

      if (error > 0)
      {
         --x;
         tx += 2;
         error += (tx - diameter);
      }
   }
}

void draw_filled_circle(SDL_Renderer* renderer, int centerX, int centerY, int radius) {
    int x = 0, y = radius;
    int d = 3 - 2 * radius;

    while (y >= x) {
        // Draw vertical lines from (centerX ± x, centerY ± y) to (centerX ± y, centerY ± x)
        SDL_RenderDrawLine(renderer, centerX - x, centerY - y, centerX + x, centerY - y);
        SDL_RenderDrawLine(renderer, centerX - x, centerY + y, centerX + x, centerY + y);

        // Draw vertical lines from (centerX ± y, centerY ± x) to (centerX ± x, centerY ± y)
        SDL_RenderDrawLine(renderer, centerX - y, centerY - x, centerX + y, centerY - x);
        SDL_RenderDrawLine(renderer, centerX - y, centerY + x, centerX + y, centerY + x);

        if (d <= 0) {
            d += 4 * x + 6;
        } else {
            d += 4 * (x - y) + 10;
            y--;
        }
        x++;
    }
}

void renderLine(SDL_Renderer *renderer, int x1, int y1, int x2, int y2, SDL_Color color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
    SDL_RenderDrawLine(renderer, x1+1, y1, x2+1, y2);
    SDL_RenderDrawLine(renderer, x1-1, y1, x2-1, y2);
    SDL_RenderDrawLine(renderer, x1, y1+1, x2, y2+1);
    SDL_RenderDrawLine(renderer, x1, y1-1, x2, y2-1);
}

void renderBezier(SDL_Renderer *renderer, int x1, int y1, int x2, int y2, int x3, int y3, SDL_Color color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    float t = 0.0;
    int x = x1;
    int y = y1;
    int x_old = x1;
    int y_old = y1;
    for (t = 0.0; t < 1.0; t += 0.01)
    {
        x = (1 - t) * (1 - t) * x1 + 2 * (1 - t) * t * x2 + t * t * x3;
        y = (1 - t) * (1 - t) * y1 + 2 * (1 - t) * t * y2 + t * t * y3;
        // SDL_RenderDrawPoint(renderer, x, y);
        renderLine(renderer, x_old, y_old, x, y, color);
        x_old = x;
        y_old = y;
    }
    DrawCircle(renderer, x1, y1, 10,COLOR_RED);
    DrawCircle(renderer, x2, y2, 10,COLOR_RED);
    DrawCircle(renderer, x3, y3, 10,COLOR_RED);
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
    SDL_RenderDrawLine(renderer, x2, y2, x3, y3);
}


void pollDraw(int *pixels, SDL_Event *event, SDL_Renderer *renderer)
{
    static SDL_Color color = COLOR_WHITE;
    switch (event->type)
    {
        case SDL_KEYDOWN:
            if (event->key.keysym.sym == SDLK_r)
            {
                color = COLOR_RED;
            }
            else if (event->key.keysym.sym == SDLK_g)
            {
                color = COLOR_GREEN;
            }
            else if (event->key.keysym.sym == SDLK_b)
            {
                color = COLOR_BLUE;
            }
            else if (event->key.keysym.sym == SDLK_w)
            {
                color = COLOR_WHITE;
            }
            else if (event->key.keysym.sym == SDLK_c)
            {
                clearScreen(renderer,COLOR_PURPLE);
            }
            break;
        /* Mouse Clicks */
        case SDL_MOUSEBUTTONDOWN:
            if (event->button.button == SDL_BUTTON_LEFT)
            {
            }
            if (event->button.button == SDL_BUTTON_RIGHT)
            {
            }
            break;
        case SDL_MOUSEMOTION:
            if(event->motion.state & SDL_BUTTON_LMASK)
            {
                int mouseX = event->motion.x % SCREEN_WIDTH;
                int mouseY = event->motion.y % SCREEN_HEIGHT;
                renderLine(renderer,lastx,lasty,mouseX,mouseY,color);
                lastx = mouseX;
                lasty = mouseY;
                pixels[mouseY*SCREEN_WIDTH+mouseX] = 1;
            }else{
                lastx = event->motion.x;
                lasty = event->motion.y;
            }
            break;
    }
}


void pollEvents(SDL_Window *window, SDL_Rect *rect, int *pixels)
{
    SDL_Event event;
    while(SDL_PollEvent(&event)){
        pollRect(rect,&event);
        pollWindow(&event);
        pollDraw(pixels,&event,SDL_GetRenderer(window));
    }
}

int main(int argc, char** argv)
{
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    SDL_Window *window = SDL_CreateWindow("SDL test",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          SCREEN_WIDTH, SCREEN_HEIGHT,
                                          SDL_WINDOW_SHOWN);

    SDL_Renderer *renderer = SDL_CreateRenderer(window,
                                                -1,
                                                SDL_RENDERER_ACCELERATED);

    SDL_Texture *texture1 = IMG_LoadTexture(renderer, "assets/test.png");

    assert(renderer && window && texture1);

    Entity entity1 = initEntity(texture1, 600.0f, 300.0f);
    SDL_Rect rec1 = newRect(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 120, 120);

    int *pixels= (int *)malloc(SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(int));
    memset(pixels, 0, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(int));
    /// render multiple points at once

    running = true;
    clearScreen(renderer,COLOR_PURPLE);
    // /* Game Loop */
    while(running){
        pollEvents(window,&rec1,pixels);
        // clearScreen(renderer,COLOR_PURPLE);
        renderEntity(&entity1, renderer);
        renderRect(renderer, COLOR_TEAL, &rec1);
        DrawCircle(renderer, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 200,COLOR_OLIVE);
        draw_filled_circle(renderer, 400, 300, 100);
        // renderPixel(renderer,pixels,COLOR_MAGENTA);
        renderBezier(renderer, x1, y1, x2, y2, x3, y3, COLOR_WHITE);
        SDL_RenderPresent(renderer);
    }
    destroy(window, renderer);

    return 0;
}