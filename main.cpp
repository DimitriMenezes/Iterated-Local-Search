#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <stdlib.h>
#include <typeinfo>
#include "cpp/requirement.cpp"
#include "cpp/teacher_preference.cpp"
#include "cpp/room.cpp"
#include "cpp/teacher.cpp"
#include "cpp/discipline.cpp"
#include "cpp/curriculum.cpp"
#include "cpp/instance.cpp"
#include "cpp/solution.cpp"
#include "cpp/neighborhood.cpp"
#include "cpp/morning_curriculum.cpp"
#include "cpp/afternoon_curriculum.cpp"
#include "cpp/evening_curriculum.cpp"
#include <utility>
#include <regex>
using namespace std;
using std::string;


vector<string> explode(const string& str, const char& ch) {
    string next;
    vector<string> result;

    // For each character in the string
    for (string::const_iterator it = str.begin(); it != str.end(); it++) {
        // If we've hit the terminal character
        if(*it == '"' || *it == '<' || *it == '>'){
            continue;
        }

        if (*it == ch) {
            // If we have some characters accumulated
            if (!next.empty()) {
                // Add them to the result vector
                result.push_back(next);
                next.clear();
            }
        } else {
            // Accumulate the next character into the sequence
            next += *it;
        }
    }
    if (!next.empty())
         result.push_back(next);
    return result;
}

 // cout << typeid(x).name() << endl;

int main(){

//PRE PROCESSAMENTO

vector<TeacherPreference> TeacherPreferenceSet;
vector<Requirement> RequirementSet;
vector<Room> RoomSet;
vector<Curriculum> CurriculumSet;
vector<Teacher> TeacherSet;
vector<Discipline> DisciplineSet;

ifstream read1("data/requirement.txt");
ifstream read2("data/preference.txt");
ifstream read3("data/rooms.txt");

ifstream read4("data/morning_curriculum.txt");
ifstream read5("data/afternoon_curriculum.txt");
ifstream read6("data/noon_curriculum.txt");

char buffer[256];

//OFERTAS
while(!read1.eof()){
    read1.getline(buffer,256);

    vector<string> v = explode(buffer,',');

    string curriculum = v[0];
    string discipline = v[1];
    int repetition = atoi(v[2].c_str());
    int capacity = atoi(v[3].c_str());

    Requirement r(curriculum,discipline,repetition,capacity);
    RequirementSet.push_back(r);


    //DISCIPLINAS
    int counter = 0;

    for(std::vector<Discipline>::iterator it = DisciplineSet.begin(); it != DisciplineSet.end(); ++it) {
        if(it->getDiscipline() == discipline){
            counter++;
        }
    }

    if(counter > 0){

    }else{
        Discipline d(discipline);
        DisciplineSet.push_back(d);
        d.increment();
    }
}

//PREFERENCIA
while(!read2.eof()){
    read2.getline(buffer,256);

    vector<string> v = explode(buffer,',');

    string teacher = v[0];
    string discipline = v[1];
    string curriculum = v[2];

    //Primary Preference
    int weight1 = atoi(v[3].c_str());
    string character = v[4];
    string hour;
    vector<int> primaryHours;
    for(int j = 0; j < character.length(); j ++){

        if(character[j] == '{'){}
        else if(character[j] == ' ' || character[j] == '}'){
            int value = atoi(hour.c_str());
            primaryHours.push_back(value);
            string temp;
            hour = temp;
        }
        else {
            hour += character[j];
        }
    }

    //Secundary Preference
    int weight2 = atoi(v[5].c_str());
    character = v[6];
    string x;
    hour = x;
    vector<int> secundaryHours;
    for(int j = 0; j < character.length(); j ++){
        if(character[j] == '{'){}
        else if(character[j] == ' ' || character[j] == '}'){
            int value = atoi(hour.c_str());
            secundaryHours.push_back(value);
            string temp;
            hour = temp;
        }
        else {
            hour += character[j];
        }
    }

    TeacherPreference t(teacher,discipline,curriculum,weight1,primaryHours,weight2,secundaryHours);
    TeacherPreferenceSet.push_back(t);

    //PROFESSORES
    int counter = 0;

    for(std::vector<Teacher>::iterator it = TeacherSet.begin(); it != TeacherSet.end(); ++it) {
        if(it->getTeacherName() == teacher){
            counter++;
        }
    }

    if(counter > 0){

    }else{
        Teacher currentTeacher(teacher);
        TeacherSet.push_back(currentTeacher);
        currentTeacher.increment();
    }
}
//SALAS
while(!read3.eof()){
    read3.getline(buffer,256);
    vector<string> v = explode(buffer,',');

    Room r(v[0],atoi(v[1].c_str()));
    RoomSet.push_back(r);
    r.increment();
}

//CURRICULOS
while(!read4.eof()){
    read4.getline(buffer,256);
    MorningCurriculum c(buffer);
    CurriculumSet.push_back(c);
    c.increment();
}

while(!read5.eof()){
    read5.getline(buffer,256);
    AfternoonCurriculum c(buffer);
    CurriculumSet.push_back(c);
    c.increment();
}

while(!read6.eof()){
    read6.getline(buffer,256);
    EveningCurriculum c(buffer);
    CurriculumSet.push_back(c);
    c.increment();
}

//VECTOR DE ALOCACA�AO DE AULAS
vector<Instance> InstanceSet;
for(std::vector<Requirement>::iterator it = RequirementSet.begin(); it != RequirementSet.end(); ++it) {
    for(int i = 1; i <= it->getRepetitions(); i++){
         Instance inst(it->getDiscipline(),it->getCurriculum(),it->getRepetitions(),it->getCapacity());
         InstanceSet.push_back(inst);
    }
}

 //GERANDO SOLUCAO INICIAL
 Solution s;
 srand((unsigned)time(0));

 for(std::vector<Instance>::iterator it = InstanceSet.begin(); it != InstanceSet.end(); ++it) {
        int random_teacher;
        int random_room;// = rand() % RoomSet.size() + 1;
        int random_hour;
        //int random_hour = rand() % 39;
        int instanceID = it->getID();

        //s.addInstanceInHour(random_hour,instanceID);


        bool allocated = false;
        Teacher currentTeacher;
        //garantir que o professor pode lecionar a disciplina
        do{
            random_teacher = rand() % TeacherSet.size() + 1;

            for(int i = 0; i < TeacherSet.size(); i++){
                if(random_teacher == TeacherSet[i].getID()){
                    currentTeacher = TeacherSet[i];
                    break;
                }
            }

            for(int j = 0; j < TeacherPreferenceSet.size() ; j++ ){
                if(currentTeacher.getTeacherName() == TeacherPreferenceSet[j].getTeacherName()){
                    if(it->getCurriculum() == TeacherPreferenceSet[j].getCurriculum() &&
                           it->getDiscipline() == TeacherPreferenceSet[j].getDiscipline() ){
                                    //cout << random_teacher << "professor alocado em " << instanceID << endl;
                                    allocated = true;
                                    s.addInstanceInTeacher(random_teacher,instanceID);
                                    break;
                    }
                }
            }
        }while(allocated == false);

        //garantir o hor�rio de preferencia do professor
        allocated = false;
        do{
            random_hour = rand() % 39;
            currentTeacher;

           for(int j = 0; j < TeacherPreferenceSet.size() ; j++ ){
                if(currentTeacher.getTeacherName() == TeacherPreferenceSet[j].getTeacherName() &&
                    it->getCurriculum() == TeacherPreferenceSet[j].getCurriculum() &&
                    it->getDiscipline() == TeacherPreferenceSet[j].getDiscipline() ){

                    auto vector1 = TeacherPreferenceSet[j].getMainHours();
                    auto vector2 = TeacherPreferenceSet[j].getSecundaryHours();
                    vector1.insert( vector1.end(), vector2.begin(), vector2.end() );

                    auto possibleHours = vector1;

                    for(int k = 0; k < possibleHours.size(); k++){
                        if(random_hour == possibleHours[k]){
                            allocated = true;
                            s.addInstanceInHour(random_hour,instanceID);
                            break;
                        }
                    }
                }
            }
        }while(allocated == false);

        //garantir sala � capaz de comportar a aula
        allocated = false;
        Room r;
        do{
            random_room = rand() % RoomSet.size() + 1;

            for(int i =0; i < RoomSet.size() ; i++){
                if(random_room == RoomSet[i].getID()){
                    r = RoomSet[i];
                    break;
                }
            }

            if(r.getRoomCapacity() >= it->getClassCapacity() ){
                s.addInstanceInRoom(random_room,instanceID);
                allocated = true;
                break;
            }
        }while(allocated == false);

        //Atribui��o da fun��o objetivo
        for(int i = 0; i < TeacherSet.size(); i++){
            if(random_teacher == TeacherSet[i].getID()){
                for(int j = 0; j < TeacherPreferenceSet.size(); j++){
                    if(currentTeacher.getTeacherName() == TeacherPreferenceSet[j].getTeacherName() &&
                    it->getCurriculum() == TeacherPreferenceSet[j].getCurriculum() &&
                    it->getDiscipline() == TeacherPreferenceSet[j].getDiscipline() ){

                    	vector<int> primaryHours =  TeacherPreferenceSet[j].getMainHours();
                    	for(int k = 0; k < primaryHours.size(); k++){
                    		if(primaryHours[k] == random_hour){
                    			s.objective += TeacherPreferenceSet[j].getMainWeight();
                    			break;
                    		}
                    	}

                    	vector<int> secundaryHours =  TeacherPreferenceSet[j].getSecundaryHours();
                    	for(int k = 0; k < secundaryHours.size(); k++){
                    		if(secundaryHours[k] == random_hour){
	                    		s.objective += TeacherPreferenceSet[j].getSecundaryWeight();
	                    		break;
                    		}
                    	}
                    }
                }
            }
        }
 }

cout << s;

Neighborhood n(s);


cout << CurriculumSet[50] ;


//END MAIN
}


