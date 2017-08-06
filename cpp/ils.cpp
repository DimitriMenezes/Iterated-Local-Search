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

	Solution initialSolution = generateInitialSolution();
    cout << "Solucao inicial: " ;
    cout << initialSolution.objective << endl;

    Solution localOptimum = LocalSearch(initialSolution);
    cout << "-------------------------------------------" << endl;

    for(int i = 0; i < 20 ; i++){
		localOptimum = Perturbation(localOptimum);
        localOptimum = LocalSearch(localOptimum);
        cout << "Solucao corrente: " ;
        cout <<  localOptimum.objective << endl;
        cout << "-------------------------------------------" << endl;
    }

    cout << localOptimum;
    
}


/*
procedimento gerarSolucaoInicial(){

Solucao s
para toda aula faça

    boolean aula_alocada = falso
    faca
        selecionar professor randomicamente

        se nao houver conflitos
            alocar aula para professor
            aula_alocada = verdade

    enquanto (aula_alocada == falso)

    aula_alocada = falso
    faça
        selecionar horario randomicamente
        selecionar sala randomicamente

        se nao houver conflitos
            alocar aula em sala e horario
            aula_alocada = verdade

    enquanto (aula_alocada == falso);

    calcular funcao objetivo de s


retornar s
}

*/

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
        //FASE 1: Alocar aula para professor

        do{
            random_teacher = rand() % this->TeacherSet.size() + 1;
            for(int i = 0; i < TeacherSet.size(); i++){
                if(random_teacher == TeacherSet[i].getID()){
                    currentTeacher = TeacherSet[i];
                    break;
                }
            }

            if (   restricao2(currentTeacher) == true
                && restricao5(currentTeacher,currentInstance) == true
                ) {
                s.addInstanceInTeacher(random_teacher,instanceID);
                allocated = true;
            }

        }while(allocated == false);


        allocated = false;
        //FASE 2: Alocar aula em sala e horario

        do{
            random_hour = rand() % 39;
            random_room = rand() % this->RoomSet.size() + 1;

            for(int i =0; i < this->RoomSet.size() ; i++){
                if(random_room == this->RoomSet[i].getID()){
                    currentRoom = this->RoomSet[i];
                    break;
                }
            }

            if(
                     restricao4(random_hour,random_teacher,s) == true
                  && restricao6(random_room, random_hour, s) == true
                  && restricao7(currentRoom,currentInstance) == true
                  && restricao8(random_room, currentInstance, s) == true
                  && ( restricao3(random_hour,currentTeacher,currentInstance) == true
                  || restricao10(random_hour, currentInstance) == true )
                  && restricao9(random_hour,s,currentInstance) == true

                ){
                    s.addInstanceInHour(random_hour,instanceID);
                    s.addInstanceInRoom(random_room,instanceID);
                    allocated = true;
                    cout << "alocado " << currentInstance << " horario :" << random_hour << " sala: " << random_room << endl ;
            }
        }while(allocated == false);
    }

    objectiveFunction(s);

    return s;
}


/*procedimento busca_local(){

    para todo curriculum faça
        para toda aula faça
            se aula for desse curriculum entao

                se aula foi alocada em horario sem preferencia do professor entao

                    selecionar um horario de maior preferencia do professor para aula
                    se aula pode ser alocada nesse horario entao
                        mover ela para esse horario
                        calcular funçao objetivo da solucao atual
                    se nao
                        selecionar um horario de menor preferencia do professor para aula
                        se aula pode ser alocada nesse horario entao
                            mover ela para esse horario
                            calcular funçao objetivo da solucao atual


                se nao se aula foi alocada em horario de menor preferencia do professor
                    selecionar um horario de maior preferencia do professor para aula
                    se aula pode ser alocada nesse horario entao
                        mover ela para esse horario
                        calcular funçao objetivo da solucao atual

	se a solucao melhorou
		adotar como solucao candidata
}
*/


Solution ILS::LocalSearch(Solution initialSolution){
	vector<int> v;
	historic = v;

    //Mudar o horário de todas as aulas
    Solution localSolution = initialSolution;

    for(int i = 0; i < CurriculumSet.size() ; i++){
        Curriculum currentCurriculum = CurriculumSet[i];
        for(int j=0; j < InstanceSet.size(); j++){
            Instance currentInstance = InstanceSet[j];
            bool allocated = false;

            if(currentCurriculum.getCurriculum() == currentInstance.getCurriculum() ){

                int instance_hour;
                for(int k = 0; k < localSolution.hour.size() ; k++){
                    if( localSolution.hour[k].second == currentInstance.getID() ){
                        instance_hour = localSolution.hour[k].first;
                        break;
                    }
                }

                Teacher currentTeacher;
                int teacher_id;
                for(int m =0; m < TeacherSet.size(); m++){
                    for(int l =0; l < localSolution.teacher.size();l++){
                        if(localSolution.teacher[l].first == TeacherSet[m].getID() &&
                           localSolution.teacher[l].second == currentInstance.getID()
                         ){
                            currentTeacher = TeacherSet[m];
                                break;
                        }
                    }
                }

                Room currentRoom;
                int room_id;

                for(int m=0; m < RoomSet.size(); m++){
                    for(int n=0; n < localSolution.room.size(); n++){
                        if( localSolution.room[n].second == currentInstance.getID()
                            && localSolution.room[n].first == RoomSet[m].getID()
                        ){
                            currentRoom = RoomSet[m];
                            room_id = localSolution.room[n].first;
                            break;
                        }
                    }
                }

                //buscar a aula alocada sem a preferencia do professor
                if(restricao3(instance_hour,currentTeacher, currentInstance) != true){

                    for(int n = 0; n < TeacherPreferenceSet.size(); n++){
                        if( currentTeacher.getTeacherName() == TeacherPreferenceSet[n].getTeacherName() &&
                            currentCurriculum.getCurriculum() == TeacherPreferenceSet[n].getCurriculum() &&
                            currentInstance.getDiscipline() == TeacherPreferenceSet[n].getDiscipline()
                            ){
                            auto vector1 = this->TeacherPreferenceSet[n].getMainHours();
                            auto vector2 = this->TeacherPreferenceSet[n].getSecundaryHours();

                            for(int n=0; n < vector1.size() ; n++){
                                int new_hour = vector1[n];

                                if( new_hour != instance_hour
                                    && restricao4(new_hour,teacher_id,localSolution) == true
                                    && restricao6(room_id, new_hour, localSolution) == true
                                    && restricao7(currentRoom,currentInstance) == true
                                    && restricao8(room_id, currentInstance, localSolution) == true
                                    && restricao10(new_hour, currentInstance) == true
                                    && restricao9(new_hour,localSolution,currentInstance) == true
                                ){
                                    localSolution.moveInstanceToHour(currentInstance.getID(),new_hour);
                                    objectiveFunction(localSolution);
                                    allocated = true;
                                     break;
                                }
                            }

                            if(allocated == false){
                                for(int n=0; n < vector2.size() ; n++){
                                    int new_hour = vector2[n];

                                    if( new_hour != instance_hour
                                        && restricao4(new_hour,teacher_id,localSolution) == true
                                        && restricao6(room_id, new_hour, localSolution) == true
                                        && restricao7(currentRoom,currentInstance) == true
                                        && restricao8(room_id, currentInstance, localSolution) == true
                                        && restricao10(new_hour, currentInstance) == true
                                        && restricao9(new_hour,localSolution,currentInstance) == true
                                    ){
                                        localSolution.moveInstanceToHour(currentInstance.getID(),new_hour);
                                        objectiveFunction(localSolution);
                                        allocated = true;
                                        break;

                                    }
                                }

                                //inserir aula com conflito em historico
                                if(allocated == false){
                                    // mudar a sala
                                    //cout << "instancia com problema " << currentInstance.getID()  << endl;
                                    historic.push_back(currentInstance.getID());
                                }
                            }
                        }
                    }
                }
                //buscar a aula alocada com a menor preferencia do professor
                else{
                    vector<int> vector1;
                    vector<int> vector2;

                    for(int n = 0; n < TeacherPreferenceSet.size(); n++){
                        if( currentTeacher.getTeacherName() == TeacherPreferenceSet[n].getTeacherName() &&
                            currentCurriculum.getCurriculum() == TeacherPreferenceSet[n].getCurriculum() &&
                            currentInstance.getDiscipline() == TeacherPreferenceSet[n].getDiscipline()
                            ){
                            vector1 = this->TeacherPreferenceSet[n].getMainHours();
                            vector2 = this->TeacherPreferenceSet[n].getSecundaryHours();
                            break;
                        }
                    }

                    bool should_change = false;

                    for(int n = 0; n < vector2.size(); n++){
                        if(instance_hour == vector2[n]){ //significa que a aula foi alocada em menor preferencia
                            should_change = true;
                            break;

                        }
                    }
                    bool allocated = false;
                    if(should_change == true){
                        for(int o = 0 ; o < vector1.size() ; o++ ){
                            int new_hour = vector1[o];
                            if( restricao4(new_hour,teacher_id,localSolution) == true
                                && restricao6(room_id, new_hour, localSolution) == true
                                && restricao7(currentRoom,currentInstance) == true
                                && restricao8(room_id, currentInstance, localSolution) == true
                                && restricao10(new_hour, currentInstance) == true
                                && restricao9(new_hour,localSolution,currentInstance) == true){
                                    localSolution.moveInstanceToHour(currentInstance.getID(),new_hour);
                                    objectiveFunction(localSolution);
                                    break;
                                    allocated = true;
                            }
                        }
                         //inserir aula com conflito em historico
                        if(allocated == false){
                            //cout << "instancia com problema " << currentInstance.getID()  << endl;
                            historic.push_back(currentInstance.getID());
                        }
                    }
                }
            }
        }
    }
	cout << "Quantidade de elementos a serem perturbados" << historic.size() << endl ;
    return AcceptanceCriteria(initialSolution,localSolution);
}


/*
procedimento swap()

    selecionar uma aula conflitante
    selecionar uma aula qualquer de mesmo curriculo

    trocar horarios das duas aulas

    retornar solucao
*/


Solution ILS::Perturbation1(Solution initialSolution){
    Solution localSolution = initialSolution;

    //obter aula com conflito
    for(int i = 0; i < historic.size() ; i++){
        Instance firstInstance;
        for(int j = 0; j < InstanceSet.size(); j++){
            if( historic[i] == InstanceSet[j].getID() ){
                firstInstance = InstanceSet[j];
                break;
            }
        }

        Instance secondInstance;
        for(int j = 0; j < InstanceSet.size(); j++){
            if( firstInstance.getID() != InstanceSet[j].getID() &&
                firstInstance.getCurriculum() == InstanceSet[j].getCurriculum()
             ){
                secondInstance = InstanceSet[j];
                break;
            }
        }

        bool flag1 =false;
        bool flag2 =false;
        int firstHour;
        int secondHour;
        for(int k =0; k < localSolution.hour.size(); k++ ){
            if(localSolution.hour[k].second == firstInstance.getID() ){
                firstHour = localSolution.hour[k].first;
                flag1 = true;
            }

            if(localSolution.hour[k].second == secondInstance.getID() ){
                secondHour = localSolution.hour[k].first;
                flag2 =true;
            }

            //SWAP HORARIOS
            if(flag1 == true && flag2 == true ){
                //cout << firstInstance.getID() <<" swap com "<< secondInstance.getID() << endl;
                localSolution.moveInstanceToHour(firstInstance.getID(),secondHour);
                localSolution.moveInstanceToHour(secondInstance.getID(),firstHour);
                break;
            }
        }
    }

    objectiveFunction(localSolution);
    cout << "resultado da perturbacao: " << localSolution.objective << endl;

    return localSolution;
}

/*
funcao simpleMove(){
    para toda aula conflitante
        selecionar uma sala randomicamente;

        se aula pode se alocada nessa sala
            mover aula para essa aula

retornar solucao
}
*/

/*ALTERAR A SALA DAS AULAS QUE DERAM CONFLITOS */
Solution ILS::Perturbation(Solution initialSolution){

    Solution localSolution = initialSolution;

	for(int i = 0; i < historic.size() ; i++){
        Instance currentInstance;
		for(int j = 0; j < InstanceSet.size(); j++){
            if( historic[i] == InstanceSet[j].getID() ){
				currentInstance = InstanceSet[j];
                break;
            }
        }

        int hour_id;
		for(int k =0; k < localSolution.hour.size(); k++ ){
            if(localSolution.hour[k].second == currentInstance.getID() ){
                hour_id = localSolution.hour[k].first;
                break;
            }
        }

    	bool allocated = false;

        do{
        	int random_room = rand() % this->RoomSet.size() + 1;
            int random_hour = rand() % 39;

        	Room currentRoom;

        	for(int l =0; l < this->RoomSet.size() ; l++){
        		if(random_room == this->RoomSet[l].getID()){
        			currentRoom = this->RoomSet[l];
        			break;
        		}
        	}

            if(
                   restricao6(random_room, random_hour, localSolution) == true  //random_hour
                   && restricao7(currentRoom,currentInstance) == true
                   && restricao10(random_hour, currentInstance) == true
        		){
                    //mover ela para outra sala
                    localSolution.moveInstanceToRoom(currentInstance.getID() , random_room );
                    localSolution.moveInstanceToHour(currentInstance.getID(),  random_hour);
        		    allocated = true;
        	}

        }while(allocated == false);

	}

    objectiveFunction(localSolution);
    return localSolution;
}

Solution ILS::AcceptanceCriteria(Solution s1, Solution s2){
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


void ILS::objectiveFunction(Solution& s){ //passagem por referencia
    //Atribuição da função objetivo
    s.objective = 0;

    for(int i = 0; i < s.hour.size() ; i++){
        for(int j =0; j < s.teacher.size();j++){
            if(s.hour[i].second == s.teacher[j].second){

                Teacher currentTeacher;
                for(int k =0; k < TeacherSet.size(); k++){
                    if(s.teacher[j].first == TeacherSet[k].getID() ){
                        currentTeacher = TeacherSet[k];
                    }
                }

                for(int l = 0; l < TeacherPreferenceSet.size(); l++){
                    if( currentTeacher.getTeacherName() == TeacherPreferenceSet[l].getTeacherName() ){
                        for(int m=0; m < InstanceSet.size(); m++){
                            if( s.hour[i].second == InstanceSet[m].getID() &&
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

// Restrição 6: Garantir uma unica aula em determinada sala
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

// Restrição 7 : Adequacao e capacidade da sala
bool ILS::restricao7(Room r, Instance currentInstance){
    bool result = false;
    if(r.getRoomCapacity() >= currentInstance.getClassCapacity() ){
        result = true;
    }
    return result;
}

//Restrição 8: estabilidade de salas
bool ILS::restricao8(int random_room, Instance it, Solution s){
    bool result = true;
    int counter = 0;

    Instance currentInstance;
    for(int i =0; i < InstanceSet.size(); i++){
        if(    it.getID() != InstanceSet[i].getID()
            && it.getCurriculum() == InstanceSet[i].getCurriculum()
            && it.getDiscipline() == InstanceSet[i].getDiscipline() ){
                currentInstance = InstanceSet[i];
                break;
        }
    }

    for(int i = 0; i < s.room.size(); i++){
        if(s.room[i].second == currentInstance.getID() &&
            s.room[i].first != random_room
        ){
           result = false;
           break;
        }

    }

/*
       for(int k =0; k < InstanceSet.size(); k++){
            Instance currentInstance = InstanceSet[k];

            if( s.room[i].second == currentInstance.getID() && it.getID() != currentInstance.getID()
                && it.getCurriculum() == currentInstance.getCurriculum()
                && it.getDiscipline() == currentInstance.getDiscipline()
             ){
                 counter++;
                if( it.getCurriculum() == currentInstance.getCurriculum()
                    && it.getDiscipline() == currentInstance.getDiscipline()
                    && s.room[i].first == random_room
                ){

                    result = true;
                    break;
                }
            }
       }*/
   // }
/*
    //significa que ainda nao foi alocado nenhuma aula daquela disciplina
    if(counter == 0){
        result = true;
    }*/

    return result;
}

//Restricao 9: garantir uma unica aula de curriculum por horario
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

//Restricoes 10, 11 e 12
//Garantir que os curriculus da manha serao alocadas na manha.
//Garantir que os curriculus da tarde serao alocadas na tarde.
//Garantir que os curriculus da noite serao alocadas na noite.
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
