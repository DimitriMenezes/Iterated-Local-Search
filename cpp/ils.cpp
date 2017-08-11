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
    cout << "Solucao apos busca local" ;
    cout <<  localOptimum.objective << endl;
    cout << "-------------------------------------------" << endl;

    for(int i = 0; i < 50 ; i++){
		localOptimum = Perturbation2(localOptimum);
        localOptimum = LocalSearch(localOptimum);
        cout << "Solucao apos busca local" ;
        cout <<  localOptimum.objective << endl;
        cout << "-------------------------------------------" << endl;
    }

    printSolution(localOptimum);
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

            if (  constraint2(currentTeacher) == true
                  && constraint5(currentTeacher,currentInstance) == true
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
                     constraint4(random_hour,random_teacher,s) == true
                  && constraint6(random_room, random_hour, s) == true
                  && constraint7(currentRoom,currentInstance) == true
                  //&& constraint8(random_room, currentInstance, s) == true
                  && ( constraint3(random_hour,currentTeacher,currentInstance) == true
                  || constraint10(random_hour, currentInstance) == true )
                  && constraint9(random_hour,s,currentInstance) == true

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

    construir historico de conflitos                       

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
                if(constraint3(instance_hour,currentTeacher, currentInstance) != true){

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
                                    && constraint4(new_hour,teacher_id,localSolution) == true
                                    && constraint6(room_id, new_hour, localSolution) == true
                                    && constraint7(currentRoom,currentInstance) == true
                                    && constraint8(room_id, currentInstance, localSolution) == true
                                    && constraint10(new_hour, currentInstance) == true
                                    && constraint9(new_hour,localSolution,currentInstance) == true
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
                                        && constraint4(new_hour,teacher_id,localSolution) == true
                                        && constraint6(room_id, new_hour, localSolution) == true
                                        && constraint7(currentRoom,currentInstance) == true
                                        && constraint8(room_id, currentInstance, localSolution) == true
                                        && constraint10(new_hour, currentInstance) == true
                                        && constraint9(new_hour,localSolution,currentInstance) == true
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
                            if( constraint4(new_hour,teacher_id,localSolution) == true
                                && constraint6(room_id, new_hour, localSolution) == true
                                && constraint7(currentRoom,currentInstance) == true
                                && constraint8(room_id, currentInstance, localSolution) == true
                                && constraint10(new_hour, currentInstance) == true
                                && constraint9(new_hour,localSolution,currentInstance) == true){
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

    trocar salas das duas aulas ? TESTAR 

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

            if(flag1 == true && flag2 == true ){
                break;
            }
        }

        bool allocated = false;
        do{
            int firstRoom = rand() % this->RoomSet.size() + 1;
            int secondRoom = rand() % this->RoomSet.size() + 1;

            Room currentRoom1;
            Room currentRoom2;

            for(int x =0; x < this->RoomSet.size() ; x++){
                if(firstRoom == this->RoomSet[x].getID()){
                    currentRoom1 = this->RoomSet[x];
                    break;                   
                }
            }

            for(int x =0; x < this->RoomSet.size() ; x++){
                if(secondRoom == this->RoomSet[x].getID()){
                    currentRoom2 = this->RoomSet[x];
                    break;                   
                }
            }



            if(constraint6(firstRoom, secondHour, localSolution) == true &&
               constraint7(currentRoom1,firstInstance) == true &&
               constraint6(secondRoom, firstHour, localSolution) == true &&
               constraint7(currentRoom2,secondInstance) == true
               ){
                localSolution.moveInstanceToHour(firstInstance.getID(),secondHour);
                localSolution.moveInstanceToHour(secondInstance.getID(),firstHour);
                localSolution.moveInstanceToRoom(firstInstance.getID(),secondRoom);
                localSolution.moveInstanceToRoom(secondInstance.getID(),firstRoom);
                allocated = true;
            }

        }while(allocated == false);

    }

    objectiveFunction(localSolution);
    cout << "solucao apos perturbacao: " << localSolution.objective << endl;

    return localSolution;
}

/*
funcao simpleMove(){
    para toda aula conflitante
        selecionar uma sala randomicamente;

        se aula pode ser alocada nessa sala
            mover aula para essa aula

retornar solucao
}
*/

/*ALTERAR A SALA DAS AULAS QUE DERAM CONFLITOS */
Solution ILS::Perturbation2(Solution initialSolution){

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
                   constraint6(random_room, random_hour, localSolution) == true  //random_hour
                   && constraint7(currentRoom,currentInstance) == true
                   && constraint10(random_hour, currentInstance) == true
        		){
                    //mover ela para outra sala
                    localSolution.moveInstanceToRoom(currentInstance.getID() , random_room );
                    localSolution.moveInstanceToHour(currentInstance.getID(),  random_hour);
        		    allocated = true;
        	}

        }while(allocated == false);

	}
    cout << "solucao apos perturbacao: " << localSolution.objective << endl;
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

// constraint 2: de carga horária mínima
bool ILS::constraint2(Teacher currentTeacher){

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
bool ILS::constraint3(int hour,Teacher currentTeacher, Instance it){
    bool result = false;

    for(int j = 0; j < this->TeacherPreferenceSet.size() ; j++ ){
        if(currentTeacher.getTeacherName() == this->TeacherPreferenceSet[j].getTeacherName() &&
            it.getCurriculum() == this->TeacherPreferenceSet[j].getCurriculum() &&
            it.getDiscipline() == this->TeacherPreferenceSet[j].getDiscipline() ){

                auto vector1 = this->TeacherPreferenceSet[j].getMainHours();
                auto vector2 = this->TeacherPreferenceSet[j].getSecundaryHours();
                vector1.insert( vector1.end(), vector2.begin(), vector2.end() ); //merge dos 2 vetores

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
bool ILS::constraint4(int hour, int teacher , Solution s){
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
bool ILS::constraint5(Teacher currentTeacher, Instance currentInstance){
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
bool ILS::constraint6(int room, int hour , Solution s){
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

    //significa que pode alocar nesta sala
    if(counter == 0)
        return true;
    else
        return false;
}

// Restrição 7 : Adequacao e capacidade da sala
bool ILS::constraint7(Room r, Instance currentInstance){
    bool result = false;
    if(r.getRoomCapacity() >= currentInstance.getClassCapacity() ){
        result = true;
    }
    return result;
}

//Restrição 8: estabilidade de salas
bool ILS::constraint8(int random_room, Instance it, Solution s){
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

    return result;
}

//constraint 9: garantir uma unica aula de curriculum por horario
bool ILS::constraint9(int random_hour, Solution s, Instance it){
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
bool ILS::constraint10(int hour , Instance it){

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

/*Metodo para imprimir as grades de horarios*/
void ILS::printSolution(Solution s){

    for(int i =0; i < CurriculumSet.size() ; i++){

        Curriculum currentCurriculum = CurriculumSet[i];

        cout << "Curriculum " << currentCurriculum.getCurriculum() << endl;

        int day = 0;

        for(int j = 0; j < 40; j++){

            if(j % 8 == 0) {
                day++;
                cout << " "<< endl;
                if(day == 1){
                    cout << "SEG"<< endl;
                }
                else if(day == 2){
                    cout << "TER"<< endl;
                }
                else if(day == 3){
                    cout << "QUA"<< endl;
                }
                else if(day == 4){
                    cout << "QUI"<< endl;
                }
                else if(day == 5){
                    cout << "SEX"<< endl;
                }
            }

            if(j % 8 == 3)
                cout << "Almoço"<< endl;
            if(j % 8 == 6)
                cout << "Janta"<< endl;

            bool activity = false;
            Instance currentInstance;
            Teacher currentTeacher;
            Room currentRoom;
            for(int l = 0; l < s.hour.size(); l++){
                for(int m = 0; m < InstanceSet.size(); m++){
                    if(InstanceSet[m].getCurriculum() == currentCurriculum.getCurriculum() &&
                       InstanceSet[m].getID() ==  s.hour[l].second &&
                       s.hour[l].first == j
                    ){
                        activity = true;
                        currentInstance = InstanceSet[m];
                        for(int n = 0; n < s.teacher.size(); n++){
                            if(s.teacher[n].second == currentInstance.getID()){
                                for(int o = 0; 0 < TeacherSet.size() ; o++){
                                    if(TeacherSet[o].getID() == s.teacher[n].first){
                                        currentTeacher = TeacherSet[o];
                                        break;
                                    }
                                }
                            }
                        }

                        for(int n =0; n < s.room.size(); n++){
                            if(s.room[n].second == currentInstance.getID()){
                                for(int o =0; o < RoomSet.size(); o++){
                                    if(RoomSet[o].getID() == s.room[n].first){
                                        currentRoom = RoomSet[o];
                                        break;
                                    }
                                }
                            }
                        }

                        break;
                    }
                }
            }

            if(j % 8+1 == 1){
                cout << "07-09" << "\t" ;
            }
            else if(j % 8+1 == 2){
                cout << "09-11" << "\t" ;
            }
            else if(j % 8+1 == 3){
                cout << "11-13" << "\t" ;
            }
            else if(j % 8+1 == 4){
                cout << "13-15" << "\t" ;
            }
            else if(j % 8+1 == 5){
               cout << "15-17" << "\t" ;
            }
            else if(j % 8+1 == 6){
                cout << "17-19" << "\t" ;
            }
            else if(j % 8+1 == 7){
                cout << "19-21" << "\t" ;
            }
            else if(j % 8+1 == 8){
                cout << "21-23" << "\t" ;
            }

            if(activity == true)
                cout << currentRoom.getRoomName() << "\t"
                << currentInstance.getDiscipline() << "\t" <<
                currentTeacher.getTeacherName() << endl;
            else
                cout << "Free time" << endl;
        }
    }
}