#include "../hpp/neighborhood.hpp"

Neighborhood::Neighborhood(Solution initialSolution){
	this->initialSolution = initialSolution;
	this->solutionSet.push_back(initialSolution);
	std::cout << "Vizinhaca";
}

void Neighborhood::moveSchedule(Solution s, int instance, int oldSchedule, int newSchedule){

}

void Neighborhood::moveRoom(Solution s, int instance, int newRoom){

}
