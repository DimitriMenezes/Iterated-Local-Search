#ifndef INCLUDED_ILS_HPP
#define INCLUDED_ILS_HPP

#include "../hpp/curriculum.hpp"
#include "../hpp/solution.hpp"
using namespace std;
using std::string;

class ILS{

public:
	ILS( vector<Instance>, vector<Requirement>, vector<TeacherPreference>, vector<Room> ,
		 vector<Teacher>, vector<Discipline> , vector<Curriculum>
	 );

	void start();
	Solution generateInitialSolution();
	Solution LocalSearch(Solution);
	Solution Perturbation1(Solution);
	Solution Perturbation2(Solution);
	Solution AcceptanceCriteria(Solution,Solution);

	void objectiveFunction(Solution&);

	bool constraint2(Teacher);

	bool constraint3(int,Teacher,Instance);

	bool constraint4(int,int,Solution);

	bool constraint5(Teacher,Instance);

	bool constraint6(int,int,Solution);

	bool constraint7(Room,Instance);

	bool constraint8(int,Instance,Solution);

	bool constraint9(int, Solution, Instance);

	bool constraint10(int, Instance);

	void printSolution(Solution);

private:
	vector<Instance> InstanceSet;
	vector<Requirement> RequirementSet;
	vector<TeacherPreference> TeacherPreferenceSet;
	vector<Room> RoomSet;
	vector<Teacher> TeacherSet;
	vector<Discipline> DisciplineSet;
	vector<Curriculum> CurriculumSet;
	vector<int> historic;

};
#endif
