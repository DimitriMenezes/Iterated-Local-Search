#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <stdlib.h>
#include "cpp/requirement.cpp"
#include "cpp/teacher_preference.cpp"
#include "cpp/room.cpp"
#include "cpp/teacher.cpp"
#include "cpp/discipline.cpp"
#include "cpp/curriculum.cpp"
#include "cpp/instance.cpp"
#include "cpp/solution.cpp"

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


int main(){

//PRE PROCESSMENT

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

    Discipline d(v[1]);
    DisciplineSet.push_back(d);
}

//PREFERENCIA
while(!read2.eof()){
    read2.getline(buffer,256);

    vector<string> v = explode(buffer,',');

    string teacher = v[0];
    string discipline = v[1];
    string curriculum = v[2];
    int weight1 = atoi(v[3].c_str());

    string hours = v[4] + " "+ v[5] +" "+ v[6];
    vector<int> ph1;
    vector<int> ph2 = {1,2,3};

    string aux;
    for(int i = 0; i < hours.length(); i++){
       if(hours[i] == '{'){continue;}
       if(hours[i] == '}'){break;}
       aux += hours[i];
    }

    vector<string> primaryHours = explode(aux,' ');

    TeacherPreference t(teacher,discipline,curriculum,10,{10,20,30},9,{5,9});

    TeacherPreferenceSet.push_back(t);


    TeacherSet.push_back(v[0]);

}
//SALAS
while(!read3.eof()){
    read3.getline(buffer,256);
    vector<string> v = explode(buffer,',');

    Room r(v[0],atoi(v[1].c_str()));
    RoomSet.push_back(r);
}

//CURRICULOS
while(!read4.eof()){
    read4.getline(buffer,256);
    Curriculum c(buffer);
    CurriculumSet.push_back(c);
    c.increment();
}

while(!read5.eof()){
    read5.getline(buffer,256);
    Curriculum c(buffer);
    CurriculumSet.push_back(c);
    c.increment();
}

while(!read6.eof()){
    read6.getline(buffer,256);
    Curriculum c(buffer);
    CurriculumSet.push_back(c);
    c.increment();
}

//VECTOR DE ALOCACAÇAO DE AULAS
vector<Instance> InstanceSet;

for(std::vector<Requirement>::iterator it = RequirementSet.begin(); it != RequirementSet.end(); ++it) {
    //cout << *it << endl;

    for(int i = 1; i <= it->getRepetitions(); i++){
         Instance inst(it->getDiscipline(),it->getCurriculum(),it->getRepetitions(),it->getCapacity());
         InstanceSet.push_back(inst);
    }

 }

 for(std::vector<Curriculum>::iterator it = CurriculumSet.begin(); it != CurriculumSet.end(); ++it) {
     //cout << it->getID();
     //cout << " ";
     //cout << it->getCurriculum() << endl;
 }

 Solution s;

 srand((unsigned)time(0));
 for(std::vector<Instance>::iterator it = InstanceSet.begin(); it != InstanceSet.end(); ++it) {

        int random_teacher = rand() % TeacherSet.size() + 1;
        int random_room = rand() % RoomSet.size() + 1;
        int random_hour = rand() % 40 + 1;

        int instanceID = it->getID();

        s.addInstanceInTeacher(random_teacher,instanceID);
        s.addInstanceInHour(random_hour,instanceID);
        s.addInstanceInRoom(random_room,instanceID);
 }

 cout << s;

//END MAIN
}


