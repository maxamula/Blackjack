#pragma once
#include "SDL.h"
#include "utils.h"
#include <string>
#include <mutex>
#include <vector>
#include <windows.h>

#define DISABLE_MOVE_COPY(class_name) class_name(const class_name&) = delete; class_name& operator=(const class_name&) = delete; class_name(class_name&&) = delete; class_name& operator=(class_name&&) = delete;
#define DISABLE_COPY(class_name) class_name(const class_name&) = delete; class_name& operator=(const class_name&) = delete;

const uint32_t INVALID_ID = 0xffffffff;

namespace blackjack
{
	extern SDL_Window* g_mainWindow;
	extern SDL_Renderer* g_renderer;
}
