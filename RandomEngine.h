#pragma once

class RandomEngine {
public:
	RandomEngine();
	~RandomEngine();

private:
	std::random_device rd{ };
	std::default_random_engine dre{ rd() };

public:
	int GetRandInt(int min, int max);
	float GetRandFloat();
};

