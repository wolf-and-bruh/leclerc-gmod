#pragma once
#include "../includes.hpp"

class PredictionEngine {
public:
	void Start();
	void End();
};

inline auto g_Prediction = std::make_unique<PredictionEngine>();