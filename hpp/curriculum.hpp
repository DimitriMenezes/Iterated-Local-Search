#ifndef INCLUDED_CURRICULUM_HPP
#define INCLUDED_CURRICULUM_HPP
using namespace std;
using std::string;

class Curriculum{

public:
    static int counter;

    Curriculum();
    Curriculum(string,string);
    string getCurriculum(){return this->curriculum;};
    setCurriculum(string curriculum){this->curriculum = curriculum;};
    void setCurriculumType(string type){this->type = type;};
    string getCurriculumType(){return this->type;};

    int getID(){ return this->id;};
    void increment();
private:
    int id;
    string curriculum;
    string type; //MorningCurriculum , AfternoonCurriculum , EveningCurriculum

friend ostream& operator<<(ostream& , Curriculum&);

};
#endif
