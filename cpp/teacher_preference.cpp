#include "../hpp/teacher_preference.hpp"

TeacherPreference::TeacherPreference(string teacher,string discipline,string curriculum,int weight1,std::vector<int> primaryHours
                                     ,int weight2,std::vector<int> secundaryHours){
this->teacher = teacher;
this->discipline = discipline;
this->curriculum = curriculum;
this->weight1 = weight1;
this->primaryHours = primaryHours;
this->weight2 = weight2;
this->secundaryHours = secundaryHours;
}
