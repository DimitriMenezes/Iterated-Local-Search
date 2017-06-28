#ifndef INCLUDED_TEACHER_HPP
#define INCLUDED_TEACHER_HPP
using namespace std;
using std::string;

class Teacher {

	public:
	    static int counter;
	    Teacher(string);
	    Teacher();
	    int getID(){ return this->id;};
	    string getTeacherName(){return this->teacher;};

	    void increment();
	private:
	    int id;
		string teacher;
	friend ostream& operator<<(ostream& , Teacher&);	
};


#endif // INCLUDED_TEACHER_HPP
