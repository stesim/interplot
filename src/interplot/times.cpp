#include "times.h"

namespace interplot
{

Times::Times()
	: real( 0.0f ),
	realDelta( 0.0f ),
	scale( 1.0f ),
	total( 0.0f ),
	delta( 0.0f ),
	ticks( 0 )
{
}

void Times::initialize()
{
	using namespace std;
	m_tStart = chrono::high_resolution_clock::now();
}

void Times::update()
{
	using namespace std;
	itime newTicks = chrono::duration_cast<chrono::microseconds>(
			chrono::high_resolution_clock::now() - m_tStart ).count();

	itime deltaTicks = newTicks - ticks;
	ticks = newTicks;

	real       = (ftime)ticks / TICKS_PER_SECOND;
	realDelta  = (ftime)deltaTicks / TICKS_PER_SECOND;
	delta      = scale * realDelta;
	total     += delta;
}

}
