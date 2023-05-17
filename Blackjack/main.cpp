#include "UI/idrawable.h"
#include "Core/scene.h"
#include "Game/deck.h"
#include "Game/mainmenu.h"
#include "Game/player.h"
#include "Game/game.h"
#include "Core/animations.h"
#include "imgui/imgui.h"
#include "imgui/imgui_sdl.h"

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

    g_mainWindow = SDL_CreateWindow("Blackjack alpha", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN /*SDL_WINDOW_RESIZABLE*/);
    g_renderer = SDL_CreateRenderer(g_mainWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_RenderSetLogicalSize(g_renderer, 1280, 720);
    SDL_Rect viewport = { 0, 0, 1280, 720 };
    SDL_RenderSetViewport(g_renderer, &viewport);
    _UpdateSize();

    ImGui::CreateContext();
    ImGuiSDL::Initialize(g_renderer, 1280, 720);
    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(1.2f);


    SDL_Texture* bgTexture = IMG_LoadTexture(g_renderer, "C:\\bg.jpg");
    PlayCard::LoadSkinSet("C:\\normal");
    Scene scene;
    g_pActiveScene = &scene;
    Player p;
    Game game(scene, p);

    bool isRunning = true;
    std::thread renderThread([&]() {
        while (isRunning)
        {
            SDL_SetRenderDrawColor(g_renderer, 255, 255, 255, 255);
            SDL_RenderClear(g_renderer);
            // Отрисовка графики здесь
            SDL_RenderCopy(g_renderer, bgTexture, nullptr, nullptr);
            ImGui::NewFrame();
#ifdef _DEBUG
            ImGui::SetNextWindowPos(ImVec2(0, 0));
            ImGui::SetNextWindowSize(ImVec2(300, 500));
            ImGui::Begin("Debug view", nullptr, ImGuiWindowFlags_NoResize);
            g_pActiveScene->DrawDebugOverlay();
            game.DrawDebugOverlay();
            ImGui::End();
#endif
            g_pActiveScene->Overlay();
            ImGui::Render();
            ImGuiSDL::Render(ImGui::GetDrawData());
            if (g_pActiveScene)
                g_pActiveScene->Render();
            SDL_RenderPresent(g_renderer);
        }
    });
    SDL_Event event;
    float dt = 0;
    while (isRunning)
    {
        auto start = std::chrono::high_resolution_clock::now();

        ImGuiIO& io = ImGui::GetIO();
        int wheel = 0;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                isRunning = false;
                renderThread.join();
                break;
            case SDL_KEYDOWN:
#ifdef _DEBUG
                if (event.key.keysym.sym == SDLK_F1)
                    game.Hit();
#endif
                break;
            case SDL_EventType::SDL_WINDOWEVENT:
                switch (event.window.event)
                {
                case SDL_WINDOWEVENT_RESIZED:
                    io.DisplaySize.x = static_cast<float>(event.window.data1);
                    io.DisplaySize.y = static_cast<float>(event.window.data2);
                    int windowWidth = event.window.data1;
                    int windowHeight = event.window.data2;

                    float scaleX = (float)windowWidth / 1280;
                    float scaleY = (float)windowHeight / 720;
                    SDL_RenderSetScale(g_renderer, scaleX, scaleY);
                    break;
                }
                break;
            case SDL_MOUSEWHEEL:
                wheel = event.wheel.y;
                break;
            }
            g_pActiveScene->HandleEvent(event);
        }

        // Update animations
        Animation::UpdateAnimations(dt);

        // Imgui Input
        int mouseX, mouseY;
        const int buttons = SDL_GetMouseState(&mouseX, &mouseY);
        io.DeltaTime = 1.0f / 60.0f;
        io.MousePos = ImVec2(static_cast<float>(mouseX), static_cast<float>(mouseY));
        io.MouseDown[0] = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
        io.MouseDown[1] = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);
        io.MouseWheel = static_cast<float>(wheel);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float, std::milli> duration = end - start;
        dt = duration.count();
    }

    PlayCard::FreeSkinSet();
    ImGuiSDL::Deinitialize();
    SDL_DestroyRenderer(g_renderer);
    SDL_DestroyWindow(g_mainWindow);
    SDL_Quit();
    return 0;
}