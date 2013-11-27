// TideServiceSample.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include <string>
#include <iostream>
#include <memory>
#include <vector>
#include "ITideService.h"

using namespace std;
using namespace tideservice;

int _tmain(int argc, _TCHAR* argv[])
{
	auto_ptr<ITideService> ptr(ITideService::Create(DEFAULT_HOST, DEFAULT_HOSTPATH, "."));

	const DailyTide* dt = ptr->GetTideOfTheDay(J5);
	if(dt == NULL) {
		cout << "failed" << endl;
		return -1;
	}

	vector<DailyTide*> vec = ptr->GetTideOfTheMonth(J5);
	vector<DailyTide*>::iterator it = vec.begin();
	while(it != vec.end()) {
		for(int i = 0; i < 23; i++) {
			cout << (*it)->tides[i] << endl;
		}
		it++;
	}
	return 0;
}

