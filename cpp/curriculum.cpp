#include "../hpp/curriculum.hpp"

int Curriculum::counter = 0;

Curriculum::Curriculum(string curriculum){
    this->curriculum = curriculum;
}

void Curriculum::increment(){
    counter++;
    id = counter;
}


ostream& operator<<(ostream& os , Curriculum& c)
{
    //char ch = c.getID();
    string s = c.getCurriculum() + " | ";

    os << c.getID();
    return os;
}
