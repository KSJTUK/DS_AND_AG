#include "pch.h"
#include "RandomEngine.h"

std::unique_ptr<RandomEngine> randomEngine = std::make_unique<RandomEngine>();

int Random::RandInt(int min, int max) {
	return randomEngine->GetRandInt(min, max);
}
