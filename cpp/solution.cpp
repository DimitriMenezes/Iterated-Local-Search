#include "../hpp/solution.hpp"

Solution::Solution(){
    this->objective = 0;
}

void Solution::addInstanceInTeacher(int teacher, int instance){
	std::pair <int,int> x = std::make_pair (teacher,instance);
	this->teacher.push_back(x);
}

void Solution::addInstanceInHour(int hour, int instance){
	std::pair <int,int> x = std::make_pair (hour,instance);
	this->hour.push_back(x);
}

void Solution::addInstanceInRoom(int room , int instance){
	std::pair <int,int> x = std::make_pair (room,instance);
	this->room.push_back(x);
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
