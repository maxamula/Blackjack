#pragma once

namespace blackjack
{
	extern SDL_Window* g_mainWindow;
	extern SDL_Renderer* g_renderer;

	extern OneTimeEvent<> g_defferedAction;

	void GotoGame();
    void GotoOptions();
	void NewGame();
}