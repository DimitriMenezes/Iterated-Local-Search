#ifndef INCLUDED_TEACHER_HPP
#define INCLUDED_TEACHER_HPP
using namespace std;
using std::string;

class Teacher {

	public:
	    static int counter;
	    Teacher(string);
	    int getID(){ return this->id;};

	    void increment();
	private:
	    int id;
		string teacher;
};


#endif // INCLUDED_TEACHER_HPP
