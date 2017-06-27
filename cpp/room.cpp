#include "../hpp/room.hpp"

int Room::counter = 0;

Room::Room(string room, int capacity){
	this->room = room;
	this->capacity = capacity;
}

void Room::increment(){
	counter++;
	this->id = counter;
}
