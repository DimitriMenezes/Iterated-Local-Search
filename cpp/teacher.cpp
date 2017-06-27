#include "../hpp/teacher.hpp"

int Teacher::counter = 0;

Teacher::Teacher(string teacher){
    this->teacher = teacher;
}

void Teacher::increment(){
	counter++;
	this->id = counter;
}
