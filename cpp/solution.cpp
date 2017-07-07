#include "../hpp/solution.hpp"

Solution::Solution(){
    this->objective = 0;
    this->isCandidateSolution = false;
}

void Solution::addInstanceInTeacher(int currentTeacher, int instance){
	std::pair <int,int> x = std::make_pair (currentTeacher,instance);
	this->teacher.push_back(x);
}

void Solution::addInstanceInHour(int currentHour, int instance){
	std::pair <int,int> x = std::make_pair (currentHour,instance);
	this->hour.push_back(x);
}

void Solution::addInstanceInRoom(int currentRoom , int instance){
	std::pair <int,int> x = std::make_pair (currentRoom,instance);
	this->room.push_back(x);
}

void Solution::moveInstanceToRoom(int currentInstance,int newRoom){
    for(int i = 0 ; i < room.size(); i++){
        if(room[i].second == currentInstance){
            room[i].first = newRoom;
            break;
        }
    }
}

void Solution::moveInstanceToHour(int currentInstance,int newHour){
    for(int i = 0 ; i < hour.size(); i++){
        if(hour[i].second == currentInstance){
            hour[i].first = newHour;
            break;
        }
    }
}

ostream& operator<<(ostream& os , Solution& s) {
	int objective = s.getObjective();

	os << "Solution with objective ";
	os << objective << endl;

	for(std::vector<std::pair <int,int>>::iterator i = s.teacher.begin(); i != s.teacher.end(); ++i) {
        os << "Teacher: " << i->first << " Instance: " <<  i->second << endl;

        for(std::vector<std::pair <int,int>>::iterator j = s.hour.begin(); j != s.hour.end(); ++j) {
            if(i->second == j->second){
                os << " Hour: " << j->first << endl;
                break;
            }
        }
        for(std::vector<std::pair <int,int>>::iterator k = s.room.begin(); k != s.room.end(); ++k) {
           if(i->second == k->second){
                os << " Room: " << k->first << endl;
                break;
            }
        }
	}
	return os;
}
