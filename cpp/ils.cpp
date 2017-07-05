#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <stdlib.h>
#include <utility>
#include <regex>
#include <typeinfo>

#include "../hpp/ils.hpp"

ILS::ILS(vector<Instance> InstanceSet, vector<Requirement> RequirementSet,
		vector<TeacherPreference> TeacherPreferenceSet,
		vector<Room> RoomSet,
		vector<Teacher> TeacherSet, vector<Discipline> DisciplineSet , vector<Curriculum> CurriculumSet
	 ){

	this->InstanceSet = InstanceSet;
	this->RequirementSet = RequirementSet;
	this->TeacherPreferenceSet = TeacherPreferenceSet;
	this->RoomSet = RoomSet;
	this->TeacherSet = TeacherSet;
	this->DisciplineSet = DisciplineSet;
	this->CurriculumSet = CurriculumSet;

	start();
}

void ILS::start(){

	Solution s1 = generateInitialSolution();
    //cout << s1;
    cout << "Solucao inicial: " ;
    cout << s1.objective << endl ;

    Solution s2 = LocalSearch(s1);

    /*

    for(int i = 0; i < 100 ; i++){
        s2 = LocalSearch(s2);
    } */


    cout << s2;

	//Neighborhood n(s1);
}


Solution ILS::generateInitialSolution(){
    Solution s;
  	srand((unsigned)time(0));
 	for(int it = 0; it < InstanceSet.size(); it++) {
        int random_teacher;
        int random_room;
        int random_hour;
        int instanceID = InstanceSet[it].getID();
        Instance currentInstance = InstanceSet[it];

        bool allocated = false;
        Teacher currentTeacher;
        //garantir que o professor pode lecionar a disciplina
        do{
            random_teacher = rand() % this->TeacherSet.size() + 1;
            for(int i = 0; i < TeacherSet.size(); i++){
                if(random_teacher == TeacherSet[i].getID()){
                    currentTeacher = TeacherSet[i];
                    break;
                }
            }

            if(restricao5(currentTeacher,currentInstance) == true){
                s.addInstanceInTeacher(random_teacher,instanceID);
                allocated = true;
            }

        }while(allocated == false);

        //garantir o horário de preferencia do professor
        allocated = false;
        do{
            random_hour = rand() % 39;
            currentTeacher;

            if(restricao3(random_hour,currentTeacher,currentInstance) == true){
                s.addInstanceInHour(random_hour,instanceID);
                allocated = true;
            }
        }while(allocated == false);

        //garantir sala é capaz de comportar a aula
        allocated = false;
        Room currentRoom;
        do{
            random_room = rand() % this->RoomSet.size() + 1;

            for(int i =0; i < this->RoomSet.size() ; i++){
                if(random_room == this->RoomSet[i].getID()){
                    currentRoom = this->RoomSet[i];
                    break;
                }
            }

            if(restricao7(currentRoom,currentInstance) == true){
                s.addInstanceInRoom(random_room,instanceID);
                allocated = true;
            }

        }while(allocated == false);

 	}

    s = restricao8(s);

    objectiveFunction(s);

 	return s;
}


Solution ILS::LocalSearch(Solution s){
    int contador = 0;
    //Mudar o horário de todas as aulas
    Solution result = s;

    for(int i = 0; i < s.hour.size() ; i++){
        for(int j =0; j < s.teacher.size();j++){
            if(s.hour[i].second == s.teacher[j].second){
                for(int k =0; k < TeacherSet.size(); k++){
                    if(s.teacher[j].first == TeacherSet[k].getID() ){
                        Teacher currentTeacher = TeacherSet[k];
                        for(int l = 0; l < InstanceSet.size(); l++){
                            if(InstanceSet[l].getID() == s.hour[i].second){
                                bool done = false;
                                do{
                                    int random_hour = rand() % 39;
                                    if(restricao3(random_hour,currentTeacher,InstanceSet[l]) == true
                                        && random_hour != result.hour[j].first ){

                                        result.hour[j].first = random_hour;
                                        done = true;
                                    }
                                }while(done == false);
                            }
                        }
                    }
                }
            }
        }
    }

    objectiveFunction(result);
    return AcceptanceCriterion(s,result);
}

void ILS::Perturbation(){}


Solution ILS::AcceptanceCriterion(Solution s1, Solution s2){
        if(s1.objective > s2.objective){
             cout << "Busca local nao melhorou. Melhor solucao:" ;
             cout << s1.objective << endl ;
            return s1;
        }

        else{
            cout << "Busca local melhorou. Melhor solucao:";
            cout <<  s2.objective << endl;
            return s2;
        }
}

void ILS::objectiveFunction(Solution& s){
    //Atribuição da função objetivo
    s.objective = 0;

    for(int i = 0; i < s.hour.size() ; i++){
        for(int j =0; j < s.teacher.size();j++){
            if(s.hour[i].second == s.teacher[j].second){
                for(int k =0; k < TeacherSet.size(); k++){
                    if(s.teacher[j].first == TeacherSet[k].getID() ){
                        Teacher currentTeacher = TeacherSet[k];
                        for(int l = 0; l < TeacherPreferenceSet.size(); l++){
                            if( currentTeacher.getTeacherName() == TeacherPreferenceSet[l].getTeacherName() ){
                                for(int m=0; m < InstanceSet.size(); m++){
                                    if(s.hour[i].second == InstanceSet[m].getID() &&
                                        InstanceSet[m].getCurriculum() == TeacherPreferenceSet[l].getCurriculum() &&
                                        InstanceSet[m].getDiscipline() == TeacherPreferenceSet[l].getDiscipline()
                                        ){

                                        vector<int> primaryHours =  TeacherPreferenceSet[l].getMainHours();
                                        for(int n = 0; n < primaryHours.size(); n++){
                                            if(primaryHours[n] == s.hour[i].first){
                                                s.objective += TeacherPreferenceSet[l].getMainWeight();
                                                break;
                                            }
                                        }

                                        vector<int> secundaryHours =  TeacherPreferenceSet[l].getSecundaryHours();
                                        for(int n = 0; n < secundaryHours.size(); n++){
                                            if(secundaryHours[n] == s.hour[i].first){
                                                s.objective += TeacherPreferenceSet[l].getSecundaryWeight();
                                                break;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

// Restricao 2: de carga horária mínima
bool ILS::restricao2(Teacher currentTeacher){

    bool result = false;
    int sum = 0;
    for(int j = 0; j < TeacherPreferenceSet.size() ; j++){
            for(int k =0; k < RequirementSet.size() ; k++){
                if(currentTeacher.getTeacherName() == TeacherPreferenceSet[j].getTeacherName() &&
                    TeacherPreferenceSet[j].getDiscipline() == RequirementSet[k].getDiscipline() &&
                    TeacherPreferenceSet[j].getCurriculum() == RequirementSet[k].getCurriculum()

                    ){
                      sum += RequirementSet[k].getRepetitions()*2;
                }
            }
    }
    if(sum <= 24){
        result = true;
    }
}

// Restrição 3: Preferencia do Professor
bool ILS::restricao3(int hour,Teacher currentTeacher, Instance it){
    bool result = false;

    for(int j = 0; j < this->TeacherPreferenceSet.size() ; j++ ){
        if(currentTeacher.getTeacherName() == this->TeacherPreferenceSet[j].getTeacherName() &&
            it.getCurriculum() == this->TeacherPreferenceSet[j].getCurriculum() &&
            it.getDiscipline() == this->TeacherPreferenceSet[j].getDiscipline() ){

                auto vector1 = this->TeacherPreferenceSet[j].getMainHours();
                auto vector2 = this->TeacherPreferenceSet[j].getSecundaryHours();
                vector1.insert( vector1.end(), vector2.begin(), vector2.end() );

                auto possibleHours = vector1;

                for(int k = 0; k < possibleHours.size(); k++){
                    if(hour == possibleHours[k]){
                        result = true;
                        break;
                    }
                }
        }
    }

    return result;
}

// Restrição 5: Professor possivel
bool ILS::restricao5(Teacher currentTeacher, Instance currentInstance){
    bool result = false;

    for(int j = 0; j < this->TeacherPreferenceSet.size() ; j++ ){
        if(currentTeacher.getTeacherName() == this->TeacherPreferenceSet[j].getTeacherName()){
            if(currentInstance.getCurriculum() == this->TeacherPreferenceSet[j].getCurriculum() &&
                currentInstance.getDiscipline() == this->TeacherPreferenceSet[j].getDiscipline() ){
                result = true;
                break;
            }
        }
    }
    return result;
}

// Restrição 7 : Capacidade da sala
bool ILS::restricao7(Room r, Instance currentInstance){
    bool result = false;
    if(r.getRoomCapacity() >= currentInstance.getClassCapacity() ){
        result = true;
    }
    return result;
}

//Restrição 8: estabilidade de salas
Solution ILS::restricao8(Solution s){

    for(int i =0; i < InstanceSet.size(); i++){
        for(int j =0; j < InstanceSet.size(); j++){
            if(i != j){
                Instance inst1 = InstanceSet[i];
                Instance inst2 = InstanceSet[j];
                if(inst1.getDiscipline() == inst2.getDiscipline() &&
                    inst1.getCurriculum() == inst2.getCurriculum() ){
                    int room1 = -1;
                    int room2 = -1;
                    for(int k = 0; k < s.room.size(); k++){
                        if(s.room[k].second == inst1.getID()){
                           room1 = s.room[k].first;
                        }
                        else if(s.room[k].second == inst2.getID()){
                           room2 = s.room[k].first;
                        }
                    }
                    if(room1 != room2){
                        s.moveInstanceToRoom(inst1.getID(),room1);
                        s.moveInstanceToRoom(inst2.getID(),room1);
                    }
                }
            }
        }
    }

   return s;

}
