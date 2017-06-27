#ifndef INCLUDED_INSTANCE_HPP
#define INCLUDED_INSTANCE_HPP
using namespace std;
using std::string;

class Instance {

	public:
	    static int counter;
	    Instance(string,string,int,int);
        string getCurriculum();
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
