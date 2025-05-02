#pragma once
#include <string>

#include <random>
#include <ctime>
#include <windows.h>
#include <MMsystem.h>
#include <iostream>
#include <string>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <atomic>
#include "SDL.h"

namespace generic_tools 
{
	int random_number(int limit);
	int random_number(int limit_start, int limit_end);
	void clear_console();
	void sleep(int time);
	void wait_for_enter();
	void CreateRect(std::vector<SDL_Rect>& Rect, int frames);
}
