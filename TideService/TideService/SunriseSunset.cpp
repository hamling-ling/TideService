// SunriseSunset.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include <ctime>
#define _USE_MATH_DEFINES
#include <cmath>
#include "SunriseSunset.h"

using namespace std;

namespace sunrisesunset
{
	/// <summary>
	/// グレゴリウス暦1年1月1日からの経過日数を求める。
	/// （グレゴリウス暦施行前の日付も、
	///   形式的にグレゴリウス暦と同じルールで計算。）
	/// </summary>
	/// <param name="y">年</param>
	/// <param name="m">月</param>
	/// <param name="d">日</param>
	/// <returns>1年1月1日からの経過日数</returns>
	static int GetDays(int y, int m, int d)
	{
		// 1・2月 → 前年の13・14月
		if (m <= 2)
		{
			--y;
			m += 12;
		}
		int dy = 365 * (y - 1); // 経過年数×365日
		int c = y / 100;
		int dl = (y >> 2) - c + (c >> 2); // うるう年分
		int dm = (m * 979 - 1033) >> 5; // 1月1日から m 月1日までの日数
		return dy + dl + dm + d - 1;
	}

	// longitude: longigude in degree
	// latitude : latitude in degree
	SunriseSunset GetSunriseSunset(time_t theDay, double longitude, double latitude)
	{
		//IdoR=観測地緯度*PI()/180
		double latitudeR = (latitude)*M_PI/180.0;
		//D=1月1日からの通し日付
		tm tm_theday;
		localtime_s(&tm_theday, &theDay);

		int gregoToJuly1st=GetDays(tm_theday.tm_year + 1900, 1, 1);
		int gregoToToday = GetDays(tm_theday.tm_year + 1900, tm_theday.tm_mon+1, tm_theday.tm_mday);
		int d = gregoToToday - gregoToJuly1st;

		//D1=2*PI()*(D-81.5)/365
		double d1 = 2.0*M_PI*(d-81.5)/365.0;
		//D2=2*PI()*(D-3)/365
		double d2 = 2.0*M_PI*(d-3.0)/365.0;
		//K1=-7.37*SIN(D2)→地球軌道が楕円による均時差
		double k1=-7.37*sin(d2);
		//K2=9.86*SIN(2*D1)→地軸の傾きによる均時差
		double k2=9.86*sin(2.0*d1);
		//Kinji=K1+K2→均時差
		double approx=k1+k2;
		//Delta=0.4082*SIN(D1)→太陽赤緯
		double delta=0.4082*sin(d1);
		//Day(min)=1440*(1-ACOS(TAN(Delta)*TAN(IdoR))/PI())→昼間の時間
		double dayInMin = 1440.0*(1.0-acos(tan(delta)*tan(latitude))/M_PI);
		//Hosei(min)=0.8502*4/SQRT(1-SIN(IdoR)*SIN(IdoR)/(COS(Delta)*COS(Delta)))→大気補正
		double correction = 0.8502*4.0/sqrt(1-sin(latitude)*sin(latitude)/(cos(delta)*cos(delta)));
		//Center=720-Kinji+4*(135-Keido)→南中時刻
		double center = 720.0 - approx + 4.0*(135.0-longitude);
		//日の出(min)=Center-Day/2-Hosei
		double sunriseInMin = center-dayInMin/2.0-correction;
		//日の入(min)=Center+Day/2+Hosei
		double sunsetInMin = center + dayInMin/2.0+correction;

		tm tm_sunrise = tm_theday;
		tm_sunrise.tm_hour = (int)(sunriseInMin / 60.0);
		tm_sunrise.tm_min = (int)((sunriseInMin * 60.0)) % 60;
		tm_sunrise.tm_sec = 0;

		tm tm_sunset = tm_theday;
		tm_sunset.tm_hour = (int)(sunsetInMin / 60.0);
		tm_sunset.tm_min = (int)((sunsetInMin * 60.0)) % 60;
		tm_sunset.tm_sec = 0;

		SunriseSunset ss;
		ss.sunrise = tm_sunrise;
		ss.sunset = tm_sunset;

		return ss;
	}

}
