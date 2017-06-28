#include "../hpp/teacher_preference.hpp"

TeacherPreference::TeacherPreference(string teacher,string discipline,string curriculum,int weight1,std::vector<int> primaryHours
                                     ,int weight2,std::vector<int> secundaryHours){
	this->teacher = teacher;
	this->discipline = discipline;
	this->curriculum = curriculum;
	this->weight1 = weight1;
	this->weight2 = weight2;

	for(std::vector<int>::iterator it = primaryHours.begin(); it != primaryHours.end(); ++it) {
		this->primaryHours.push_back(*it);	
	}

	for(std::vector<int>::iterator it = secundaryHours.begin(); it != secundaryHours.end(); ++it) {
		this->secundaryHours.push_back(*it);
	}


}

ostream& operator<<(ostream& os , TeacherPreference& t) {

	os << "Teacher: " << t.getTeacherName() << endl;
	os << "Discipline: " << t. getDiscipline();
	os << " Curriculum: " << t.getCurriculum() << endl;
	os << "Main Preference: " << t.getMainWeight();
	os << " Hours ";


	vector<int> mainHours = t.getMainHours();
	for(int i = 0 ; i < mainHours.size(); i++){
		os << mainHours[i] ;
		os << " , " ;
	}

	os << " Secundary Preference : " ;
	os << t.getSecundaryWeight() ;
	
	os << " Hours: " ;

	vector<int> secundaryHours = t.getSecundaryHours();
	for(int i = 0 ; i < secundaryHours.size(); i++){
		os << secundaryHours[i] ;
		os << " , " ;
	}

	os << endl ;
	

	return os;
}
