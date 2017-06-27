#include "../hpp/instance.hpp"


int Instance::counter = 0;

Instance::Instance(string curriculum, string discipline, int repetition, int capacity){
    this->curriculum = curriculum;
    this->discipline = discipline;
    this->repetition = repetition;
    this->capacity = capacity;

    increment();
}

void Instance::increment(){
	counter++;
	this->id = counter;
}


ostream& operator<<(ostream& os , Instance& i)
{
    string s = " " ;
    os << s;
    return os;
}
