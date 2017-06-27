#include "../hpp/teacher.hpp"

int Teacher::counter = 1;

Teacher::Teacher(string teacher){
	this->id = counter;
    this->teacher = teacher;
}

void Teacher::increment(){
	counter++;		
}

ostream& operator<<(ostream& os , Teacher& t) {

	os << "Teacher: " << t.getTeacherName() << " id: ";
	os << t.getID() << std::endl ;

	return os;
}
