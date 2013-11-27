// SunriseSunset.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
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
	/// �O���S���E�X��1�N1��1������̌o�ߓ��������߂�B
	/// �i�O���S���E�X��{�s�O�̓��t���A
	///   �`���I�ɃO���S���E�X��Ɠ������[���Ōv�Z�B�j
	/// </summary>
	/// <param name="y">�N</param>
	/// <param name="m">��</param>
	/// <param name="d">��</param>
	/// <returns>1�N1��1������̌o�ߓ���</returns>
	static int GetDays(int y, int m, int d)
	{
		// 1�E2�� �� �O�N��13�E14��
		if (m <= 2)
		{
			--y;
			m += 12;
		}
		int dy = 365 * (y - 1); // �o�ߔN���~365��
		int c = y / 100;
		int dl = (y >> 2) - c + (c >> 2); // ���邤�N��
		int dm = (m * 979 - 1033) >> 5; // 1��1������ m ��1���܂ł̓���
		return dy + dl + dm + d - 1;
	}

	// longitude: longigude in degree
	// latitude : latitude in degree
	SunriseSunset GetSunriseSunset(time_t theDay, double longitude, double latitude)
	{
		//IdoR=�ϑ��n�ܓx*PI()/180
		double latitudeR = (latitude)*M_PI/180.0;
		//D=1��1������̒ʂ����t
		tm tm_theday;
		localtime_s(&tm_theday, &theDay);

		int gregoToJuly1st=GetDays(tm_theday.tm_year + 1900, 1, 1);
		int gregoToToday = GetDays(tm_theday.tm_year + 1900, tm_theday.tm_mon+1, tm_theday.tm_mday);
		int d = gregoToToday - gregoToJuly1st;

		//D1=2*PI()*(D-81.5)/365
		double d1 = 2.0*M_PI*(d-81.5)/365.0;
		//D2=2*PI()*(D-3)/365
		double d2 = 2.0*M_PI*(d-3.0)/365.0;
		//K1=-7.37*SIN(D2)���n���O�����ȉ~�ɂ��ώ���
		double k1=-7.37*sin(d2);
		//K2=9.86*SIN(2*D1)���n���̌X���ɂ��ώ���
		double k2=9.86*sin(2.0*d1);
		//Kinji=K1+K2���ώ���
		double approx=k1+k2;
		//Delta=0.4082*SIN(D1)�����z�Ԉ�
		double delta=0.4082*sin(d1);
		//Day(min)=1440*(1-ACOS(TAN(Delta)*TAN(IdoR))/PI())�����Ԃ̎���
		double dayInMin = 1440.0*(1.0-acos(tan(delta)*tan(latitude))/M_PI);
		//Hosei(min)=0.8502*4/SQRT(1-SIN(IdoR)*SIN(IdoR)/(COS(Delta)*COS(Delta)))����C�␳
		double correction = 0.8502*4.0/sqrt(1-sin(latitude)*sin(latitude)/(cos(delta)*cos(delta)));
		//Center=720-Kinji+4*(135-Keido)���쒆����
		double center = 720.0 - approx + 4.0*(135.0-longitude);
		//���̏o(min)=Center-Day/2-Hosei
		double sunriseInMin = center-dayInMin/2.0-correction;
		//���̓�(min)=Center+Day/2+Hosei
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
