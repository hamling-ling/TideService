#include <string>
#include <vector>
#include <ctime>
#include <fstream>
#include "ITideService.h"

namespace tideservice {

	class TideService : public ITideService
	{

	public:
		TideService(std::string host, std::string hostPath, std::string localCachePath);
		virtual ~TideService(){};
		virtual const DailyTide* GetTideOfTheDay(AreaCode areaCode);
		virtual std::vector<DailyTide*> GetTideOfTheMonth(AreaCode areaCode);
		virtual SunriseSunset GetSunriseSunset(AreaCode areaCode, time_t theDay);

	private:
		std::string _host;
		std::string _hostPath;
		std::string _localCachePath;
		std::vector<DailyTide> _dailyTides;

		DailyTide* FindTodaysTide(std::vector<DailyTide>& dailyTides);
		std::vector<DailyTide*> FindThisMonthTide(std::vector<DailyTide>& dailyTides);
		int ParseDataFile(AreaCode areaCode);
		int DownloadTideData(AreaCode areaCode, int year);
		void HttpGet(std::string host, std::string path, std::ofstream& ofs);
		std::string IntToString(int num);
		struct tm GetTmNow();
		int LoadOrDownloadIfNecessary(AreaCode areaCode);
	};
}
