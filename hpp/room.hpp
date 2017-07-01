#ifndef INCLUDED_ROOM_HPP
#define INCLUDED_ROOM_HPP
using namespace std;
using std::string;

class Room {
	public:
	    static int counter;
	    Room(string,int);
	    Room();
	    string getRoomName(){return room;};
	    int getRoomCapacity(){return capacity;};
	    setRoomName(string room){this->room = room;};
	    setRoomCapacity(int capacity){this->capacity = capacity;};
	    int getID(){ return this->id;};
	    void increment();
	private:
	    int id;
		string room;
		int capacity;
	friend ostream& operator<<(ostream& , Room&);
};


#endif // INCLUDED_ROOM_HPP
