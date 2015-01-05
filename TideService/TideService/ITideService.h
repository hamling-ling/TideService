#ifndef _ITIDESERVICE_H_
#define _ITIDESERVICE_H_

#include "TideTypes.h"
#include <string>

#define DEFAULT_HOST		"www.data.jma.go.jp"
#define DEFAULT_HOSTPATH	"/gmd/kaiyou/data/db/tide/suisan/txt/"

namespace tideservice {

	/**
	 * @brief TideService Interface
	 */
	class ITideService
	{

	public:

		/**
		 * @brief create ITideService
		 * @param host[in] hostname to obtain tide data over the internet
		 * @param hostPath[in] tide data path in the host server
		 * @param localCachePath[in] path to cache the obtained data
		 */
		static ITideService* Create(std::string host, std::string hostPath, std::string localCachePath);

		/**
		 * @brief Obtain today's tide data
		 * @param areaCode[in] area
		 */
		virtual const DailyTide* GetTideOfTheDay(AreaCode areaCode) = 0;

		/**
		 * @brief Obtain this month's tide data
		 * @param areaCode[in] area
		 */
		virtual std::vector<DailyTide*> GetTideOfTheMonth(AreaCode areaCode) = 0;

		/**
		 * @brief Obtain time of sunrise and sunset
		 * @param areaCode[in] area
		 * @param theDay[in] specify a date
		 */
		virtual SunriseSunset GetSunriseSunset(AreaCode areaCode, time_t theDay) = 0;
	};
}

#endif
