// HrManagement.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <unordered_map>
using namespace std;

const unordered_map<string, int> degreeRates = { // Create an unordered map linking each degree with their respective rates
    {"PhD", 112},
    {"Master", 82},
    {"Bachelor", 42}
};

struct Person { // Person class definition
    private:
        int id;
        int age;
        string name;
        string category;
    protected:
        static int nextID;

        virtual void SetCategory(string category);
    public:
        static int personnelCount;
        static int personnelAvgAge;
        static int fullTimeCount;
        static int partTimeCount;

};

struct Staff : public Person { // Staff class definition and inherits from Person 
    private:
        string duty;
        int workload;
        void SetCategory(string category);
    public:
        float ComputePayRoll(int workload);

};

struct Teacher : public Person { // Teacher class definition and inherits from Person 
    private:
        string type;
        string speciality;
        string degree;
        void SetCategory(string category);

};

struct PartTime : public Teacher { // PartTime class definition and inherits from Teacher 
    private:
        int hoursWorked;
    public:
        float ComputePayRoll(int hoursWorked, int degreeRate) const;

};

struct FullTime : public Teacher { // FullTime class definition and inherits from Teacher 
    public:
        float ComputePayRoll(int degreeRate) const;

};

struct Department{
    private:
        int id;
        string name;
        Teacher dean;
        vector<Teacher> teacherList;
        vector<Staff> staffList;

    public:


};

int main()
{
    std::cout << "Hello World!\n";
}
