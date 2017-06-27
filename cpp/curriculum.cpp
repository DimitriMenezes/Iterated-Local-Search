#include "../hpp/curriculum.hpp"

int Curriculum::counter = 1;

Curriculum::Curriculum(string curriculum){
	this->id = counter;
    this->curriculum = curriculum;
}

void Curriculum::increment(){
    counter++;
}


ostream& operator<<(ostream& os , Curriculum& c)
{
    //char ch = c.getID();
    string s = c.getCurriculum() + " | ";
    os << s ;
    os << c.getID();
    return os;
}
