#pragma once
#include "cbaseplayer.hpp"

class IMoveHelper {
private:
	virtual void UnknownVirtual() = 0;
public:
	virtual void SetHost(C_BasePlayer* host) = 0;
};