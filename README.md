# Iterated-Local-Search
ILS algorithm to solve Curriculum Based Timetabling Problem of UFS 


maximize teacher's preference to have a lesson in a specific hour

subject to

minimum number of credits/hours: Each teacher should have at least N classes during a semester

teacherPreference: Teachers have preference on a hour to a specific curriculum

teacherAvailability: Ensure that a teacher having just 1 or 0 classes in a specific hour.

teacherPossible: Ensure that the teacher can teaches that discipline

roomAvailability: Ensure just one class by time and room

roomCapacity/roomAdequacy: Ensure room capacity >= class capacity. Ensure reserved rooms to specific classrooms

roomEstability: Ensure rooms are the same for classes of specific discipline

curriculumConstraint: Ensure unique classes at time by curriculum

hourPossible: Ensure morning classes start and end at morning. Same for afternoon and evening.

minimumDays: Ensure at least one class allocated by requirement.





