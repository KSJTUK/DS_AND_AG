#pragma once

#include <Windows.h>
#include <iostream>
#include <string>
#include <random>
#include <memory>

#include "ConsoleHandling.h"

extern std::unique_ptr<class RandomEngine> randomEngine;

namespace Random {
	int RandInt(int min, int max);
	float RandFloat(float min, float max);
}