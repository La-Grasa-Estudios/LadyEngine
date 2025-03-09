#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <memory>
#include <functional>
#include <iostream>
#include "TexturedRectangle.hpp"
#include "AnimatedSprite.hpp"

void SetPixel(SDL_Surface* surface, uint32_t x, uint32_t y, uint8_t R, uint8_t G, uint8_t B)
{
    SDL_LockSurface(surface);
    std::cout << "Left mouse button has been pressed in x:" << x << " y: " << y << "\n";
    uint8_t* pixelArray = static_cast<uint8_t*>(surface->pixels);
    pixelArray[y * surface->pitch + x * surface->format->BytesPerPixel +0] = B;
    pixelArray[y * surface->pitch + x * surface->format->BytesPerPixel +1] = G;
    pixelArray[y * surface->pitch + x * surface->format->BytesPerPixel +2] = R;
    SDL_UnlockSurface(surface);
}

void move_rectangle(SDL_Rect& rectangle, uint32_t unx, uint32_t uny) 
{
    rectangle.x = unx;
    rectangle.y = uny;
}


int main() 
{
    
    bool increment = true;

    uint32_t RED = 0;
    uint32_t GREEN = 0;
    uint32_t BLUE = 0;

    int mouseX, mouseY;
    

    SDL_INIT_GAMECONTROLLER;
    SDL_Window* window = SDL_CreateWindow("Comandante en jefe", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN);

    SDL_Renderer* render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    //SDL_SetColorKey(four_texture, SDL_TRUE, SDL_MapRGB(four_texture->format, 255, 0, 0));
 
    TexturedRectangle raiden(render, "images/raiden.png");
    raiden.set_attributes(200, 200, 250, 250);
    raiden.set_texture();
    //TexturedRectangle raiden2(render, "images/raiden.png");
    //raiden2.set_attributes(90, 90, 100, 100);
    //raiden2.set_texture();


    SDL_UpdateWindowSurface(window);

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "SDL couldn't be initialized";
    }
    else
    {
        std::cout << "SDL initialized successfully";
    }
    
    bool running = true;

    uint8_t R = 0;
    uint8_t G = 0;
    uint8_t B = 0;


    //FONTS*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*//*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*//*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/
    if (TTF_Init() == -1)
    {
        std::cout << "\nSomething is wrong with TTF\n";
    }
    else
    {
        std::cout << "\nTTF ready to go\n";
    }

    TTF_Font* my_font = TTF_OpenFont("fonts/Gameplay.ttf", 16);

    if(my_font == nullptr)
    {
        std::cout << "\nCould not find the font\n";
    }
    else
    {
        std::cout << "\nSuccessfully font opening\n";
    }

    SDL_Surface* text_surface = TTF_RenderText_Solid(my_font, "Balatro Balatrez", { 0,255,0 });
    SDL_Texture* Ttexture = SDL_CreateTextureFromSurface(render, text_surface);

    SDL_FreeSurface(text_surface);

    SDL_Rect Text_Rect;

    Text_Rect.x = 500;
    Text_Rect.y = 600;
    Text_Rect.w = 400;
    Text_Rect.h = 100;
    
    //*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*//*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*//*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*//*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/
    //IMAGES*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*//*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*//*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*//*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/

    if (IMG_Init(IMG_INIT_PNG) == -1) 
    {
        std::cout << "\nImage System could not load\n";
    }
    else
    {
        std::cout << "\nImage Systema ready to go\n";
    }

    AnimatedSprite test(render, "images/test.png");
    test.portion(120, 60, 128, 128);

    
    float monitax = 0.0f;
    float monita_speed = 180.0f;
    Uint32 previous_time = SDL_GetTicks();
    float deltaTime = 0.0f;
    
    float animationFrameTime = 1.0f / 12.0f;
    float frameTimeAccumulator = 0.0f;
    int frame = 0;

    SDL_RendererFlip flip = SDL_FLIP_NONE;
    bool RIGHT = false;
    bool LEFT = false;

    while (running)
    {
        Uint32 current_time = SDL_GetTicks();
        deltaTime = (current_time - previous_time) / 1000.0f;
        previous_time = current_time;
        frameTimeAccumulator += deltaTime;

        if(frameTimeAccumulator >= animationFrameTime)
        {
            frame = (frame + 1) % 12;
            frameTimeAccumulator -= animationFrameTime;
        }
        SDL_GetMouseState(&mouseX,&mouseY);
        RED += 1;
        BLUE += 2;
        GREEN += 3;
        if (RED > 255) RED = 0;
        if (BLUE > 255) BLUE = 0;
        if (GREEN > 255) GREEN = 0;

        const Uint8* state = SDL_GetKeyboardState(NULL);
        SDL_Event event;
        int x, y;
        SDL_GetMouseState(&x, &y);


        while (SDL_PollEvent(&event))
        {
            if (event.button.button == SDL_BUTTON_LEFT)
            {

                if (R > 255) R = 0;
                if (G > 255) G = 0;
                if (B > 255) B = 0;
                //SetPixel(screen, x, y, R, G, B);
                R += 3;
                G += 2;
                B += 1;

            }
            if (event.button.button == SDL_BUTTON_RIGHT)
            {

            }

            if (event.type == SDL_QUIT)
            {
                running = false;
            }
            if (event.type == SDL_MOUSEMOTION)
            {
               // std::cout << "Mouse moved";
                test.portion(mouseX, mouseY, 128, 128);
            }
            if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_a)
                {
                    std::cout << "\nA has been pressed\n";
                }
            }
            
            if (state[SDL_SCANCODE_F])
            {
                std::cout << "F has been pressed\n";
            }


            if (state[SDL_SCANCODE_ESCAPE])
            {
                running = false;
            }
        }
        if (state[SDL_SCANCODE_RIGHT])
        {
            monitax += (monita_speed * deltaTime);
            test.portion(monitax, 50, 128, 128);
            if (!RIGHT)
            {
                flip = SDL_FLIP_NONE;
                RIGHT = true;
                LEFT = false;
            }
        }
        if (state[SDL_SCANCODE_LEFT])
        {
            monitax -= (monita_speed * deltaTime);
            test.portion(monitax, 50, 128, 128);
            if(!LEFT)
            {
                flip = SDL_FLIP_HORIZONTAL;
                RIGHT = false;
                LEFT = true;
            }
        }

        //SDL_UpdateWindowSurface(window);
        
        SDL_SetRenderDrawColor(render, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(render);
        
        //Render Water Effect texture
        //SDL_Delay(60);
        raiden.set_render_copy();
        //raiden2.set_render_copy();
        test.play_frame(0, 0, 128, 128, frame);
        test.render(render, flip);
        SDL_RenderPresent(render);

        if(test.is_coliding_with(raiden))
        {
            std::cout << "\nMonita china colisionando con Raiden\n";
        }


    }
    SDL_DestroyWindow(window);
    TTF_CloseFont(my_font);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}