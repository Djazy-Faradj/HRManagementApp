// HrManagement.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <stdlib.h>
using namespace std;

bool run = true;

const unordered_map<string, int> degreeRateMap = { // Create an unordered map linking each degree with their respective rates
    {"PhD", 112},
    {"Master", 82},
    {"Bachelor", 42}  
}; 
enum MENU_STATE {
    MAIN_MENU,
    DEPARTMENT_CREATE,
    DEPARTMENT_DELETE,
    DEPARTMENT_MODIFY,
    PERSON_CREATE,
    PERSON_DELETE,
    PERSON_MODIFY,
    SETTINGS,
    CLOSE

};
MENU_STATE menuState;

struct Person { // Person class declaration
    private:
        int age;
        string name;
    protected:
        int id;
        static int nextID;
        string category;

        virtual void SetCategory() { category = "Person"; }
    public:
        static vector<int> personnelAgeVector;
        static vector<int> teacherInstancesIDVector;
        static vector<int> staffInstancesIDVector;
        static int staffCount;
        static int fullTimeCount;
        static int partTimeCount;

        // SETTERS
        void setAge(int a) { age = a; }
        void setName(string n) { name = n; }
        void setID(int i)
        {
            id = i;
            nextID++; // Everytime an ID is assigned, change the next id for the next person to be instanciated
        }

        // GETTERS
        int getAge() const { return age; }
        string getName() const { return name; }
        string getCategory() const { return category; }
        int getID() const { return id; }
};
int Person::nextID = 1;
int Person::staffCount = 0;
int Person::partTimeCount = 0;
int Person::fullTimeCount = 0;
vector<int> Person::personnelAgeVector = {};
vector<int> Person::teacherInstancesIDVector = {};
vector<int> Person::staffInstancesIDVector = {};
struct Staff : public Person { // Staff class declaration and inherits from Person 
    private:
        string duty;
        int workload;

        void setCategory() { category = "Staff"; }
    public:

        // SETTERS
        void setWorkload(int w) { workload = w; }
        void setDuty(string d) { duty = d; }

        // GETTERS
        int getWorkload() const { return workload; }
        string getDuty() const { return duty; }

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

        float ComputePayRoll() const { return ((workload * 32 * 2) * 0.75f); }
};
struct Teacher : public Person { // Teacher class declaration and inherits from Person 
    protected:
        int hoursWorked;
        string type;
        string speciality;
        string degree;

        void setCategory() { category = "Teacher"; }
    public:

        // SETTERS 
        void setHoursWorked(int i) { hoursWorked = i; }
        void setType(string t) { type = t; }// part-time or full-time
        void setSpeciality(string s) { speciality = s; } // i.e. Computer Science, Math, etc.
        void setDegree(string s) { degree = s; }

        // GETTERS
        int getHoursWorked() const { return hoursWorked; }
        string getType() const { return type; }
        string getSpeciality() const { return speciality; }
        string getDegree() const { return degree; }
};
struct PartTime : public Teacher { // PartTime class declaration and inherits from Teacher 
    public:
        float ComputePayRoll() const { return ((hoursWorked * degreeRateMap.at(degree) * 2) * 0.76f); }

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
        float ComputePayRoll() const { return ((hoursWorked * degreeRateMap.at(degree) * 2) * 0.85f); }

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

vector<Teacher> teacherInstancesVector = {};
vector<Staff> staffInstancesVector = {};

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
        void setName(string n) { name = n; }
        void setDean(Teacher t)
        {
            dean = t;
            teacherList.emplace(teacherList.begin(), t);  // The dean is ALWAYS the first element in the department teacher list
        }
        void setTeacherList(vector<Teacher> tl) { teacherList = tl; }
        void setStaffList(vector<Staff> sl) { staffList = sl; }

        // GETTERS
        int getID() const { return id; }
        string getName() const { return name; }
        Teacher getDean() const { return dean; }
        vector<Teacher> getTeacherList() const { return teacherList; }

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
vector<Department> departmentInstancesVector = {};

int getPersonnelAvgAge()
{   
    int sum = 0;
    if (Person::personnelAgeVector.size() == 0) return 0; // Avoid to divide by 0 if age vector is empty
    for (int i : Person::personnelAgeVector) sum += i; // Sums up the ages of personnelAgeVector
    return (sum / Person::personnelAgeVector.size()); // Gets average and returns it as a rounded up int
}

void drawAppTitle()
{
    cout << "*************** HR MANAGEMENT ***************";
}

int Init() // Get called at the start of the program, will either create a data file or read it and load the state
{
    // Read from data file
    // load state

    return 0;
}

void getExpectedIntInput(int* var, vector<int> expectedInputs, string errorMessage)
{
    // If first int of provided vector is -1, then assume the two other elements represent an integer range. If not just treat each value in vector as valid inputs
    int newInt;
    cin >> newInt;
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if (expectedInputs[0] == -1)
    {   // If vector represents a range of valid inputs
        for (int i = expectedInputs[1]; i <= expectedInputs[2]; i++)
        {
            if (i == newInt) *var = newInt;
        } 
    }
    else if (find(expectedInputs.begin(), expectedInputs.end(), newInt) != expectedInputs.end()) *var = newInt;
    if (*var == -1) {
        cout << errorMessage;
        getExpectedIntInput(var, expectedInputs, errorMessage);
    }
}

void mainMenu()
{
    system("CLS");
    drawAppTitle();
    int choice;
    cout << "\n\n--Main Menu--\n" << endl;
    // Menu options
    cout << "1--Create Department          4--Create Person" << endl;
    cout << "2--Delete Department          5--Delete Person" << endl;
    cout << "3--Modify Department          6--Modify Person" << endl;
    cout << "\n7--Settings" << endl;
    cout << "8--Exit" << endl;
    cout << endl << "*********************************************";
    cout << endl << endl;
    cout << "Please type your desired action: ";
    cin >> choice;

    switch(choice)
    {
        case 1:
            menuState = DEPARTMENT_CREATE;
            break;
        case 2:
            menuState = DEPARTMENT_DELETE;
            break;
        case 3:
            menuState = DEPARTMENT_MODIFY;
            break;
        case 4:
            menuState = PERSON_CREATE;
            break;
        case 5:
            menuState = PERSON_DELETE;
            break;
        case 6:
            menuState = PERSON_MODIFY;
            break;
        case 7:
            menuState = SETTINGS;
            break;
        case 8:
            menuState = CLOSE;
            break;
        default:
            break; // Repeats menu as no proper key was typed

    }
}

void createDepartment()
{
    system("CLS");
    drawAppTitle();
    bool departmentCreated = false;

    string name = "";
    int goToPersonCreatePrompt;
    int teacherID;
    cout << "\n\n--Create Department--\n" << endl;
    cin.get();
    // createDepartment steps
    cout << "Please type a name for the new department";
    cout << endl << "\n*********************************************";
    cout << endl << endl;
    do {
        cout << "Department name: ";
        getline(cin, name);
        for (Department d : departmentInstancesVector)
        {
            if (transform(name.begin(), name.end(), name.begin(), ::toupper) == transform(d.getName().begin(), d.getName().end(), d.getName().begin(), ::toupper))
            {
                cout << "Department already exists\n";
                name = "";
            }
        }
    } while (name.length() == 0);
    cout << endl;
    if (teacherInstancesVector.size() == 0)
    {
        cout << "There are currently no existing teacher in the college.\nPlease create a teacher first to assign as dean. \n\nType 1 to go to Person creation page: ";
        cin >> goToPersonCreatePrompt;

        switch (goToPersonCreatePrompt)
        {
            case 1:
                menuState = PERSON_CREATE;
                break;
            default:
                break;
        }
    }
    else
    {
        while (!departmentCreated)
        {
            cout << "Please assign a teacher to be this department's dean (Enter teacher ID): ";
            cin >> teacherID;
            for (Teacher t : teacherInstancesVector)
            {
                if (t.getID() == teacherID)
                {
                    Department d(name, t);
                    departmentInstancesVector.push_back(d);
                    departmentCreated = true;
                }
            }
            if (!departmentCreated) cout << "ID provided does not match any existing teacher.\n";
        }
        cout << "Department has been created successfully. \n";
        system("pause");
        menuState = MAIN_MENU;
    }
}

void deleteDepartment()
{
    system("CLS");
    drawAppTitle();
    int choice;
    cout << "\n\n--Delete Department--" << endl;
    // deleteDepartment options
    cout << endl << "*********************************************";
    cout << endl << endl;
    cout << "Please type your desired action: ";
    cin >> choice;

    switch (choice)
    {
    case 1:
        menuState = DEPARTMENT_CREATE;
        break;
    case 2:
        menuState = DEPARTMENT_DELETE;
        break;
    case 3:
        menuState = DEPARTMENT_MODIFY;
        break;
    case 4:
        menuState = PERSON_CREATE;
        break;
    case 5:
        menuState = PERSON_DELETE;
        break;
    case 6:
        menuState = PERSON_MODIFY;
        break;
    case 7:
        menuState = SETTINGS;
        break;
    case 8:
        menuState = CLOSE;
        break;
    default:
        break; // Repeats menu as no proper key was typed

    }
}

void modifyDepartment()
{
    system("CLS");
    drawAppTitle();
    int choice;
    cout << "\n\n--Modify Department--" << endl;
    // modifyDepartment options
    cout << endl << "*********************************************";
    cout << endl << endl;
    cout << "Please type your desired action: ";
    cin >> choice;

    switch (choice)
    {
    case 1:
        menuState = DEPARTMENT_CREATE;
        break;
    case 2:
        menuState = DEPARTMENT_DELETE;
        break;
    case 3:
        menuState = DEPARTMENT_MODIFY;
        break;
    case 4:
        menuState = PERSON_CREATE;
        break;
    case 5:
        menuState = PERSON_DELETE;
        break;
    case 6:
        menuState = PERSON_MODIFY;
        break;
    case 7:
        menuState = SETTINGS;
        break;
    case 8:
        menuState = CLOSE;
        break;
    default:
        break; // Repeats menu as no proper key was typed

    }
}

void createPerson()
{
    int categoryChoice = -1;
    int age = -1;
    int degreeChoice = -1;
    int hoursWorked = -1;
    int workload = -1;
    string name;
    string speciality;
    string degree;
    string duty;

    system("CLS");
    drawAppTitle();
    cout << "\n\n--Create Person--\n" << endl;
    // createPerson
    cout << "Please indicate the category of the person.\n";
    cout << "1--Teacher               2--Staff";
    cout << endl << "\n*********************************************";
    cout << endl << endl;
    cout << "Please indicate your desired category: ";
    getExpectedIntInput(&categoryChoice, { 1, 2 }, "Incorrect input, please type a correct category: ");

        if (categoryChoice == 1)
        {
            system("CLS");
            drawAppTitle();
            cout << "\n\n--Create Person-TEACHER--\n" << endl;
            // createPerson options
            cout << "Please write the full name of the teacher.";
            cout << endl << "\n*********************************************";
            cout << endl << endl;
            cout << "Name: ";
            getline(cin, name);
            cout << "\n\nNow please type the teacher's age: ";
            getExpectedIntInput(&age, { -1, 16, 125 }, "Accepted age range is from 16-125 years old: ");
            cout << "\n\nNow please type the teacher's speciality (i.e. Math, English, etc.): ";
            getline(cin, speciality);
            cout << "\n\nNow please indicate the teacher's degree. (Default degree: Bachelor)\n1--Bachelor         2--Master         3--PhD\n\nDegree: ";
            getExpectedIntInput(&degreeChoice, { 1, 2, 3 }, "Incorrect input, please type a correct degree: ");
            cout << "\n\nNow please type the teacher's weekly work hours (32hrs/week will assign the teacher as full-time): ";
            getExpectedIntInput(&hoursWorked, { -1, 3, 32 }, "Accepted work time ranges from 3-32 Hours/Week: ");

            switch (degreeChoice)
            {
            case 2:
                degree = "Master";
                break;
            case 3:
                degree = "PhD";
                break;
            default:
                degree = "Bachelor";
                break;
            }
            if (hoursWorked == 32)
            {
                FullTime t(name, age, speciality, degree);
                teacherInstancesVector.push_back(t);
            }
            else
            {
                PartTime t(name, age, speciality, degree, hoursWorked);
                teacherInstancesVector.push_back(t);
            }
        }
        else if (categoryChoice == 2)
        {
            system("CLS");
            drawAppTitle();
            cout << "\n\n--Create Person-STAFF--\n" << endl;
            // createPerson options
            cout << "Please write the full name of the staff person.";
            cout << endl << "\n*********************************************";
            cout << endl << endl;
            cout << "Name: ";
            getline(cin, name);
            cout << "\n\nNow please type the staff person's age: ";
            getExpectedIntInput(&age, { -1, 16, 125 }, "Accepted age range is from 16-125 years old: ");
            cout << "\n\nNow please type the staff person's duty: ";
            getline(cin, duty);
            cout << "\n\nNow please type the staff person's workload: ";
            getExpectedIntInput(&hoursWorked, { -1, 3, 32 }, "Accepted workload ranges from 3-32 Hours/Week: ");

            Staff s(name, age, duty, workload);
            staffInstancesVector.push_back(s);
        }
    cout << "Person created successfully. \n";
    system("pause");
    menuState = MAIN_MENU;
}

void deletePerson()
{
    system("CLS");
    drawAppTitle();
    int choice;
    cout << "\n\n--Delete Person--" << endl;
    // deletePerson options
    cout << endl << "*********************************************";
    cout << endl << endl;
    cout << "Please type your desired action: ";
    cin >> choice;

    switch (choice)
    {
    case 1:
        menuState = DEPARTMENT_CREATE;
        break;
    case 2:
        menuState = DEPARTMENT_DELETE;
        break;
    case 3:
        menuState = DEPARTMENT_MODIFY;
        break;
    case 4:
        menuState = PERSON_CREATE;
        break;
    case 5:
        menuState = PERSON_DELETE;
        break;
    case 6:
        menuState = PERSON_MODIFY;
        break;
    case 7:
        menuState = SETTINGS;
        break;
    case 8:
        menuState = CLOSE;
        break;
    default:
        break; // Repeats menu as no proper key was typed

    }
}

void modifyPerson()
{
    system("CLS");
    drawAppTitle();
    int choice;
    cout << "\n\n--Modify Person--" << endl;
    // modifyPerson options
    cout << endl << "*********************************************";
    cout << endl << endl;
    cout << "Please type your desired action: ";
    cin >> choice;

    switch (choice)
    {
    case 1:
        menuState = DEPARTMENT_CREATE;
        break;
    case 2:
        menuState = DEPARTMENT_DELETE;
        break;
    case 3:
        menuState = DEPARTMENT_MODIFY;
        break;
    case 4:
        menuState = PERSON_CREATE;
        break;
    case 5:
        menuState = PERSON_DELETE;
        break;
    case 6:
        menuState = PERSON_MODIFY;
        break;
    case 7:
        menuState = SETTINGS;
        break;
    case 8:
        menuState = CLOSE;
        break;
    default:
        break; // Repeats menu as no proper key was typed

    }
}

void settingsMenu()
{
    system("CLS");
    drawAppTitle();
    int choice;
    cout << "\n\n--Settings--" << endl;
    // Settings options
    cout << endl << "*********************************************";
    cout << endl << endl;
    cout << "Please type your desired action: ";
    cin >> choice;

    switch (choice)
    {
    case 1:
        menuState = DEPARTMENT_CREATE;
        break;
    case 2:
        menuState = DEPARTMENT_DELETE;
        break;
    case 3:
        menuState = DEPARTMENT_MODIFY;
        break;
    case 4:
        menuState = PERSON_CREATE;
        break;
    case 5:
        menuState = PERSON_DELETE;
        break;
    case 6:
        menuState = PERSON_MODIFY;
        break;
    case 7:
        menuState = SETTINGS;
        break;
    case 8:
        menuState = CLOSE;
        break;
    default:
        break; // Repeats menu as no proper key was typed

    }
}

void closeApp()
{
    system("CLS");
    // Write data file on modifications...
    //,.


    cout << "Thank you for using HR Management\n";

    cin.ignore();
    run = false;
}

int main()
{
    Init();

    while (run) // main loop
    {
        switch (menuState)
        {
            case MAIN_MENU:
                mainMenu();
                break;
            case DEPARTMENT_CREATE:
                createDepartment();
                break;
            case DEPARTMENT_DELETE:
                deleteDepartment();
                break;
            case DEPARTMENT_MODIFY:
                modifyDepartment();
                break;
            case PERSON_CREATE:
                createPerson();
                break;
            case PERSON_DELETE:
                deletePerson();
                break;
            case PERSON_MODIFY:
                modifyPerson();
                break;
            case SETTINGS:
                settingsMenu();
                break;
            case CLOSE:
                closeApp();
                break;

        }
    }


    return 0;
}

