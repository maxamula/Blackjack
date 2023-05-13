#include "UI/idrawable.h"
#include "Core/scene.h"
#include "Game/deck.h"

namespace blackjack
{
    SDL_Window* g_mainWindow{ nullptr };
    SDL_Renderer* g_renderer{ nullptr };
    Event g_LMBDownEvent;
}

using namespace blackjack;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    PSTR lpCmdLine, int nCmdShow)
{
    SDL_Init(SDL_INIT_VIDEO);    

    g_mainWindow = SDL_CreateWindow("Blackjack", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    g_renderer = SDL_CreateRenderer(g_mainWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_Texture* bgTexture = IMG_LoadTexture(g_renderer, "C:\\bg.jpg");
    PlayCard::LoadSkinSet("C:\\normal");
    Scene scene;
    Deck deck(scene.CreateObject());
    PlayCard card = PlayCard::CreateCard(scene.CreateObject(), CARD_SUIT_SPADES, 12);

    GameObject button2 = scene.CreateObject();
    button2.SetSprite(CMP_SPRITE::CreateSprite("C:\\CardNormal\\CardNormal_0000_Bitmap------------------.png"));
    button2.GetTransformation().scaley = 1.0;
    button2.GetTransformation().scalex = 1.0;
    button2.GetTransformation().x = 0.5 + 0.04;
    button2.GetTransformation().y = 0.8 + 0.02;

    bool isRunning = true;
    SDL_Event event;
    while (isRunning)
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                isRunning = false;
                break;
            case SDL_MOUSEBUTTONUP :
                if (event.button.button == SDL_BUTTON_LEFT)
                    g_LMBDownEvent();
                break;
            }
            
        }

        SDL_SetRenderDrawColor(g_renderer, 255, 255, 255, 255);
        SDL_RenderClear(g_renderer);
        // Отрисовка графики здесь
        SDL_RenderCopy(g_renderer, bgTexture, nullptr, nullptr);
        scene.Render();
        SDL_RenderPresent(g_renderer);
    }

    SDL_DestroyRenderer(g_renderer);
    SDL_DestroyWindow(g_mainWindow);
    SDL_Quit();
    return 0;
}