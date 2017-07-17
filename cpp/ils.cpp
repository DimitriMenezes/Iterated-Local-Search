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
    cout << "Solucao inicial: " ;
    cout << s1.objective << endl;

    Solution s2 = LocalSearch(s1);

    
    for(int i = 0; i < 10 ; i++){
        s2 = LocalSearch(s2);
    } 


    //cout << s1;
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
        Teacher currentTeacher;
        Room currentRoom;

        bool allocated = false;

        do{
            random_teacher = rand() % this->TeacherSet.size() + 1;
            random_hour = rand() % 39;
            random_room = rand() % this->RoomSet.size() + 1;

            for(int i = 0; i < TeacherSet.size(); i++){
                if(random_teacher == TeacherSet[i].getID()){
                    currentTeacher = TeacherSet[i];
                    break;
                }
            }

            for(int i =0; i < this->RoomSet.size() ; i++){
                if(random_room == this->RoomSet[i].getID()){
                    currentRoom = this->RoomSet[i];
                    break;
                }
            }

            if(
                     restricao4(random_hour,random_teacher,s) == true
                  && restricao5(currentTeacher,currentInstance) == true
                  && restricao6(random_room, random_hour, s) == true
                  && restricao7(currentRoom,currentInstance) == true
                  && ( restricao3(random_hour,currentTeacher,currentInstance) == true
                  || restricao10(random_hour, currentInstance) == true )

                  && restricao9(random_hour,s,currentInstance) == true
                ){
                    s.addInstanceInTeacher(random_teacher,instanceID);
                    s.addInstanceInHour(random_hour,instanceID);
                    s.addInstanceInRoom(random_room,instanceID);
                    allocated = true;
                    //cout << "alocado " << InstanceSet[it] << " horario :" << random_hour  << endl ;
            }
        }while(allocated == false);
    }

    s = restricao8(s);

    objectiveFunction(s);

    return s;
}


/*
    para todo curriculum faça
        para toda aula faça
            se aula for desse curriculum entao
                se essa aula foi alocada em horario sem preferencia do professor entao
                    selecionar um horario de preferencia do professor para aula
                    se aula pode ser alocada nesse horario entao
                        mover ela para esse horario
                        calcular funçao objetivo
                        inserir a solucao local na vizinhança de solucoes
*/
Solution ILS::LocalSearch(Solution initialSolution){
    int contador = 0;
    //Mudar o horário de todas as aulas
    Solution localSolution = initialSolution;

    for(int i = 0; i < CurriculumSet.size() ; i++){
        Curriculum currentCurriculum = CurriculumSet[i];
        for(int j=0; j < InstanceSet.size(); j++){
            Instance currentInstance = InstanceSet[j];
            if(currentCurriculum.getCurriculum() == currentInstance.getCurriculum() ){
                for(int k = 0; k < localSolution.hour.size() ; k++){
                    for(int l =0; l < localSolution.teacher.size();l++){
                        if(localSolution.hour[k].second == localSolution.teacher[l].second
                            && localSolution.hour[k].second == currentInstance.getID()
                            ){
                            Teacher currentTeacher;

                            for(int m =0; m < TeacherSet.size(); m++){
                                if(localSolution.teacher[l].first == TeacherSet[m].getID() ){
                                     currentTeacher = TeacherSet[m];
                                     break;
                                }
                            }

                            if(restricao3(localSolution.hour[k].first,currentTeacher, currentInstance) != true){

                                for(int n = 0; n < TeacherPreferenceSet.size(); n++){
                                    if(currentTeacher.getTeacherName() == TeacherPreferenceSet[n].getTeacherName() ){
                                        auto vector1 = this->TeacherPreferenceSet[n].getMainHours();
                                        auto vector2 = this->TeacherPreferenceSet[n].getSecundaryHours();

                                        for(int n=0; n < vector1.size() ; n++){
                                            if(vector1[n] != localSolution.hour[k].first

                                                && restricao4(vector1[n],localSolution.teacher[l].first,localSolution) == true
                                                && restricao10(vector1[n], currentInstance) == true
                                                && restricao9(vector1[n],localSolution,currentInstance) == true

                                                ){
/*
                                                cout << "Aula " << currentInstance.getID()
                                                << " movida do horario " << localSolution.hour[k].first
                                                << " para o horario " << vector1[n] << endl ;*/

                                                localSolution.hour[k].first = vector1[n];
                                                objectiveFunction(localSolution);                                              
                                                break;
                                            }
                                        }
                                    }
                                }
                                //cout << currentInstance.getID() << "Horario alocado nao foi o da preferencia" << endl;
                            }
                        }
                    }
                }
            }
        }
    }
   
    return AcceptanceCriterion(initialSolution,localSolution);
}

void ILS::Perturbation(Solution &initialSolution){}

Solution ILS::AcceptanceCriterion(Solution s1, Solution s2){
        if(s1.objective >= s2.objective){
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

//Restrição 4: garantir que o professor tenha uma ou nenhuma aula em um determinado horário
bool ILS::restricao4(int hour, int teacher , Solution s){
    auto hours = s.hour;
    auto teachers = s.teacher;

    int counter = 0;

    for(int j = 0; j < hours.size(); j++ ){
        for(int k = 0; k < teachers.size() ; k++){
            if(hours[j].second == teachers[k].second &&
                hours[j].first == hour &&
                teachers[k].first == teacher
                ){
                    counter++;
            }
        }
    }

    //significa que pode alocar neste horario
    if(counter == 0)
        return true;
    else
        return false;
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

bool ILS::restricao6(int room, int hour , Solution s){
    auto hours = s.hour;
    auto rooms = s.room;

    int counter = 0;

    for(int j = 0; j < hours.size(); j++ ){
        if(hours[j].first == hour){
            for(int k = 0; k < rooms.size() ; k++){
                if( rooms[k].first == room && rooms[k].second == hours[j].second){
                    //cout << "Sala: " << room << " HORA " << hour << " Instance " << hours[j].second << endl;
                    counter++;
                }
            }
        }
    }

    //cout << counter << endl ;

    //significa que pode alocar nesta sala
    if(counter == 0)
        return true;
    else
        return false;
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

bool ILS::restricao9(int random_hour, Solution s, Instance it){
    int counter = 0;
    for(int i = 0; i < s.hour.size(); i++){
        if(s.hour[i].first == random_hour){
            for(int j = 0; j < InstanceSet.size(); j++){
                if(s.hour[i].second == InstanceSet[j].getID()
                   && InstanceSet[j].getCurriculum() == it.getCurriculum() ){
                    counter++;

                }
            }
        }
    }

    if(counter == 0) // nao existe aula naquele horario, entao pode alocar
        return true;
    else
        return false;
}

//Garantir que os curriculus da manha serao alocadas na manha.
bool ILS::restricao10(int hour , Instance it){

    bool result = false;

    Curriculum c;
    for(int i = 0; i < CurriculumSet.size(); i++){
        if(CurriculumSet[i].getCurriculum() == it.getCurriculum() ){
            c = CurriculumSet[i];
            break;
        }
    }


    if(c.getCurriculumType() == "MorningCurriculum" &&
            hour % 8 >= 0 &&
            hour % 8 <= 2
        ){      
            result = true;
        }
    else if(c.getCurriculumType() == "AfternoonCurriculum" &&
            hour % 8 >= 3 &&
            hour % 8 <= 5
            ){
                result = true;
            }
    else if(c.getCurriculumType() == "EveningCurriculum" &&
            hour % 8 >= 5 &&
            hour % 8 <= 7 ){              
                result = true;
            }
    return result;
}

//LIXEIRA

/*
Solution ILS::generateInitialSolution(){
    Solution s;
    srand((unsigned)time(0));
    for(int it = 0; it < InstanceSet.size(); it++) {

        int random_teacher;
        int random_room;
        int random_hour;
        int instanceID = InstanceSet[it].getID();

        Instance currentInstance = InstanceSet[it];
        Teacher currentTeacher;
        Room currentRoom;

        bool allocated = false;

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

            if( //restricao9(random_hour,s,currentInstance) == true
                //&&
                ( restricao4(random_hour,random_teacher,s) == true
                && restricao3(random_hour,currentTeacher,currentInstance) == true )

                ){
                cout << "alocado " << InstanceSet[it] << " horario :" << random_hour  << endl ;
                s.addInstanceInHour(random_hour,instanceID);
                allocated = true;
            }
        }while(allocated == false);

        //garantir sala é capaz de comportar a aula
        allocated = false;

        do{
            random_room = rand() % this->RoomSet.size() + 1;

            for(int i =0; i < this->RoomSet.size() ; i++){
                if(random_room == this->RoomSet[i].getID()){
                    currentRoom = this->RoomSet[i];
                    break;
                }
            }

            if(restricao7(currentRoom,currentInstance) == true
               && restricao6(random_room, random_hour, s) == true
                ){
                s.addInstanceInRoom(random_room,instanceID);
                allocated = true;
            }

        }while(allocated == false);

    }

    s = restricao8(s);

    objectiveFunction(s);

    return s;
}*/

/* LOCAL SEARCH
    for(int i = 0; i < localSolution.hour.size() ; i++){
        for(int j =0; j < localSolution.teacher.size();j++){
            if(localSolution.hour[i].second == localSolution.teacher[j].second){
                for(int k =0; k < TeacherSet.size(); k++){
                    if(localSolution.teacher[j].first == TeacherSet[k].getID() ){
                        Teacher currentTeacher = TeacherSet[k];
                        for(int l = 0; l < InstanceSet.size(); l++){
                            if(InstanceSet[l].getID() == localSolution.hour[i].second){
                                bool done = false;
                                do{
                                    int random_hour = rand() % 39;
                                    cout << "hour" << random_hour << endl;
                                    if( restricao3(localSolution.hour[i].first,currentTeacher,InstanceSet[l]) == true
                                        && restricao4(localSolution.hour[i].first,localSolution.teacher[j].first,localSolution) == true
                                        && restricao5(currentTeacher,InstanceSet[l]) == true
                                        && random_hour != localSolution.hour[j].first ){

                                        localSolution.hour[j].first = random_hour;
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
*/
