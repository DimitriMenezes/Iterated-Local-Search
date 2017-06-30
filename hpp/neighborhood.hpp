#ifndef INCLUDED_NEIGHBORHOOD_HPP
#define INCLUDED_NEIGHBORHOOD_HPP
#include <string>
#include <vector>

using namespace std;
using std::string;

class Neighborhood {
public:
	Neighborhood(Solution);
	void moveSchedule(Solution,int,int,int);
	void moveRoom(Solution,int,int);
	void localSearch();

private:
	Solution initialSolution;
	std::vector<Solution> solutionSet;
};

#endif