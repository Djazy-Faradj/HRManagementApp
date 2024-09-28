// HrManagement.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

const unordered_map<string, int> degreeRateMap = { // Create an unordered map linking each degree with their respective rates
    {"PhD", 112},
    {"Master", 82},
    {"Bachelor", 42}  
}; 

struct Person { // Person class declaration
    private:
        int id;
        int age;
        string name;
    protected:
        static int nextID;
        string category;

        virtual void SetCategory() { category = "Person"; }
    public:
        static vector<int> personnelAgeVector;
        static int staffCount;
        static int fullTimeCount;
        static int partTimeCount;

        // SETTERS
        void setAge(int a)
        {
            age = a;
        }
        void setName(string n)
        {
            name = n;
        }
        void setID(int i)
        {
            id = i;
            nextID++; // Everytime an ID is assigned, change the next id for the next person to be instanciated
        }

        // GETTERS
        int getAge() const
        {
            return age;
        }
        string getName() const
        {
            return name;
        }
        string getCategory() const
        {
            return category;
        }
        int getID() const
        {
            return id;
        }
};
int Person::nextID = 1;
int Person::staffCount = 0;
int Person::partTimeCount = 0;
int Person::fullTimeCount = 0;
vector<int> Person::personnelAgeVector = {};
struct Staff : public Person { // Staff class declaration and inherits from Person 
    private:
        string duty;
        int workload;

        void setCategory()
        {
            category = "Staff";
        }
    public:

        // SETTERS
        void setWorkload(int w)
        {
            workload = w;
        }
        void setDuty(string d)
        {
            duty = d;
        }

        // GETTERS
        int getWorkload() const
        {
            return workload;
        }
        string getDuty() const
        {
            return duty;
        }

        Staff(string name, int age, string duty, int workload = 0)   // Staff class constructor
        {
            setAge(age);
            setName(name);
            setCategory();
            setWorkload(workload);
            setDuty(duty);
            setID(nextID);

            personnelAgeVector.push_back(age);
            staffCount++;
        }

        float ComputePayRoll() const
        {
            return ((workload * 32 * 2) * 0.75f);
        }
};
struct Teacher : public Person { // Teacher class declaration and inherits from Person 
    protected:
        int hoursWorked;
        string type;
        string speciality;
        string degree;

        void setCategory()
        {
            category = "Teacher";
        }
    public:

        // SETTERS 
        void setHoursWorked(int i)
        {
            hoursWorked = i;
        }
        void setType(string t)
        {
            type = t; // part-time or full-time
        }
        void setSpeciality(string s)
        {
            speciality = s;  // i.e. Computer Science, Math, etc.
        }
        void setDegree(string s)
        {
            degree = s;
        }

        // GETTERS
        int getHoursWorked() const
        {
            return hoursWorked;
        }
        string getType() const
        {
            return type;
        }
        string getSpeciality() const
        {
            return speciality;
        }
        string getDegree() const
        {
            return degree;
        }
};
struct PartTime : public Teacher { // PartTime class declaration and inherits from Teacher 
    public:
        float ComputePayRoll() const
        {
            return ((hoursWorked * degreeRateMap.at(degree) * 2) * 0.76f);
        }

        PartTime(string name, int age, string speciality, string degree, int hoursWorked = 0)   // PartTime class constructor
        {
            setAge(age);
            setName(name);
            setCategory();
            setHoursWorked(hoursWorked);
            setType("part-time");
            setSpeciality(speciality);
            setDegree(degree);

            setID(nextID);

            personnelAgeVector.push_back(age);
            partTimeCount++;
        }

};
struct FullTime : public Teacher { // FullTime class declaration and inherits from Teacher 
    public:
        float ComputePayRoll() const
        {
            return ((hoursWorked * degreeRateMap.at(degree) * 2) * 0.85f);
        }

        FullTime(string name, int age, string speciality, string degree)   // PartTime class constructor
        {
            setAge(age);
            setName(name);
            setCategory();
            setHoursWorked(32);  // Full-time teachers work 32 hours all the time
            setType("full-time");
            setSpeciality(speciality);
            setDegree(degree);

            setID(nextID);

            personnelAgeVector.push_back(age);
            fullTimeCount++;
        }
};

struct Department{ // Department class declaration
    private:
        int id;
        static int nextID;
        string name;
        Teacher dean;
        vector<Teacher> teacherList;
        vector<Staff> staffList;

    public:

        // SETTERS
        void setID(int i)
        {
            id = i;
            nextID++; // Everytime an ID is assigned, change the next id for the next person to be instanciated
        }
        void setName(string n)
        {
            name = n;
        }
        void setDean(Teacher t)
        {
            dean = t;
            teacherList.emplace(teacherList.begin(), t);  // The dean is ALWAYS the first element in the department teacher list
        }
        void setTeacherList(vector<Teacher> tl)
        {
            teacherList = tl;
        }
        void setStaffList(vector<Staff> sl)
        {
            staffList = sl;
        }

        // GETTERS
        int getID() const
        {
            return id;
        }
        string getName() const
        {
            return name;
        }
        Teacher getDean() const
        {
            return dean;
        }
        vector<Teacher> getTeacherList() const
        {
            return teacherList;
        }

        string addTeacher(Teacher t) // First check if teacher is already in dept. if not, add and return success message, if yes, return error message
        {
            for (Teacher x : teacherList)
            {
                if (x.getID() == t.getID())
                {
                    return (t.getName() + " is already assigned in the \"" + name + "\" department."); // Error message
                }
            }
            teacherList.push_back(t);
            return ("Added " + t.getName() + " to the \"" + name + "\" department."); // Success message
        } 
        string removeTeacher(Teacher t) // First check if teacher is already in dept. if yes, remove and return success message, if not, return error message. If remove attempt is a dean return error message too
        {
            for (int i = 0; i < teacherList.size(); i++)
            {
                if (teacherList[i].getID() == t.getID() && teacherList[i].getID())
                {
                    if (teacherList[i].getID() == dean.getID()) return ("Cannot remove dean from department. Please assign a new dean first.");
                    teacherList.erase(teacherList.begin() + i);
                    return ("Removed " + t.getName() + " from the \"" + name + "\" department."); // Success message
                }
            }
            if (teacherList.size() == 0) return ("Department currently has no teachers.");
            else return (t.getName() + " is not in the \"" + name + "\" department."); // Error message
        }

        Department(string name, Teacher dean)
        {
            setName(name);
            setDean(dean);
            setID(nextID);
        }

};
int Department::nextID = 1;

int getPersonnelAvgAge()
{   
    int sum = 0;
    if (Person::personnelAgeVector.size() == 0) return 0; // Avoid to divide by 0 if age vector is empty
    for (int i : Person::personnelAgeVector) sum += i; // Sums up the ages of personnelAgeVector
    return (sum / Person::personnelAgeVector.size()); // Gets average and returns it as a rounded up int
}

int Init() // Get called at the start of the program, will either create a data file or read it and load the state
{
    return 0;
}

int main()
{
    Init();
    FullTime t1("Thomas", 24, "Math", "Master");
    FullTime t2("Michael", 45, "Computer Science", "PhD");
    PartTime t3("Sarah", 20, "Chemist", "Bachelor", 15);

    Department d1("Computer Engineering", t1);
    cout << d1.addTeacher(t2) << endl;
    cout << d1.removeTeacher(t1) << endl;
    cout << d1.removeTeacher(t2) << endl;
    cout << d1.removeTeacher(t3) << endl;

    cout << d1.getTeacherList().size() << endl;


    return 0;
}

