
#ifndef INCLUDED_SOLUTION_HPP
#define INCLUDED_SOLUTION_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <stdlib.h>
using namespace std;
using std::string;



class Solution {
public:
	Solution();
	void addInstanceInTeacher(int,int);
	void addInstanceInHour(int,int);
	void addInstanceInRoom(int,int);
	int getObjective(){return this->objective;};

	vector<std::pair <int,int>> room;
	vector<std::pair <int,int>> teacher;
	vector<std::pair <int,int>> hour;

private:
	int objective;


friend ostream& operator<<(ostream& , Solution&);
};

#endif
