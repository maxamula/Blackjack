#pragma once
#include "SDL.h"
#include "utils.h"
#include "main.h"
#include <string>
#include <mutex>
#include <vector>
#include <windows.h>

#define DISABLE_MOVE_COPY(class_name) class_name(const class_name&) = delete; class_name& operator=(const class_name&) = delete; class_name(class_name&&) = delete; class_name& operator=(class_name&&) = delete;
#define DISABLE_COPY(class_name) class_name(const class_name&) = delete; class_name& operator=(const class_name&) = delete;

//#define ENABLE_DEALER_MUSTHAVE

const uint32_t uint32_invalid = 0xffffffff;
const uint64_t uint64_invalid = 0xffffffffffffffff;

