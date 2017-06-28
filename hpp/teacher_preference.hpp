#ifndef INCLUDED_TEACHER_PREFERENCE_HPP
#define INCLUDED_TEACHER_PREFERENCE_HPP
#include <string>
#include <vector>

using namespace std;
using std::string;

class TeacherPreference {

	public:
	    TeacherPreference(string,string,string,int,std::vector<int>,int,std::vector<int>);

	    string getTeacherName(){return this->teacher;};
	    string getDiscipline(){return this->discipline;};
	    string getCurriculum(){return this->curriculum;};
	    int getMainWeight(){return this->weight1;};
	    int getSecundaryWeight(){return this->weight2;};
	    std::vector<int> getMainHours(){return this->primaryHours;};
	    std::vector<int> getSecundaryHours(){return this->secundaryHours;};

	private:
	    string teacher;
	    string discipline;
	    string curriculum;
	    int weight1;
	    std::vector<int> primaryHours;
	    int weight2;
	    std::vector<int> secundaryHours;
	    
	friend ostream& operator<<(ostream& , TeacherPreference&);    
};


#endif // INCLUDED_TEACHER_PREFERENCE_HPP
