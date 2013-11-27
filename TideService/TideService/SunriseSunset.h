#ifndef _SUNRISESUNSET_H_
#define _SUNRISESUNSET_H_

#include "TimeTypes.h"
#include <ctime>

namespace sunrisesunset
{
	SunriseSunset GetSunriseSunset(time_t theDay, double longitude, double latitude);
}

#endif
