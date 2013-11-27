#include "stdafx.h"
#include <ctime>
#include <fstream>
#include <vector>
#include <boost/asio.hpp>
#include <boost/filesystem.hpp>
#include "TideService.h"
#include "TideParser.h"
#include "SunriseSunset.h"

using namespace std;
using namespace boost::asio::ip;

namespace tideservice {

	ITideService* ITideService::Create(std::string domain, std::string remotePath, std::string localCachePath) {
		TideService *p = new TideService(domain, remotePath, localCachePath);
		return p;
	}

	TideService::TideService(std::string host, std::string hostPath, std::string localCachePath) {
		_host = host;
		_hostPath = hostPath;
		_localCachePath = localCachePath;
	}

	const DailyTide* TideService::GetTideOfTheDay(AreaCode areaCode) {

		// load data
		int err = LoadOrDownloadIfNecessary(areaCode);
		if(err != 0)
			return NULL;

		// in this point, local file must be cached
		return FindTodaysTide(_dailyTides);
	}

	vector<DailyTide*> TideService::GetTideOfTheMonth(AreaCode areaCode)
	{
		vector<DailyTide*> result;

		// load data
		int err = LoadOrDownloadIfNecessary(areaCode);
		if(err != 0)
			return result;

		// in this point, local file must be cached
		result = FindThisMonthTide(_dailyTides);
		return result;
	}

	SunriseSunset TideService::GetSunriseSunset(AreaCode areaCode, time_t theDay)
	{
		AreaDataEntry entry = GetAreaDataEntry(areaCode);
		return sunrisesunset::GetSunriseSunset(theDay,
			entry.longitude.degree + entry.longitude.min/60.0,
			entry.latitude.degree + entry.latitude.min/60.0);
	}

	int TideService::ParseDataFile(AreaCode areaCode)
	{
		string areaCodeString = GetAreaCodeString(areaCode);
		string filename = _localCachePath + "\\" + areaCodeString + ".txt";
		_dailyTides.clear();

		return ParseData(filename, _dailyTides);
	}

	int TideService::DownloadTideData(AreaCode areaCode, int year)
	{
		string host = _host;
		string filename = GetAreaCodeString(areaCode) + ".txt";
		string path = _hostPath + IntToString(year) + "/" + filename;

		try {
			ofstream ofs(filename.c_str(), ios::trunc);
			HttpGet(host, path, ofs);
		} catch (std::exception& e) {
			std::cout << "Exception: " << e.what() << "\n";

			if(boost::filesystem::exists(filename))
				boost::filesystem::remove(filename);

			return -1;
		}

		return 0;
	}

	void TideService::HttpGet(string host, string path, ofstream& ofs)
	{
		boost::asio::io_service io_service;

		tcp::resolver resolver(io_service);
		tcp::resolver::query query(host, "http");
		tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
		tcp::resolver::iterator end;

		tcp::socket socket(io_service);
		boost::system::error_code error = boost::asio::error::host_not_found;

		while (error && endpoint_iterator != end) {
			socket.close();
			socket.connect(*endpoint_iterator++, error);
		}

		if (error) {
			throw boost::system::system_error(error);
		}

		boost::asio::streambuf request;
		std::ostream request_stream(&request);

		request_stream << "GET " << path << " HTTP/1.0\r\n";
		request_stream << "Host: " << host << "\r\n";
		request_stream << "Connection: close\r\n\r\n";

		boost::asio::write(socket, request);

		boost::asio::streambuf response;
		boost::asio::read_until(socket, response, "\r\n");
		std::istream response_stream(&response);

		std::string http_version;
		response_stream >> http_version;

		unsigned int status_code;
		response_stream >> status_code;

		std::string status_message;
		std::getline(response_stream, status_message);

		if (!response_stream || http_version.substr(0, 5) != "HTTP/") {
			throw std::exception("Invalid response");
		}

		if (status_code != 200) {
			throw std::exception("Response returned with status code\n");
		}


		boost::asio::read_until(socket, response, "\r\n\r\n");

		std::string header;

		while (std::getline(response_stream, header) && header != "\r") {
			cout << header << "\n";
		}

		cout << "\n";

		if (response.size() > 0) {
			ofs << &response;
		}

		while (boost::asio::read(socket, response, boost::asio::transfer_at_least(1), error)) {
			ofs << &response;
		}

		if (error != boost::asio::error::eof) {
			throw boost::system::system_error(error);
		}
	}

	DailyTide* TideService::FindTodaysTide(vector<DailyTide>& dailyTides)
	{
		struct tm tm_now = GetTmNow();

		vector<DailyTide>::iterator it = dailyTides.begin();
		while(it != dailyTides.end()) {
			struct tm tm_work;
			localtime_s(&tm_work, &it->date);
			if( tm_work.tm_year == tm_now.tm_year && tm_work.tm_mon == tm_now.tm_mon && tm_work.tm_mday == tm_now.tm_mday) {
				return &(*it);
			}
			it++;
		}

		return NULL;
	}

	vector<DailyTide*> TideService::FindThisMonthTide(std::vector<DailyTide>& dailyTides)
	{
		struct tm tm_now = GetTmNow();

		vector<DailyTide*> result;
		vector<DailyTide>::iterator it = dailyTides.begin();

		while(it != dailyTides.end()) {
			struct tm tm_work;
			localtime_s(&tm_work, &it->date);
			if( tm_work.tm_year == tm_now.tm_year && tm_work.tm_mon == tm_now.tm_mon) {
				result.push_back(&(*it));
			}
			it++;
		}

		return result;
	}

	string TideService::IntToString(int num)
	{
		stringstream ss;
		ss << num;
		return ss.str();
	}

	struct tm TideService::GetTmNow()
	{
		time_t now = time(NULL);
		struct tm tm_now;
		localtime_s(&tm_now, &now);

		return tm_now;
	}

	int TideService::LoadOrDownloadIfNecessary(AreaCode areaCode)
	{
		int err = 0;
		// not loaded yet
		if(_dailyTides.empty()) {
			// try load from local cache
			err = ParseDataFile(areaCode);
			if(err != 0) {
				// download from internet
				err = DownloadTideData(areaCode, GetTmNow().tm_year + 1900);
				if(err != 0)
					return -1;
				// parse data
				err = ParseDataFile(areaCode);
				if(err != 0)
					return -1;
			}
		}

		return 0;
	}
}