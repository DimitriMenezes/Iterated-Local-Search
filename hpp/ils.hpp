#ifndef INCLUDED_ILS_HPP
#define INCLUDED_ILS_HPP

#include "../hpp/curriculum.hpp"
#include "../hpp/solution.hpp"
#include "../hpp/neighborhood.hpp"
using namespace std;
using std::string;

class ILS{


public:
	ILS(vector<Instance>, vector<Requirement>, vector<TeacherPreference>, vector<Room> ,
		vector<Teacher>, vector<Discipline> , vector<Curriculum>
	 );

	void start();
	Solution generateInitialSolution();
	Solution LocalSearch(Solution);
	void Perturbation();
	Solution AcceptanceCriterion(Solution,Solution);

	void objectiveFunction(Solution&);

	bool restricao2(Teacher);

	bool restricao3(int,Teacher,Instance);

	bool restricao5(Teacher,Instance);

	bool restricao7(Room,Instance);

private:
	vector<Instance> InstanceSet;
	vector<Requirement> RequirementSet;
	vector<TeacherPreference> TeacherPreferenceSet;
	vector<Room> RoomSet;
	vector<Teacher> TeacherSet;
	vector<Discipline> DisciplineSet;
	vector<Curriculum> CurriculumSet;

};
#endif
