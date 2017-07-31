#include "../hpp/instance.hpp"


int Instance::counter = 0;


Instance::Instance(){
    
}


Instance::Instance(string discipline , string curriculum, int repetition, int capacity){
    this->discipline = discipline;
    this->curriculum = curriculum;
    this->repetition = repetition;
    this->capacity = capacity;

    increment();
}

void Instance::increment(){
	counter++;
	this->id = counter;
}


ostream& operator<<(ostream& os , Instance& i) {
   
    os << "Instance ID: ";
    os << i.getID() << endl;
    os << "Discipline " << i.getDiscipline() ;
    os << "Curriculum " << i.getCurriculum() ;

    os << endl ;    

    return os;
}
