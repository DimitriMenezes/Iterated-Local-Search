#include "../hpp/room.hpp"

int Room::counter = 1;

Room::Room(){
	
}

Room::Room(string room, int capacity){
	this->id = counter;
	this->room = room;
	this->capacity = capacity;
}

void Room::increment(){
	counter++;
}

ostream& operator<<(ostream& os , Room& r) {

	os << "Room: " << r.getRoomName() ;
	os << " Capacity: " ;
	os << r.getRoomCapacity();
	os << " ID: " ;
	os << r.getID() << std::endl ;

	return os;
}
