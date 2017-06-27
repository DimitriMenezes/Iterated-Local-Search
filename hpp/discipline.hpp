#ifndef INCLUDED_DISCIPLINE_HPP
#define INCLUDED_DISCIPLINE_HPP
using namespace std;
using std::string;

class Discipline {

	public:
	    static int counter;

	    Discipline(string);
		string getDiscipline(){return this->discipline;};
		setDiscipline(string discipline){this->discipline = discipline;};

		int getID(){ return this->id;};
		void increment();
	private:
	    int id;
		string discipline;

};


#endif // INCLUDED_DISCIPLINE_HPP
