#include "math.h"
#include "math.h"

float math::calcDist(float x1, float y1, float x2, float y2)
{
    float dx = x1 - x2;
    float dy = y1 - y2;
    float mult = dx * dx + dy * dy;
    return sqrtf(mult);
}

int math::randd()
{
	return ((rand() / (float)(0x7fff + 1)) < 0.5 ? 1 : 0);
}
