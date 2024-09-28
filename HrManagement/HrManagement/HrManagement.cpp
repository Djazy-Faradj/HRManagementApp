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
    public:
        void setCategory()
        {
            category = "Staff";
        }

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
    public:
        void setCategory()
        {
            category = "Teacher";
        }

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
        int getHoursWorked()
        {
            return hoursWorked;
        }
        string getType()
        {
            return type;
        }
        string getSpeciality()
        {
            return speciality;
        }
        string getDegree()
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
        string name;
        Teacher dean;
        vector<Teacher> teacherList;
        vector<Staff> staffList;

    public:


};

int getPersonnelAgeAverage()
{   
    int sum = 0;
    if (Person::personnelAgeVector.size() == 0) return 0; // Avoid to divide by 0 if age vector is empty
    for (int i : Person::personnelAgeVector) sum += i; // Sums up the ages of personnelAgeVector
    return (sum / Person::personnelAgeVector.size()); // Gets average and returns it as a rounded up int
}

int main()
{
    // Testing the classes
    PartTime t1("Gerald", 10, "English", "Bachelor", 15);
    cout << "T1 Name: " << t1.getName() << endl;
    cout << "T1 Age: " << t1.getAge() << endl;
    cout << "T1 Category: " << t1.getCategory() << endl;
    cout << "T1 ID: " << t1.getID() << endl;
    cout << "T1 Speciality: " << t1.getSpeciality() << endl;
    cout << "T1 Degree: " << t1.getDegree() << endl;
    cout << "T1 ComputePayRoll(): " << t1.ComputePayRoll() << endl;

    FullTime t2("Thomas", 30, "Computer Science", "PhD");
    cout << endl << endl << "T2 Name: " << t2.getName() << endl;
    cout << "T2 Age: " << t2.getAge() << endl;
    cout << "T2 Category: " << t2.getCategory() << endl;
    cout << "T2 ID: " << t2.getID() << endl;
    cout << "T2 Speciality: " << t2.getSpeciality() << endl;
    cout << "T2 Degree: " << t2.getDegree() << endl;
    cout << "T2 ComputePayRoll(): " << t2.ComputePayRoll() << endl;

    Staff s1("Michael", 20, "Floor cleaning", 23);
    cout << endl << endl << "S1 Name: " << s1.getName() << endl;
    cout << "S1 Age: " << s1.getAge() << endl;
    cout << "S1 Category: " << s1.getCategory() << endl;
    cout << "S1 ID: " << s1.getID() << endl;
    cout << "S1 Duty: " << s1.getDuty() << endl;
    cout << "S1 Workload: " << s1.getWorkload() << endl;
    cout << "S1 ComputePayRoll(): " << s1.ComputePayRoll() << endl;

    cout << endl << endl;
    cout << "Total part-time count: " << Person::partTimeCount << endl;
    cout << "Total full-time count: " << Person::fullTimeCount << endl;
    cout << "Total teacher count: " << Person::partTimeCount + Person::fullTimeCount << endl;
    cout << "Total staff count: " << Person::staffCount << endl;
    cout << "Personnel age average: " << getPersonnelAgeAverage() << endl;

    return 0;
}

