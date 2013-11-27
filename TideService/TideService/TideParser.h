#ifndef _TIDE_PARSER_H_
#define _TIDE_PARSER_H_

#include "TideTypes.h"
#include <vector>
#include <string>

namespace tideservice {

	typedef struct _Latitude {
		int degree;
		int min;
	} Latitude;

	typedef struct _Longitude {
		int degree;
		int min;
	} Longitude;

	typedef struct _AreaDataEntry {
		char codestring[4];
		char areaname[28];
		Latitude latitude;
		Longitude longitude;
	} AreaDataEntry;

	int ParseData(const std::string& filename, std::vector<DailyTide> &vec);
	std::string GetAreaCodeString(AreaCode areaCode);
	AreaDataEntry GetAreaDataEntry(AreaCode areaCode);
}

#endif
