#ifndef INCLUDED_TEACHER_PREFERENCE_HPP
#define INCLUDED_TEACHER_PREFERENCE_HPP
#include <string>
#include <vector>

using namespace std;
using std::string;

class TeacherPreference {

	public:
	    TeacherPreference(string,string,string,int,std::vector<int>,int,std::vector<int>);
	private:
	    string teacher;
	    string discipline;
	    string curriculum;
	    int weight1;
	    std::vector<int> primaryHours;
	    int weight2;
	    std::vector<int> secundaryHours;
};


#endif // INCLUDED_TEACHER_PREFERENCE_HPP
