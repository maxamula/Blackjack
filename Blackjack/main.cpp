#include "UI/idrawable.h"
#include "Core/scene.h"
#include "Game/deck.h"
#include "Game/mainmenu.h"

#undef min

namespace blackjack
{
    SDL_Window* g_mainWindow{ nullptr };
    SDL_Renderer* g_renderer{ nullptr };

    Scene* g_pActiveScene = nullptr;
}

using namespace blackjack;

void _UpdateSize()
{
    int windowWidth, windowHeight;
    SDL_GetWindowSize(g_mainWindow, &windowWidth, &windowHeight);

    float scale = std::min((float)windowWidth / 1280.0f, (float)windowHeight / 720.0f);
    int scaledWidth = (int)(1280.0f * scale);
    int scaledHeight = (int)(720.0f * scale);

    int viewportX = (windowWidth - scaledWidth) / 2;
    int viewportY = (windowHeight - scaledHeight) / 2;

    SDL_Rect viewport = { viewportX, viewportY, scaledWidth, scaledHeight };
    SDL_RenderSetViewport(g_renderer, &viewport);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    PSTR lpCmdLine, int nCmdShow)
{
    SDL_Init(SDL_INIT_VIDEO);    

    g_mainWindow = SDL_CreateWindow("Blackjack alpha", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    g_renderer = SDL_CreateRenderer(g_mainWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_RenderSetLogicalSize(g_renderer, 1280, 720);
    _UpdateSize();

    SDL_Texture* bgTexture = IMG_LoadTexture(g_renderer, "C:\\bg.jpg");
    PlayCard::LoadSkinSet("C:\\normal");
    Scene scene;
    //CreateMainMenu(scene);
    PlayCard card = PlayCard::CreateCard(scene.CreateObject(), CARD_SUIT_SPADES, 14);
    scene.DestroyObject(card.GetObject());

    {
        std::shared_ptr<Texture> t = g_resourceManager.GetResource<Texture>("C:\\bg.jpg");
    }

    g_pActiveScene = &scene;

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
            case SDL_EventType::SDL_WINDOWEVENT:
                switch (event.window.event)
                {
                case SDL_WINDOWEVENT_RESIZED:
                    // Получение новых размеров окна
                    int windowWidth = event.window.data1;
                    int windowHeight = event.window.data2;

                    // Вычисление масштаба для заполнения всей области окна
                    float scaleX = (float)windowWidth / 1280;
                    float scaleY = (float)windowHeight / 720;

                    // Установка новых размеров вьюпорта и масштаба
                    SDL_Rect viewport = { 0, 0, 1280, 720 };
                    SDL_RenderSetViewport(g_renderer, &viewport);
                    SDL_RenderSetScale(g_renderer, scaleX, scaleY);
                    break;
                }
                break;
            }
            g_pActiveScene->HandleEvent(event);
        }
        SDL_SetRenderDrawColor(g_renderer, 255, 255, 255, 255);
        SDL_RenderClear(g_renderer);
        // Отрисовка графики здесь
        SDL_RenderCopy(g_renderer, bgTexture, nullptr, nullptr);
        
        if (g_pActiveScene)
            g_pActiveScene->Render();
        SDL_RenderPresent(g_renderer);
    }

    SDL_DestroyRenderer(g_renderer);
    SDL_DestroyWindow(g_mainWindow);
    SDL_Quit();
    return 0;
}