#ifndef INCLUDED_INSTANCE_HPP
#define INCLUDED_INSTANCE_HPP
using namespace std;
using std::string;

class Instance {

	public:
	    static int counter;
        Instance();
	    Instance(string,string,int,int);
        string getCurriculum(){return this->curriculum;};
        string getDiscipline(){return this->discipline;};
        int getClassCapacity(){return this->capacity;};
        int getID(){ return this->id;};
        void increment();
	private:
        string curriculum;
        string discipline;
        int repetition;
        int capacity;
        int id;

    friend ostream& operator<<(ostream& , Instance&);
};


#endif // INCLUDED_INSTANCE_HPP
