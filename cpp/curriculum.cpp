#include "../hpp/curriculum.hpp"

int Curriculum::counter = 1;

Curriculum::Curriculum(){
	
}

Curriculum::Curriculum(string curriculum, string type){
	this->id = counter;
    this->curriculum = curriculum;
    this->type = type;
}

void Curriculum::increment(){
    counter++;
}


ostream& operator<<(ostream& os , Curriculum& c)
{
    string s = c.getCurriculum() + " | ";
    os << s ;
    os << c.getID();
    return os;
}
