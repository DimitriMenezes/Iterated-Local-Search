#ifndef INCLUDED_REQUIREMENT_HPP
#define INCLUDED_REQUIREMENT_HPP
using namespace std;
using std::string;
#include <string>

class Requirement {

	public:
        Requirement(string,string,int,int);
        string getDiscipline(void){return discipline;}
        string getCurriculum(void){return curriculum;}
        int getRepetitions(void){return repetitions;}
        int getCapacity(void){return capacity;}

	private:
		string discipline;
		string curriculum;
		int repetitions;
		int capacity;

	friend ostream& operator<<(ostream& , Requirement& );
};


#endif // INCLUDED_REQUIREMENT_HPP
