#ifndef INCLUDED_CURRICULUM_HPP
#define INCLUDED_CURRICULUM_HPP
using namespace std;
using std::string;

class Curriculum{

public:
    static int counter;
    Curriculum(string);
    string getCurriculum(){return this->curriculum;};
    setCurriculum(string curriculum){this->curriculum = curriculum;};

    int getID(){ return this->id;};

    void increment();
private:
    int id;
    string curriculum;

friend ostream& operator<<(ostream& , Curriculum&);

};
#endif
