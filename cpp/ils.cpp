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
    Solution s2 = LocalSearch(s1);

    cout << s2;


	//Neighborhood n(s1);
}


Solution ILS::generateInitialSolution(){
    Solution s;
  	srand((unsigned)time(0));
 	for(std::vector<Instance>::iterator it = this->InstanceSet.begin(); it != this->InstanceSet.end(); ++it) {
        int random_teacher;
        int random_room;
        int random_hour;
        int instanceID = it->getID();

        bool allocated = false;
        Teacher currentTeacher;
        //garantir que o professor pode lecionar a disciplina
        do{
            random_teacher = rand() % this->TeacherSet.size() + 1;

            for(int i = 0; i < this->TeacherSet.size(); i++){
                if(random_teacher == TeacherSet[i].getID()){
                    currentTeacher = TeacherSet[i];
                    break;
                }
            }

            for(int j = 0; j < this->TeacherPreferenceSet.size() ; j++ ){
                if(currentTeacher.getTeacherName() == this->TeacherPreferenceSet[j].getTeacherName()){
                    if(it->getCurriculum() == this->TeacherPreferenceSet[j].getCurriculum() &&
                           it->getDiscipline() == this->TeacherPreferenceSet[j].getDiscipline() ){
                                    //cout << random_teacher << "professor alocado em " << instanceID << endl;
                                    allocated = true;
                                    s.addInstanceInTeacher(random_teacher,instanceID);
                                    break;
                    }
                }
            }
        }while(allocated == false);

        //garantir o horário de preferencia do professor
        allocated = false;
        do{
            random_hour = rand() % 39;
            currentTeacher;

           for(int j = 0; j < this->TeacherPreferenceSet.size() ; j++ ){
                if(currentTeacher.getTeacherName() == this->TeacherPreferenceSet[j].getTeacherName() &&
                    it->getCurriculum() == this->TeacherPreferenceSet[j].getCurriculum() &&
                    it->getDiscipline() == this->TeacherPreferenceSet[j].getDiscipline() ){

                    auto vector1 = this->TeacherPreferenceSet[j].getMainHours();
                    auto vector2 = this->TeacherPreferenceSet[j].getSecundaryHours();
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

        //garantir sala é capaz de comportar a aula
        allocated = false;
        Room r;
        do{
            random_room = rand() % this->RoomSet.size() + 1;

            for(int i =0; i < this->RoomSet.size() ; i++){
                if(random_room == this->RoomSet[i].getID()){
                    r = this->RoomSet[i];
                    break;
                }
            }

            if(r.getRoomCapacity() >= it->getClassCapacity() ){
                s.addInstanceInRoom(random_room,instanceID);
                allocated = true;
                break;
            }
        }while(allocated == false);

        //Atribuição da função objetivo
        /*
        for(int i = 0; i < this->TeacherSet.size(); i++){
            if(random_teacher == this->TeacherSet[i].getID()){
                for(int j = 0; j < this->TeacherPreferenceSet.size(); j++){
                    if(currentTeacher.getTeacherName() == this->TeacherPreferenceSet[j].getTeacherName() &&
                        it->getCurriculum() == this->TeacherPreferenceSet[j].getCurriculum() &&
                        it->getDiscipline() == this->TeacherPreferenceSet[j].getDiscipline() ){

                    	vector<int> primaryHours =  this->TeacherPreferenceSet[j].getMainHours();
                    	for(int k = 0; k < primaryHours.size(); k++){
                    		if(primaryHours[k] == random_hour){
                    			s.objective += this->TeacherPreferenceSet[j].getMainWeight();
                    			break;
                    		}
                    	}

                    	vector<int> secundaryHours =  this->TeacherPreferenceSet[j].getSecundaryHours();
                    	for(int k = 0; k < secundaryHours.size(); k++){
                    		if(secundaryHours[k] == random_hour){
	                    		s.objective += this->TeacherPreferenceSet[j].getSecundaryWeight();
	                    		break;
                    		}
                    	}
                    }
                }
            }
        
        }*/
 	}


    objectiveFunction(s);

 	return s;
}


Solution ILS::LocalSearch(Solution s){
    //Mudar o horário de todas as aulas

    for(int i = 0; i < InstanceSet.size(); i++){
       for(int j=0;j< s.hour.size();j++){
            if(InstanceSet[i].getID() == s.hour[j].second){
                int random_hour = rand() % 39;
                s.hour[j].first = random_hour;
            }
       }
    }

    objectiveFunction(s);
   
    return s;
}

void ILS::Perturbation(){}


Solution ILS::AcceptanceCriterion(Solution s1, Solution s2){
        if(s1.objective > s2.objective)
            return s1;
        else
            return s2;
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
