#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

#define SCREEN_WIDTH 1280 
#define SCREEN_HEIGHT 720

typedef struct Entity{
    SDL_Texture *texture;
    SDL_Rect currentFrame;
    float x_pos;
    float y_pos;
}Entity;

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

int main(int argc, char** argv)
{
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    SDL_Window *window = SDL_CreateWindow("SLD test",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SCREEN_WIDTH, SCREEN_HEIGHT,
                                          SDL_WINDOW_SHOWN);

    SDL_Renderer *renderer = SDL_CreateRenderer(window,
                                                -1,
                                                SDL_RENDERER_ACCELERATED);
    SDL_Texture *texture1 = IMG_LoadTexture(renderer, "assets/test.png");
    SDL_Texture *texture2 = IMG_LoadTexture(renderer, "assets/e1.png");

    Entity entity1 = initEntity(texture1, 600.0f, 300.0f);
    Entity entity2 = initEntity(texture2, 150.0f, 300.0f);


    bool running = true;

    /* Game Loop */
    while(running){
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_QUIT:
                    running = false;
                    break;

                default:
                    break;
            }
        }
        // SDL_SetRenderDrawColor(renderer, 0, 100, 0, 255);
        SDL_RenderClear(renderer);
        renderEntity(&entity1, renderer);
        renderEntity(&entity2, renderer);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}