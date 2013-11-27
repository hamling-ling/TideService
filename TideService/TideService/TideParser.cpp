// TideParser.cpp : ƒRƒ“ƒ\[ƒ‹ ƒAƒvƒŠƒP[ƒVƒ‡ƒ“‚ÌƒGƒ“ƒgƒŠ ƒ|ƒCƒ“ƒg‚ğ’è‹`‚µ‚Ü‚·B
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <ctime>
#include "TideParser.h"

using namespace std;

namespace tideservice {

	typedef struct _TideDataLineTide {
		char data[3];
	} TideDataLineTide;

	typedef struct _TideDataLineDate {
		char yeardata[2];
		char monthdata[2];
		char daydata[2];
	} TideDataLineDate;

	typedef struct _TideDataLineLocation {
		char data[2];
	} TideDataLineLocation;

	typedef struct _TideDataLineTime {
		char hourdata[2];
		char mindata[2];
	} TideDataLineTime;

	typedef struct _TideDataLineTimeTide {
		TideDataLineTime time;
		TideDataLineTide tide;
	} TideDataLineTimeTide;

	typedef struct _TideDataLine {
		TideDataLineTide tides[24];
		TideDataLineDate date;
		TideDataLineLocation areacodestring;
		TideDataLineTimeTide heighests[4];
		TideDataLineTimeTide lowests[4];
	} TideDataLine;

	AreaDataEntry AreaCodeNameTable[] = {
		{"WN", "’t“à",		{45,	24},	{141,	41}},
		{"KE", "}K",		{44,	56},	{142,	35}},
		{"A0", "–ä•Ê",		{44,	21},	{143,	22}},
		{"AS", "–Ô‘–",		{44,	01},	{144,	17}},
		{"A6", "—…‰P",		{44,	01},	{145,	12}},
		{"NM", "ªº",		{43,	21},	{145,	35}},
		{"HN", "‰Ôç",		{43,	17},	{145,	34}},
		{"KP", "–¶‘½•z",	{43,	05},	{145,	07}},
		{"KR", "‹ú˜H",		{42,	59},	{144,	22}},
		{"B1", "\Ÿ",		{42,	18},	{143,	19}},
		{"A9", "‰Y‰Í",		{42,	10},	{142,	46}},
		{"C8", "“Ï¬–q“Œ",	{42,	36},	{141,	49}},
		{"TM", "“Ï¬–q¼",	{42,	38},	{141,	37}},
		{"SO", "”’˜V",		{42,	31},	{141,	19}},
		{"A8", "º—–",		{42,	21},	{140,	57}},
		{"A3", "X",		{42,	07},	{140,	36}},
		{"HK", "”ŸŠÙ",		{41,	47},	{140,	43}},
		{"Q0", "‹g‰ª",		{41,	27},	{140,	14}},
		{"A5", "¼‘O",		{41,	25},	{140,	 6}},
		{"ES", "]·",		{41,	52},	{140,	 8}},
		{"ZP", "‰œK",		{42,	05},	{139,	29}},
		{"OR", "‰œK`",	{42,	10},	{139,	31}},
		{"SE", "£’I",		{42,	27},	{139,	51}},
		{"B6", "õ“s",		{42,	48},	{140,	14}},
		{"B5", "Šâ“à",		{42,	59},	{140,	31}},
		{"Z8", "”E˜H",		{43,	13},	{140,	52}},
		{"B3", "¬’M",		{43,	12},	{141,	00}},
		{"IK", "ÎëV`",	{43,	13},	{141,	18}},
		{"B2", "—¯–G",		{43,	57},	{141,	38}},
		{"F3", "ŒBŒ`",		{45,	11},	{141,	8}},
		{"Q1", "—³”ò",		{41,	15},	{140,	23}},
		{"AO", "ÂX",		{40,	50},	{140,	46}},
		{"ZA", "ó’",		{40,	54},	{140,	52}},
		{"Q2", "‘å–©",		{41,	15},	{141,	9}},
		{"B4", "‘åŠÔ",		{41,	32},	{140,	54}},
		{"SH", "‰º–k",		{41,	22},	{141,	14}},
		{"XS", "‚Ş‚Â¬ìŒ´",{40,	56},	{141,	23}},
		{"HC", "”ªŒË",		{40,	32},	{141,	32}},
		{"XT", "‹vœ",		{40,	12},	{141,	48}},
		{"MY", "‹{ŒÃ",		{39,	39},	{141,	59}},
		{"Q6", "Š˜Î",		{39,	16},	{141,	53}},
		{"OF", "‘å‘D“n",	{39,	01},	{141,	45}},
		{"AY", "ˆ¼ì",		{38,	18},	{141,	30}},
		{"E6", "ÎŠª",		{38,	24},	{141,	16}},
		{"SG", "‰–Š˜",		{38,	19},	{141,	02}},
		{"SD", "å‘äV`",	{38,	16},	{141,	01}},
		{"ZM", "‘Š”n",		{37,	50},	{140,	58}},
		{"ON", "¬–¼•l",	{36,	56},	{140,	54}},
		{"D1", "“ú—§",		{36,	30},	{140,	38}},
		{"D3", "‘åô",		{36,	18},	{140,	34}},
		{"D2", "­“‡",		{35,	56},	{140,	42}},
		{"CS", "’¶q‹™`",	{35,	45},	{140,	52}},
		{"ZF", "Ÿ‰Y",		{35,	8},		{140,	15}},
		{"MR", "•z—Ç",		{34,	55},	{139,	50}},
		{"TT", "ŠÙR",		{34,	59},	{139,	51}},
		{"KZ", "–ØX’Ã",	{35,	22},	{139,	55}},
		{"QL", "ç—t",		{35,	34},	{140,	03}},
		{"CB", "ç—t`",	{35,	36},	{140,	06}},
		{"TK", "“Œ‹",		{35,	39},	{139,	46}},
		{"KW", "ìè",		{35,	31},	{139,	45}},
		{"YK", "‹•l`",	{35,	28},	{139,	38}},
		{"QS", "‰¡•l",		{35,	27},	{139,	39}},
		{"HM", "–{–q",		{35,	26},	{139,	40}},
		{"QN", "‰¡{‰ê",	{35,	17},	{139,	39}},
		{"Z1", "–ûšâ",		{35,	10},	{139,	37}},
		{"OK", "‰ª“c",		{34,	47},	{139,	23}},
		{"QO", "_’Ã“‡",			{34,	13},	{139,	8}},
		{"MJ", "O‘î“‡i’Ø“cj",	{34,	3},	{139,	33}},
		{"QP", "O‘î“‡iˆ¢ŒÃj",	{34,	4},	{139,	29}},
		{"D4", "”ªä“‡i”ªdªj",	{33,	6},	{139,	46}},
		{"QQ", "”ªä“‡i_–©j",	{33,	8},	{139,	48}},
		{"CC", "•ƒ“‡",		{27,	06},	{142,	12}},
		{"MC", "“ì’¹“‡",	{24,	17},	{153,	59}},
		{"D8", "Ã“ì`",	{35,	18},	{139,	29}},
		{"OD", "¬“cŒ´",	{35,	14},	{139,	9}},
		{"Z3", "ˆÉ“Œ",		{34,	54},	{139,	8}},
		{"D6", "‰º“c",		{34,	41},	{138,	58}},
		{"QK", "“ìˆÉ“¤",	{34,	38},	{138,	53}},
		{"G9", "Î˜Lè",	{34,	37},	{138,	51}},
		{"Z4", "“cq",		{34,	48},	{138,	46}},
		{"UC", "“à‰Y",		{35,	01},	{138,	53}},
		{"SM", "´…`",	{35,	01},	{138,	31}},
		{"Z5", "Ä’Ã",		{34,	52},	{138,	20}},
		{"OM", "Œä‘Oè",	{34,	37},	{138,	13}},
		{"MI", "•‘ã",		{34,	41},	{137,	37}},
		{"I4", "Ô‰Hª",	{34,	36},	{137,	11}},
		{"G4", "O‰Í",		{34,	44},	{137,	19}},
		{"G5", "Œ`Œ´",		{34,	47},	{137,	11}},
		{"G8", "ˆß‰Y",		{34,	53},	{136,	57}},
		{"ZD", "‹Sè",		{34,	54},	{136,	49}},
		{"NG", "–¼ŒÃ‰®",	{35,	05},	{136,	53}},
		{"G3", "l“ús`",	{34,	58},	{136,	38}},
		{"TB", "’¹‰H",		{34,	29},	{136,	49}},
		{"OW", "”ö˜h",		{34,	05},	{136,	12}},
		{"KN", "ŒF–ì",		{33,	56},	{136,	10}},
		{"UR", "‰Y_",		{33,	34},	{135,	54}},
		{"KS", "‹ø–{",		{33,	29},	{135,	46}},
		{"SR", "”’•l",		{33,	41},	{135,	23}},
		{"GB", "Œä–V",		{33,	51},	{135,	10}},
		{"H1", "‰º’Ã",		{34,	7},		{135,	8}},
		{"Z9", "ŠC“ì",		{34,	9},		{135,	12}},
		{"WY", "˜a‰ÌR",	{34,	13},	{135,	9}},
		{"TN", "’W—Ö",		{34,	20},	{135,	11}},
		{"KK", "ŠÖ‹ó“‡",	{34,	26},	{135,	12}},
		{"J2", "Šİ˜a“c",	{34,	28},	{135,	22}},
		{"IO", "ò‘å’Ã",	{34,	31},	{135,	24}},
		{"SI", "ä",		{34,	36},	{135,	28}},
		{"OS", "‘åã",		{34,	39},	{135,	26}},
		{"AM", "“òè",		{34,	42},	{135,	24}},
		{"J5", "¼‹{",		{34,	43},	{135,	20}},
		{"KB", "_ŒË",		{34,	41},	{135,	11}},
		{"AK", "–¾Î",		{34,	39},	{134,	59}},
		{"ST", "F–{",		{34,	21},	{134,	54}},
		{"EI", "]ˆä",		{34,	28},	{134,	50}},
		{"K1", "•P˜Hiü–j",	{34,	47},	{134,	40}},
		{"UN", "‰F–ì",		{34,	29},	{133,	57}},
		{"MM", "…“‡",		{34,	32},	{133,	44}},
		{"IZ", "…è",		{34,	24},	{133,	05}},
		{"TH", "’|Œ´",		{34,	20},	{132,	55}},
		{"Q9", "Œà",		{34,	14},	{132,	33}},
		{"Q8", "L“‡",		{34,	21},	{132,	28}},
		{"QA", "“¿R",		{34,	02},	{131,	48}},
		{"J9", "O“cK",	{34,	02},	{131,	35}},
		{"WH", "‰F•”",		{33,	56},	{131,	15}},
		{"CF", "’·•{",		{34,	01},	{131,	00}},
		{"A1", "’íq‘Ò",	{33,	56},	{130,	56}},
		{"TI", "“cƒmñ",	{33,	55},	{130,	55}},
		{"OH", "‘åR‚Ì•@",	{33,	55},	{130,	54}},
		{"HR", "“ì•—”‘",	{33,	57},	{130,	53}},
		{"MT", "¼R",		{33,	52},	{132,	43}},
		{"M3", "”g~•l",	{34,	6},		{132,	56}},
		{"M0", "—ˆ“‡q˜H",	{34,	8},		{132,	59}},
		{"L0", "¡¡",		{34,	04},	{133,	00}},
		{"NI", "V‹•l",	{33,	58},	{133,	16}},
		{"L8", "ˆÉ—\O“‡",	{33,	59},	{133,	33}},
		{"TX", "‘½“x’Ã",	{34,	17},	{133,	45}},
		{"AX", "Â–Ø",		{34,	22},	{133,	41}},
		{"J8", "—^“‡",		{34,	23},	{133,	49}},
		{"TA", "‚¼",		{34,	21},	{134,	03}},
		{"KM", "¬¼“‡",	{34,	01},	{134,	35}},
		{"J6", "‹k",		{33,	52},	{134,	38}},
		{"AW", "ˆ¢”g—RŠò",	{33,	46},	{134,	36}},
		{"HW", "“ú˜a²",	{33,	43},	{134,	33}},
		{"L7", "b‰Y",		{33,	33},	{134,	18}},
		{"MU", "ºŒË–¦",	{33,	16},	{134,	10}},
		{"KC", "‚’m",		{33,	30},	{133,	34}},
		{"V7", "{è",		{33,	23},	{133,	18}},
		{"ZH", "‹v—ç",		{33,	20},	{133,	15}},
		{"L6", "‚’m‰º“c",	{32,	56},	{133,	00}},
		{"TS", "“y²´…",	{32,	47},	{132,	58}},
		{"SU", "•Ğ“‡",		{32,	55},	{132,	42}},
		{"UW", "‰F˜a“‡",	{33,	14},	{132,	33}},
		{"N1", "“ú–¾",		{33,	55},	{130,	53}},
		{"N0", "»’Ã",		{33,	54},	{130,	53}},
		{"MO", "–åi",		{33,	57},	{130,	58}},
		{"AH", "Â•l",		{33,	57},	{131,	01}},
		{"O3", "Š¡“c",		{33,	48},	{131,	00}},
		{"BP", "•Ê•{",		{33,	18},	{131,	30}},
		{"QC", "‘å•ª",		{33,	16},	{131,	41}},
		{"X5", "²”Œ",		{32,	57},	{131,	58}},
		{"Z6", "×“‡",		{32,	26},	{131,	40}},
		{"MG", "‹{è",		{31,	54},	{131,	27}},
		{"AB", "–û’Ã",		{31,	35},	{131,	25}},
		{"X6", "u•zu",	{31,	29},	{131,	07}},
		{"QG", "‘å”‘",		{31,	01},	{130,	41}},
		{"KG", "­™“‡",	{31,	35},	{130,	34}},
		{"MK", "–è",		{31,	16},	{130,	18}},
		{"ZJ", "ˆ¢‹vª",	{32,	01},	{130,	11}},
		{"QH", "¼”V•\",	{30,	44},	{131,	00}},
		{"TJ", "íq“‡",	{30,	28},	{130,	58}},
		{"QI", "’†”V“‡",	{29,	51},	{129,	51}},
		{"QJ", "–¼£",		{28,	23},	{129,	30}},
		{"O9", "‰‚”ü",		{28,	19},	{129,	32}},
		{"NK", "’†é˜p`",	{26,	20},	{127,	50}},
		{"ZO", "‰«“ê",		{26,	11},	{127,	49}},
		{"NH", "“ß”e",		{26,	13},	{127,	40}},
		{"DJ", "“ì‘å“Œ",	{25,	52},	{131,	14}},
		{"R1", "•½—Ç",		{24,	49},	{125,	17}},
		{"IS", "ÎŠ_",		{24,	20},	{124,	10}},
		{"IJ", "¼•\",		{24,	21},	{123,	45}},
		{"YJ", "—^“ß‘",	{24,	27},	{122,	57}},
		{"O7", "…–“",		{32,	12},	{130,	22}},
		{"O5", "”ª‘ã",		{32,	31},	{130,	34}},
		{"HS", "–{“n£ŒË",	{32,	26},	{130,	13}},
		{"RH", "—è–k",		{32,	28},	{130,	02}},
		{"MS", "OŠp",		{32,	37},	{130,	27}},
		{"KU", "ŒF–{",		{32,	45},	{130,	34}},
		{"O6", "‘å–´“c",	{33,	01},	{130,	25}},
		{"OU", "‘å‰Y",		{32,	59},	{130,	13}},
		{"KT", "Œû”V’Ã",	{32,	36},	{130,	12}},
		{"NS", "’·è",		{32,	44},	{129,	52}},
		{"KO", "c@",		{32,	43},	{129,	50}},
		{"FE", "•Ÿ]",		{32,	42},	{128,	51}},
		{"QD", "²¢•Û",	{33,	9},	{129,	43}},
		{"X2", "•½ŒË£ŒË",	{33,	22},	{129,	35}},
		{"ZL", "‰¼‰®",		{33,	28},	{129,	51}},
		{"KA", "“‚’Ã",		{33,	28},	{129,	58}},
		{"QF", "”‘½",		{33,	37},	{130,	24}},
		{"X3", "‹½ƒm‰Y",	{33,	45},	{129,	41}},
		{"QE", "ŒµŒ´",		{34,	12},	{129,	18}},
		{"O1", "‘Î”n",		{34,	16},	{129,	19}},
		{"N5", "‘Î”n”ä“cŸ",{34,	39},	{129,	29}},
		{"K5", "”‹",		{34,	26},	{131,	25}},
		{"ZK", "{²",		{34,	38},	{131,	36}},
		{"HA", "•l“c",		{34,	54},	{132,	04}},
		{"SK", "‹«",		{35,	33},	{133,	15}},
		{"SA", "¼‹½",		{36,	12},	{133,	20}},
		{"ZE", "“cŒã",		{35,	36},	{134,	19}},
		{"T6", "’Ã‹R",	{35,	39},	{134,	50}},
		{"T2", "‹{’Ã",		{35,	32},	{135,	12}},
		{"MZ", "•‘’ß",		{35,	29},	{135,	23}},
		{"XM", "“Ö‰ê",		{35,	40},	{136,	4}},
		{"ZG", "O‘",		{36,	15},	{136,	9}},
		{"T1", "‹à‘ò",		{36,	37},	{136,	36}},
		{"Z7", "—Ö“‡",		{37,	24},	{136,	54}},
		{"SZ", "”\“o",		{37,	30},	{137,	9}},
		{"XO", "µ”ö",		{37,	03},	{136,	58}},
		{"XQ", "•š–Ø•xR",	{36,	48},	{137,	04}},
		{"SN", "V–©",		{36,	47},	{137,	07}},
		{"TY", "•xR",		{36,	46},	{137,	13}},
		{"T3", "’¼]’Ã",	{37,	11},	{138,	15}},
		{"ZC", "”è",		{37,	21},	{138,	31}},
		{"S6", "VŠƒ¼`",	{37,	56},	{139,	04}},
		{"I5", "VŠƒ“Œ`",	{37,	59},	{139,	13}},
		{"ZN", "¬–Ø",		{37,	49},	{138,	17}},
		{"RZ", "—¼’Ã",		{38,	05},	{138,	26}},
		{"S0", "²“n",		{38,	19},	{138,	31}},
		{"QR", "ˆ¾“‡",		{38,	28},	{139,	15}},
		{"ZB", "‘lƒ–ŠÖ",	{38,	34},	{139,	33}},
		{"S9", "ğ“c",		{38,	55},	{139,	49}},
		{"ZQ", "”ò“‡",		{39,	11},	{139,	33}},
		{"S1", "H“c",		{39,	45},	{140,	04}},
		{"S2", "‘Dì`",	{39,	55},	{139,	51}},
		{"ZI", "’j­",		{39,	57},	{139,	42}},
		{"FK", "[‰Y",		{40,	39},	{139,	56}},

	};

	int ParseInt(const char* str, int strSize, int& dest)
	{
		if(strSize >= 32)
			return -1;
		if(str == NULL)
			return -1;

		char buf[32];
		memset(buf, 0, sizeof(buf));

		memcpy(buf, str, strSize);

		istringstream is(buf);
		is >> dest;

		return 0;
	}

	int ParseAreaCode(const char* str, int strSize, AreaCode& areaCode)
	{
		if(strSize > 2)
			return -1;
		if(str == NULL)
			return -1;

		const int loopNum = sizeof(AreaCodeNameTable)/sizeof(AreaDataEntry);
		for( int i = 0; i < loopNum; i++) {
			if( str[0] == AreaCodeNameTable[i].codestring[0] &&
				str[1] == AreaCodeNameTable[i].codestring[1]) {
				areaCode = static_cast<AreaCode>(i);
				return 0;
			}
		}

		return -1;
	}

	int ParseTide(TideDataLineTimeTide& tideDataLineTimeTide, const time_t& date, Tide& tide)
	{
		tm baseTm;
		localtime_s(&baseTm, &date);

		int temp = 0;
		int err = ParseInt((char*)(&tideDataLineTimeTide.time.hourdata), sizeof(tideDataLineTimeTide.time.hourdata), temp);
		if(err != 0)
			return -1;
		baseTm.tm_hour = temp;

		err = ParseInt((char*)(&tideDataLineTimeTide.time.mindata), sizeof(tideDataLineTimeTide.time.mindata), temp);
		if(err != 0)
			return -1;
		baseTm.tm_min = temp;

		tide.time = mktime(&baseTm);

		err = ParseInt((char*)(&tideDataLineTimeTide.tide.data), sizeof(tideDataLineTimeTide.tide.data), temp);
		if(err != 0)
			return -1;
		tide.extremal = temp;

		return 0;
	}

	int ParseLine(const string& line, DailyTide& dailyTide)
	{
		TideDataLine dataLine;
		memset(&dataLine, 0, sizeof(dataLine));

		size_t size = sizeof(dataLine);
		int len = line.length();
		if(len != size)
		{
			return -1;
		}

		strcpy_s((char*)(&dataLine), 136, line.substr(0, line.length()-1).c_str());

		// 0-23 hour tide
		int err = 0;
		for(int i = 0; i < 24; i++) {
			err = ParseInt((char*)(&dataLine.tides[i]), sizeof(dataLine.tides[i]), dailyTide.tides[i]);
			if(err != 0) {
				return -1;
			}
		}

		// date(year/month/day)
		struct tm work_tm;
		memset(&work_tm, 0, sizeof(work_tm));

		int temp = 0;
		err = ParseInt((char*)(&dataLine.date.yeardata), sizeof(dataLine.date.yeardata), temp);
		if(err != 0)
			return -1;
		work_tm.tm_year = temp + 2000 - 1900;

		err = ParseInt((char*)(&dataLine.date.monthdata), sizeof(dataLine.date.monthdata), temp);
		if(err != 0)
			return -1;
		work_tm.tm_mon = temp-1;

		err = ParseInt((char*)(&dataLine.date.daydata), sizeof(dataLine.date.daydata), temp);
		if(err != 0)
			return -1;
		work_tm.tm_mday = temp;
		dailyTide.date = mktime(&work_tm);

		// areacode
		err = ParseAreaCode((char*)(&dataLine.areacodestring), sizeof(dataLine.areacodestring), dailyTide.areacode);
		if(err != 0)
			return -1;

		// highests
		for(int i = 0; i < 4; i++) {
			Tide tide;
			ParseTide(dataLine.heighests[i], dailyTide.date, tide);
			if(err != 0)
				return -1;

			if(tide.extremal == 999)
				break;

			dailyTide.heighests[i] = tide;
		}

		// lowest
		for(int i = 0; i < 4; i++) {
			Tide tide;
			ParseTide(dataLine.lowests[i], dailyTide.date, tide);
			if(err != 0)
				return -1;

			if(tide.extremal == 999)
				break;

			dailyTide.lowests[i] = tide;
		}

		return 0;
	}

	int ParseData(const string& filename, vector<DailyTide> &vec)
	{
		ifstream ifs(filename);
		if(!ifs.is_open())
		{
			cout << "error" << endl;
			return -1;
		}

		string line;
		while(getline(ifs, line)) {
			DailyTide dailyTide = {0};

			int err = ParseLine(line, dailyTide);
			if(err != 0)
				return -1;

			vec.push_back(dailyTide);
		}

		if(vec.empty())
			return -1;

		return 0;
	}

	std::string GetAreaCodeString(AreaCode areaCode)
	{
		const int areaCodeNum = sizeof(AreaCodeNameTable)/sizeof(AreaDataEntry);
		if(areaCode < 0 || areaCodeNum <= areaCode)
			return "";

		return AreaCodeNameTable[static_cast<int>(areaCode)].codestring;
	}

	AreaDataEntry GetAreaDataEntry(AreaCode areaCode)
	{
		const int areaCodeNum = sizeof(AreaCodeNameTable)/sizeof(AreaDataEntry);
		if(areaCode < 0 || areaCodeNum <= areaCode)
		{
			// assertion
			// t.b.d
		}
		return AreaCodeNameTable[static_cast<int>(areaCode)];
	}
}
