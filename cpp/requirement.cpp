#include "../hpp/requirement.hpp"
using namespace std;
using std::string;
#include <string>

Requirement::Requirement(string curriculum,string discipline,int repetitions,int capacity){
    this->discipline = discipline;
    this->curriculum = curriculum;
    this->repetitions = repetitions;
    this->capacity = capacity;
}

ostream& operator<<(ostream& os , Requirement& r)
{
    string s = "Oferta: " + r.discipline + " | " + r.curriculum + " | "; // + itoa(r.capacity) + " | " + itoa(r.repetitions) ;

    os << s;
    return os;
}
