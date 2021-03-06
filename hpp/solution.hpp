
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

	void moveInstanceToRoom(int,int);
	void moveInstanceToHour(int,int);

    int objective;
    bool isCandidateSolution;
	vector<std::pair <int,int>> room;     //<room_id, instance_id>
	vector<std::pair <int,int>> teacher;  //<teacher_id, instance_id>
	vector<std::pair <int,int>> hour; 	  //<hour, instance_id>
	void setObjective(int objective){this->objective = objective;};

private:



friend ostream& operator<<(ostream& , Solution&);
};

#endif