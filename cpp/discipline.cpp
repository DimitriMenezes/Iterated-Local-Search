#include "../hpp/discipline.hpp"

int Discipline::counter = 0;

Discipline::Discipline(string discipline){

    this->discipline = discipline;
}

void Discipline::increment(){
	counter++;
	this->id = counter;
}

