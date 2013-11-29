#include "stdafx.h"

#include <fstream>
#include "OsakanaTools.h"

using namespace std;

namespace osakanatools
{
	bool file_exists(const string& filename) {
		return file_exists(filename.c_str());
	}

	bool file_exists(char *filename){
		ifstream in(filename,ios::in); //try to open the file for reading
		if(!in) return false; //can not open the file=> return false
		else return true;   //otherwise return true
	}

}
