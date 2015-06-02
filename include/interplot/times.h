#pragma once

#include "base.h"
#include <chrono>

namespace interplot
{

typedef float    ftime;
typedef long int itime;

class Times
{
public:
	Times();

	void initialize();

	void update();

public:
	static constexpr long int TICKS_PER_SECOND = 1000000;

	// total real time
	ftime real;
	// real frame time
	ftime realDelta;
	// real to virtual time scale
	ftime scale;
	// total virtual time
	ftime total;
	// virtual frame time
	ftime delta;
	// real time ticks (microseconds)
	itime ticks;

private:
	std::chrono::high_resolution_clock::time_point m_tStart;
};

}
