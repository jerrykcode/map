#pragma once
#include <random>
using namespace std;

//Random
template<typename T>
class Random {
private:
	random_device device;
	mt19937 generator;
	uniform_int_distribution<T> distribution;

public:
	Random(T min, T max) {
		generator = *(new mt19937(device()));
		distribution = *(new uniform_int_distribution<T>(min, max));
	}

	T Rand() {
		return distribution(generator);
	}
};
