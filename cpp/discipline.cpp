#include "../hpp/discipline.hpp"

int Discipline::counter = 1;

Discipline::Discipline(string discipline){
	this->id = counter;
    this->discipline = discipline;
}

void Discipline::increment(){
	counter++;	
}


ostream& operator<<(ostream& os , Discipline& d) {

	os << "Teacher: " << d.getDiscipline() << " id: ";
	os << d.getID() << std::endl ;

	return os;
}

