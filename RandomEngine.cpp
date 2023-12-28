#include "pch.h"
#include "RandomEngine.h"

RandomEngine::RandomEngine() { }

RandomEngine::~RandomEngine() { }

int RandomEngine::GetRandInt(int min, int max) {
    std::uniform_int_distribution uid{ min, max };
    return uid(dre);
}

float RandomEngine::GetRandFloat() {
    return 0.0f;
}
