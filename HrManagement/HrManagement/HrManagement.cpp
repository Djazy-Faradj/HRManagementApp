// HrManagement.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <unordered_map>

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
    VIEW_DEPARTMENTS,
    PAYROLLS,
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
        string category;

        virtual void SetCategory() { category = "Person"; }
    public:
        static int nextID; // Is public to be accessible by saveData();
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
        virtual float ComputePayRoll() const { return 0.0f; }

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
        float ComputePayRoll() const override { return ((hoursWorked * degreeRateMap.at(degree) * 2) * 0.76f); }

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
        float ComputePayRoll() const override { return ((hoursWorked * degreeRateMap.at(degree) * 2) * 0.85f); }

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

vector<Teacher*> teacherInstancesVector = {};
vector<Staff*> staffInstancesVector = {};

struct Department{ // Department class declaration
    private:
        int id;
        string name;
        Teacher dean;
        vector<Teacher> teacherList;
        vector<Staff> staffList;

    public:
        static int nextID;

        // SETTERS
        void setID(int i)
        {
            id = i;
            nextID++; // Everytime an ID is assigned, change the next id for the next person to be instanciated
        }
        void setName(string n) { name = n; }
        void setDean(Teacher t)
        {
            if (t.getID() != dean.getID())
            {
                dean = t;
            }
        }
        void setTeacherList(vector<Teacher> tl) { teacherList = tl; }
        void setStaffList(vector<Staff> sl) { staffList = sl; }

        // GETTERS
        int getID() const { return id; }
        string getName() const { return name; }
        Teacher getDean() const { return dean; }
        vector<Teacher> getTeacherList() const { return teacherList; }
        vector<Staff> getStaffList() const { return staffList; }

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
                    if (teacherList[i].getID() == dean.getID()) return ("Cannot remove dean from department");
                    teacherList.erase(teacherList.begin() + i);
                    return ("Removed " + t.getName() + " from the \"" + name + "\" department."); // Success message
                }
            }
            if (teacherList.size() == 0) return ("Department currently has no teachers.");
            else return (t.getName() + " is not in the \"" + name + "\" department."); // Error message
        }

        string addStaff(Staff s) // First check if staff is already in dept. if not, add and return success message, if yes, return error message
        {
            for (Staff x : staffList)
            {
                if (x.getID() == s.getID())
                {
                    return (s.getName() + " is already assigned in the \"" + name + "\" department."); // Error message
                }
            }
            staffList.push_back(s);
            return ("Added " + s.getName() + " to the \"" + name + "\" department."); // Success message
        }
        string removeStaff(Staff s) // First check if staff is already in dept. if yes, remove and return success message, if not, return error message. 
        {
            for (int i = 0; i < staffList.size(); i++)
            {
                if (staffList[i].getID() == s.getID() && staffList[i].getID())
                {
                    if (staffList[i].getID() == dean.getID()) return ("Cannot remove dean from department. Please assign a new dean first.");
                    staffList.erase(staffList.begin() + i);
                    return ("Removed " + s.getName() + " from the \"" + name + "\" department."); // Success message
                }
            }
            if (staffList.size() == 0) return ("Department currently has no teachers.");
            else return (s.getName() + " is not in the \"" + name + "\" department."); // Error message
        }

        Department(string name, Teacher dean)
        {
            setName(name);
            setDean(dean);
            setID(nextID);
        }

};
int Department::nextID = 1;
vector<Department*> departmentInstancesVector = {};

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

Teacher* getTeacherById(int ID)
{
    for (int i = 0; i < teacherInstancesVector.size(); i++)
    {
        if (teacherInstancesVector[i]->getID() == ID)
        {
            return teacherInstancesVector[i];
        }
    }
    return nullptr;
}
Staff* getStaffById(int ID)
{
    for (int i = 0; i < staffInstancesVector.size(); i++)
    {
        if (staffInstancesVector[i]->getID() == ID)
        {
            return staffInstancesVector[i];
        }
    }
    return nullptr;
}
Department* getDepartmentById(int ID)
{
    for (int i = 0; i < departmentInstancesVector.size(); i++)
    {
        if (departmentInstancesVector[i]->getID() == ID)
        {
            return departmentInstancesVector[i];
        }
    }
    return nullptr;
}

void displayTeacherInformation(Teacher t)
{
    system("CLS");
    drawAppTitle();
    cout << "\n\n********************INFO********************\n\n";
    if (t.getType() == "full-time")
    {
        cout << "1--Name: " << t.getName() << endl;
        cout << "#--ID: " << t.getID() << endl;
        cout << "2--Age: " << t.getAge() << endl;
        cout << "#--Type: " << t.getType() << endl;
        cout << "3--Speciality: " << t.getSpeciality() << endl;
        cout << "4--Degree: " << t.getDegree() << endl;
        cout << "#--Hours Worked: " << t.getHoursWorked() << endl;
        cout << endl << "*********************************************" << endl;
    }
    else
    {
        cout << "1--Name: " << t.getName() << endl;
        cout << "#--ID: " << t.getID() << endl;
        cout << "2--Age: " << t.getAge() << endl;
        cout << "#--Type: " << t.getType() << endl;
        cout << "3--Speciality: " << t.getSpeciality() << endl;
        cout << "4--Degree: " << t.getDegree() << endl;
        cout << "5--Hours Worked: " << t.getHoursWorked() << endl;
        cout << endl << "*********************************************" << endl;
    }
}
void displayStaffInformation(Staff s)
{
    system("CLS");
    drawAppTitle();
    cout << "\n\n********************INFO********************\n\n";
    cout << "1--Name: " << s.getName() << endl;
    cout << "#--ID: " << s.getID() << endl;
    cout << "2--Age: " << s.getAge() << endl;
    cout << "3--Duty: " << s.getDuty() << endl;
    cout << "4--Workload: " << s.getWorkload() << endl;
    cout << endl << "*********************************************" << endl;
}
void displayPageTitle(string s)
{
    system("CLS");
    drawAppTitle();
    cout << "\n\n-- " + s + " --\n" << endl;
}
void displayUserPrompt(string s)
{
    cout << endl << "-------------------------------------------------------------------" << endl << s;
}
void displayListDepartments()
{
    int i = 1;
    for (Department* d : departmentInstancesVector)
    {
        cout << i << ". " << d->getName() << " | Dean: " << d->getDean().getName() << "(" << d->getDean().getID() << ") | Dept. ID: " << d->getID() << endl;
        i++;
    }
    cout << endl << endl << "*********************************************\n";
}
void displayAllTeachers(bool showPayroll = false)
{
    cout <<  "\n*******************TEACHER******************\n\n";
    int i = 1;
    if (teacherInstancesVector.size() == 0) cout << "[Currently empty]\n";
    if (showPayroll)
    {
        for (Teacher* t : teacherInstancesVector)
        {
            if (t->getType() == "full-time") cout << i << ". " << t->getName() << " | Speciality : " << t->getSpeciality() << " | Degree : " << t->getDegree() << " | Status : full - time(32 hours / week) | Payroll: " << fixed << setprecision(2) << t->ComputePayRoll() << "$ CAD." << endl;
            else cout << i << ". " << t->getName()  << " | Speciality: " << t->getSpeciality() << " | Degree: " << t->getDegree() << " | Status: " << t->getType() << " | Hours worked: " << t->getHoursWorked() << " | Payroll: " << fixed << setprecision(2) << t->ComputePayRoll() << "$ CAD." << endl;
            i++;
        }
    }
    else
    {
        for (Teacher* t : teacherInstancesVector)
        {
            if (t->getType() == "full-time") cout << i << ". " << t->getName() << "(ID: " << t->getID() << ") | Age: " << t->getAge() << " | Speciality: " << t->getSpeciality() << " | Degree: " << t->getDegree() << " | Status: full-time (32 hours/week)" << endl;
            else cout << i << ". " << t->getName() << "(ID: " << t->getID() << ") | Age: " << t->getAge() << " | Speciality: " << t->getSpeciality() << " | Degree: " << t->getDegree() << " | Status: " << t->getType() << " | Hours worked: " << t->getHoursWorked() << endl;
            i++;
        }
    }
    //cout << "\n";
}
void displayAllStaff(bool showPayroll = false)
{
    cout << "\n********************STAFF*******************\n\n";
    int i = 1;
    if (staffInstancesVector.size() == 0) cout << "[Currently empty]\n";
    if (showPayroll)
    {
        for (Staff* s : staffInstancesVector)
        {
            cout << i << ". " << s->getName() << " | Age: " << s->getAge() << " | Duty: " << s->getDuty() << " | Workload: " << s->getWorkload() << " | Payroll: " << fixed << setprecision(2) << s->ComputePayRoll() << "$ CAD." << endl;
            i++;
        }
    }
    else
    {
        for (Staff* s : staffInstancesVector)
        {
            cout << i << ". " << s->getName() << "(ID: " << s->getID() << ") | Age: " << s->getAge() << " | Category: " << s->getCategory() << " | Duty: " << s->getDuty() << " | Workload: " << s->getWorkload() << endl;
            i++;
        }
    }
    //cout << "\n";
}
void displayDepartmentListTeacher(Department d)
{
    for (Teacher t : d.getTeacherList())
    {
        if (t.getCategory() == "full-time") cout << " -" << t.getName() << "(ID: " << t.getID() << ") | Age: " << " | Category: " << t.getCategory() << " | Speciality: " << t.getSpeciality() << " | Degree: " << t.getDegree() << endl;
        else cout << " -" << t.getName() << "(ID: " << t.getID() << ") | Age: " << t.getAge() << " | Category: " << t.getCategory() << " | Speciality: " << t.getSpeciality() << " | Degree: " << t.getDegree() << " | Hours worked: " << t.getHoursWorked() << endl ;
    }
}
void displayDepartmentListStaff(Department d)
{
    for (Staff s : d.getStaffList())
    {
        cout << " -" << s.getName() << "(ID: " << s.getID() << ") | Age: " << " | Category: " << s.getCategory() << " | Duty: " << s.getDuty() << " | Workload: " << s.getWorkload() << endl;
    }
}

// Create custom split() function.  
vector<string> customSplit(string str, char separator) {
    vector<string> strings;
    int startIndex = 0, endIndex = 0;
    for (int i = 0; i <= str.size(); i++) {

        // If we reached the end of the word or the end of the input.
        if (str[i] == separator || i == str.size()) {
            endIndex = i;
            string temp;
            temp.append(str, startIndex, endIndex - startIndex);
            strings.push_back(temp);
            startIndex = endIndex + 1;
        }
    }
    return strings;
}


string saveData() //nextID{category(teacher);type(part-time);id;Name;age;Speciality;Degree;Hoursworked|category(teacher);type(full-time);id;name;age;speciality;degree|category(staff);id;name;age;duty;workload|}nextID{id;name;deanid;[staff_ids];[teacher_ids]|}
{
    string dataString = ""; 
    
    //Teachers
    dataString += to_string(Person::nextID) + "{";
    for (Teacher* t : teacherInstancesVector)
    {
        if(t->getType() == "part-time")
        {
            dataString += t->getCategory() + ";part-time;" + to_string(t->getID()) + ";" + t->getName() + ";" + to_string(t->getAge()) + ";" + t->getSpeciality() + ";" + t->getDegree() + ";" + to_string(t->getHoursWorked()) + "|";
        }
        else
        {
            dataString += t->getCategory() + ";full-time;" + to_string(t->getID()) + ";" + t->getName() + ";" + to_string(t->getAge()) + ";" + t->getSpeciality() + ";" + t->getDegree() + "|";
        }
    }

    //Staffs
    for (Staff* s : staffInstancesVector)
    {
        dataString += s->getCategory() + ";" + to_string(s->getID()) + ";" + s->getName() + ";" + to_string(s->getAge()) + ";" + s->getDuty() + ";" + to_string(s->getWorkload()) + "|";
    }
    dataString += "}";

    //Departments
    dataString += to_string(Department::nextID) + "{";
    for (Department* d : departmentInstancesVector)
    {
        string deptTeacherIDVector = "";
        string deptStaffIDVector = "";

        for (Teacher t : d->getTeacherList())
        {
            deptTeacherIDVector += to_string(t.getID()) + ",";
        }
        for (Staff s : d->getStaffList())
        {
            deptStaffIDVector += to_string(s.getID()) + ",";
        }
        dataString += to_string(d->getID()) + ";" + d->getName() + ";" + to_string(d->getDean().getID()) + ";[" + deptStaffIDVector + "];[" + deptTeacherIDVector + "]|";
    }
    dataString += "}";

    ofstream dataFile("data.txt"); // Creates a file to write into
    dataFile << dataString; // Outputs the data saved into the data file
    dataFile.close(); // Closes dataFile

    return "Data saved.\n"; // Display status of operation
}
string loadData() //nextID{category(teacher);type(part-time);id;Name;age;Speciality;Degree;Hoursworked|category(teacher);type(full-time);id;name;age;speciality;degree|category(staff);id;name;age;duty;workload|}nextID{id;name;deanid;[staff_ids];[teacher_ids]|}
{
    Person::personnelAgeVector.clear();
    departmentInstancesVector.clear();
    teacherInstancesVector.clear();
    staffInstancesVector.clear();
    string dataString;
    string currentDataRead = "";
    bool personOrDepartment = true; // true = read for person data ; false = read for department data
    int j;

    string personNextID = "1";
    vector<vector<string>> personsData;

    string departmentNextID = "1";
    vector<vector<string>> departmentsData;


    ifstream dataFile("data.txt"); // Read from this text file
    while (getline(dataFile, dataString)); // Store the text from dataFile to dataString
    {
    }
    dataFile.close(); // Close dataFile

    for (int i = 0; i < dataString.length(); i++)
    {
        switch(personOrDepartment)
        {
        case true:
            if (i != 0 && dataString[i - 1] == '}') 
            {
                personOrDepartment = false;
                j = i;
            }
            else if (dataString[i] == '{' && personNextID == "1") personNextID = dataString.substr(0, i);
            else if (dataString[i] == '|') // End of instance data, repeat process for next instance
            {
                personsData.push_back(customSplit(currentDataRead, ';'));
                currentDataRead = "";
                i++;
            }
            else if (personNextID != "1") // Reads Persons data
                currentDataRead += dataString[i]; // Store the instances info into a string in which we will then split using the separator for each info
            break;
        case false:
            if (dataString[i - 1] == '}')
            {
                personOrDepartment = true;
            }
            else if (dataString[i] == '{' && departmentNextID == "1")
            {
                departmentNextID = dataString.substr(j, i-j);
            }
            else if (dataString[i] == '|') // End of instance data, repeat process for next instance
            {
                departmentsData.push_back(customSplit(currentDataRead, ';'));
                currentDataRead = "";
                i++;
            }
            else if (departmentNextID != "1") // Reads Persons data
                currentDataRead += dataString[i]; // Store the instances info into a string in which we will then split using the separator for each info
            break;
        }

    }

    // Create state of the loaded data
    Person::nextID = stoi(personNextID);
    for (vector<string> personData : personsData)
    {
        if (personData[0] == "Teacher")
        {
            cout << personData[0] << endl;
            if (personData[1] == "part-time")
            {
                Teacher* t = new PartTime(personData[3], stoi(personData[4]), personData[5], personData[6], stoi(personData[7]));
                t->setID(stoi(personData[2]));
                teacherInstancesVector.push_back(t);
            }
            else // full-time
            {
                Teacher* t = new FullTime(personData[3], stoi(personData[4]), personData[5], personData[6]);
                t->setID(stoi(personData[2]));
                teacherInstancesVector.push_back(t);
            }
        }
        else if (personData[0] == "Staff")// Staff person
        {
            cout << personData[0] << endl;
            Staff* s = new Staff(personData[2], stoi(personData[3]), personData[4], stoi(personData[5]));
            s->setID(stoi(personData[1]));
            staffInstancesVector.push_back(s);
        }
    }
    // Create state of the loaded data
    Department::nextID = stoi(departmentNextID);
    for (vector<string> departmentData : departmentsData)
    {
        vector<string> departmentStaffIDs;
        vector<string> departmentTeacherIDs;
        Department* d = new Department(departmentData[1], *getTeacherById(stoi(departmentData[2])));
        d->setID(stoi(departmentData[0]));
        // Loop through the staff IDs and add them to the department that was created
        departmentStaffIDs = customSplit(departmentData[3].substr(1, departmentData[3].length()-2), ',');
        departmentStaffIDs.pop_back();
        for (string staffID : departmentStaffIDs) d->addStaff(*getStaffById(stoi(staffID)));
        // Loop through the teacher IDs and add them to the department that was created
        departmentTeacherIDs = customSplit(departmentData[3].substr(1, departmentData[3].length() - 2), ',');
        departmentTeacherIDs.pop_back();
        for (string teacherID : departmentTeacherIDs) d->addTeacher(*getTeacherById(stoi(teacherID)));

        departmentInstancesVector.push_back(d);
    }

    return "Loaded data.\n"; // Display status of operation
}
string clearData()
{
    if (remove("data.txt") != 0) return "Error deleting data file."; // Tries to delete data file, returns error if it fails
    else return "Cleared data.\n"; // Display status of operation
}

int Init() // Get called at the start of the program, will either create a data file or read it and load the state
{
    loadData();

    return 0;
}

void mainMenu()
{

    int choice = -1;
    displayPageTitle("Main Menu");
    // Menu options
    cout << "1--Create Department          4--Create Person          7--View Departments" << endl;
    cout << "2--Delete Department          5--Delete Person          8--Payrolls" << endl;
    cout << "3--Modify Department          6--Modify Person" << endl;
    cout << "\n9--Settings" << endl;
    cout << "10--Exit" << endl;
    cout << endl << "*********************************************" << endl << endl;
    cout << endl << "Please type your desired action: ";
    getExpectedIntInput(&choice, { -1, 1, 10 }, "Invalid option. Please type a valid menu option: ");

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
        menuState = VIEW_DEPARTMENTS;
        break;
    case 8:
        menuState = PAYROLLS;
        break;
    case 9:
        menuState = SETTINGS;
        break;
    case 10:
        menuState = CLOSE;
        break;
    default:
        break; // Repeats menu as no proper key was typed

    }
}

void createDepartment()
{
    bool departmentCreated = false;
    string name = "";
    int goToPersonCreatePrompt;
    int teacherID;

    displayPageTitle("Create Department");
    displayListDepartments();
    // createDepartment steps
    cout << "Please type a name for the new department";
    cout << endl << "\n*********************************************";
    cout << endl << endl;
    do {
        displayUserPrompt("Department name: ");
        getline(cin, name);
        string upperName;
        for (char x : name) upperName += toupper(x); // Turns the dept. name in all uppercase so it can be compared to other dept. names in uppercase and see if it already exists
        for (Department* d : departmentInstancesVector)
        {
            string upperDeptName;
            for (char y : d->getName()) upperDeptName += toupper(y);

            if (upperName == upperDeptName) // Same name dept. already exists, prompt for a new name
            {
                cout << "\nDepartment already exists\n";
                name = "";
            }
        }
    } while (name.length() == 0);
    cout << endl;
    if (teacherInstancesVector.size() == 0)
    {
        cout << "There are currently no existing teacher in the college.\nPlease create a teacher first to assign as dean.\n ";
        system("pause");
        menuState = MAIN_MENU;
    }
    else
    {
        while (!departmentCreated)
        {
            displayAllTeachers();
            displayUserPrompt("Please assign a teacher to be this department's dean (Enter teacher ID): ");
            cin >> teacherID;
            for (Teacher* t : teacherInstancesVector)
            {
                if (t->getID() == teacherID)
                {
                    Department* d = new Department(name, *t);
                    d->addTeacher(*t);
                    departmentInstancesVector.push_back(d);
                    departmentCreated = true;
                }
            }
            if (!departmentCreated) cout << "ID provided does not match any existing teacher.\n";
        }
        saveData(); // Saves data after creating department
        cout << "Department has been created successfully.\n";
        system("pause");
        menuState = MAIN_MENU;
    }
}

void deleteDepartment()
{
    int departmentID = -1;
    vector<int> acceptedInputsDepartmentID = {};
    for (Department* d : departmentInstancesVector)
    {
        acceptedInputsDepartmentID.push_back(d->getID());
    }

    displayPageTitle("Delete Department");
    // deletePerson options
    cout << endl << "*********************************************\n\n";
    if (teacherInstancesVector.size() >= 1)
    {
        displayListDepartments();
        displayUserPrompt("Please type the ID number of the department you wish to delete: ");
        getExpectedIntInput(&departmentID, acceptedInputsDepartmentID, "Invalid input. Please type a valid department ID: ");
        for (int i = 0; i < departmentInstancesVector.size(); i++)
        {
            if (departmentInstancesVector[i]->getID() == departmentID)
            {
                departmentInstancesVector.erase(departmentInstancesVector.begin() + i);
                acceptedInputsDepartmentID.clear();
                saveData(); // Saves data after deleting department
            }
        }
    }
    else
    {
        cout << "There are currently no existing departments in the college.\nPlease return to main menu.\n";
        system("pause");
        menuState = MAIN_MENU;
    }
}

void modifyDepartment()
{
    vector<int> acceptedInputsDepartmentID = {};
    int departmentIDChoice = -1;
    int addRemoveChangeChoice = -1;
    vector<int> acceptedInputsRemovePersonID = {};
    vector<int> everyPersonID = {};
    vector<int> acceptedInputsTeachersID = {};
    int personIDChoice = -1;
    int newDeanID = -1;
    string deptNewName;
    string completionMessage = "";


    displayPageTitle("Modify Department");
    // modifyDepartment options
    for (Department* d : departmentInstancesVector)
    {
        acceptedInputsDepartmentID.push_back(d->getID());
    }
    for (Teacher* t : teacherInstancesVector)
    {
        everyPersonID.push_back(t->getID());
    }
    for (Staff* s : staffInstancesVector)
    {
        everyPersonID.push_back(s->getID());
    }

    if (departmentInstancesVector.size() == 0)
    {
        cout << "The college currently has no departments to modify.\nReturn to main menu.\n";
        system("pause");
        menuState = MAIN_MENU;
    }
    else
    {
        displayListDepartments();
        displayUserPrompt("Please type the ID of the department you wish to modify : ");
        getExpectedIntInput(&departmentIDChoice, acceptedInputsDepartmentID, "Invalid input. Please type a valid department ID: ");

        Department* d = getDepartmentById(departmentIDChoice);
        cout << endl << endl;

        displayPageTitle(d->getName());
        displayDepartmentListTeacher(*d);
        displayDepartmentListStaff(*d);
        cout << endl << "*********************************************\n";

        for (Teacher t : d->getTeacherList())
        {
            Teacher* tptr = getTeacherById(t.getID());
            acceptedInputsTeachersID.push_back(tptr->getID());
            acceptedInputsRemovePersonID.push_back(tptr->getID());
        }
        for (Staff s : d->getStaffList())
        {
            Staff* sptr = getStaffById(s.getID()); 
            acceptedInputsRemovePersonID.push_back(sptr->getID());
        }

        displayUserPrompt("Do you wish to:\n1--Add employee          2--Remove employee\n3--Change dean           4--Change name\n\nPlease type desired action: ");
        getExpectedIntInput(&addRemoveChangeChoice, { 1, 2, 3, 4 }, "Invalid option. Please choose to either add or remove a person (1 or 2):  ");
        switch (addRemoveChangeChoice)
        {
            case 1: // Choose a teacher | If teacher already in dept. return to menu | If not add to dept
                displayAllTeachers();
                displayAllStaff();
                displayUserPrompt("Please type the ID of the person you wish to add: ");
                getExpectedIntInput(&personIDChoice, everyPersonID, "This person doesn't exist. Please type a valid ID: ");
                if (getTeacherById(personIDChoice) != nullptr)
                {
                    completionMessage = "\n\nDepartment modified successfully!";
                    for (Teacher t : d->getTeacherList())
                    {
                        if (t.getID() == personIDChoice)
                        {
                            completionMessage = "\n\nThis teacher is already in this department.";
                            break;
                        }
                    }
                    d->addTeacher(*getTeacherById(personIDChoice));
                    break;
                }
                else 
                {
                    completionMessage = "\n\nDepartment modified successfully!";
                    for (Staff s : d->getStaffList())
                    {
                        if (s.getID() == personIDChoice)
                        {
                            completionMessage = "\n\nThis staff member is already in the department.";
                            break;
                        }
                    }
                    d->addStaff(*getStaffById(personIDChoice));
                    break;
                }

            case 2: // Choose a teacher | If teacher isnt in dept return to menu | If he is, remove
                displayUserPrompt("Please type the ID of the person you wish to remove: ");
                cin >> personIDChoice;

                completionMessage = "\n\nDepartment modified successfully!";
                if (getTeacherById(personIDChoice) != nullptr) completionMessage = d->removeTeacher(*getTeacherById(personIDChoice));
                else if (getStaffById(personIDChoice) != nullptr) d->removeStaff(*getStaffById(personIDChoice));
                else
                {
                    completionMessage = "\nThis person is not in this department.";
                    break;
                }
                break;

            case 3: // Choose a new teacher to be the dean. Teacher has to be in the department already
                cout << "\nCurrent dean: " << d->getDean().getName() << "(ID: " << d->getDean().getID() << ")\n";
                displayUserPrompt("Please type the ID of the teacher you wish to promote as dean: ");
                getExpectedIntInput(&personIDChoice, acceptedInputsTeachersID, "Cannot find a teacher corresponding to this ID. Please type valid ID: ");
                d->setDean(*getTeacherById(personIDChoice));

                completionMessage = "\n\nDepartment modified successfully!";
                break;

            case 4: // Change department name
                cout << "\nCurrent name: " << d->getName() << "\n\n";
                displayUserPrompt("Please type a new name for this department: ");
                getline(cin, deptNewName);
                d->setName(deptNewName);
                completionMessage = "\n\nDepartment modified successfully!";
                break;
        }
        saveData(); // Saves modification applied
        cout << completionMessage << "\nReturn to menu.\n";
        system("pause");
        menuState = MAIN_MENU;
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

    displayPageTitle("Create Person");
    // createPerson
    cout << "Please indicate the category of the person.\n\n";
    cout << "1--Teacher               2--Staff";
    displayAllTeachers();
    displayAllStaff();
    cout << endl << endl;
    displayUserPrompt("Please indicate your desired category: ");
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
        displayUserPrompt("Name: ");
        getline(cin, name);
        displayUserPrompt("\nNow please type the teacher's age: ");
        getExpectedIntInput(&age, { -1, 16, 125 }, "Accepted age range is from 16-125 years old: ");
        displayUserPrompt("\nNow please type the teacher's speciality (i.e. Math, English, etc.): ");
        getline(cin, speciality);
        cout << "\nNow please indicate the teacher's degree.\n1--Bachelor         2--Master         3--PhD\n";
        displayUserPrompt("\nDegree: ");
        getExpectedIntInput(&degreeChoice, { 1, 2, 3 }, "Incorrect input, please type a correct degree: ");
        displayUserPrompt("\nNow please type the teacher's weekly work hours (32hrs/week will assign the teacher as full-time): ");
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
            Teacher* t = new FullTime (name, age, speciality, degree);
            teacherInstancesVector.push_back(t);
        }
        else
        {
            Teacher* t = new PartTime(name, age, speciality, degree, hoursWorked);
            teacherInstancesVector.push_back(t);
        }
    }
    else if (categoryChoice == 2)
    {
        system("CLS");
        drawAppTitle();
        cout << "\n\n--Create Person-STAFF--\n" << endl;
        // createPerson options
        displayUserPrompt("Please write the full name of the staff person.");
        cout << endl << "\n*********************************************";
        cout << endl << endl;
        cout << "Name: ";
        getline(cin, name);
        displayUserPrompt("\nNow please type the staff person's age: ");
        getExpectedIntInput(&age, { -1, 16, 125 }, "Accepted age range is from 16-125 years old: ");
        displayUserPrompt("\nNow please type the staff person's duty: ");
        getline(cin, duty);
        displayUserPrompt("\nNow please type the staff person's workload: ");
        getExpectedIntInput(&workload, { -1, 3, 32 }, "Accepted workload ranges from 3-32 Hours/Week: ");

        Staff* s = new Staff(name, age, duty, workload);
        staffInstancesVector.push_back(s);
    }
    saveData();
    cout << "Person created successfully.\n";
    system("pause");
    menuState = MAIN_MENU;
}

void deletePerson()
{
    int personID = -1;
    vector<int> acceptedInputsPersonID = {};
    for (Teacher* t : teacherInstancesVector)
    {
        acceptedInputsPersonID.push_back(t->getID());
    }
    for (Staff* s : staffInstancesVector)
    {
        acceptedInputsPersonID.push_back(s->getID());
    }

    displayPageTitle("Delete Person");
    // deletePerson options
    if (teacherInstancesVector.size() == 0 && staffInstancesVector.size() == 0)
    {
        cout << "The college currently has no employee to modify.\nReturn to main menu.\n";
        system("pause");
        menuState = MAIN_MENU;
    }
    else
    {
        displayAllTeachers();
        displayAllStaff();
        displayUserPrompt("Please type the ID number of the person you wish to delete: ");
        getExpectedIntInput(&personID, acceptedInputsPersonID, "Invalid input. Please type a valid ID: ");
        for (int i = 0; i < teacherInstancesVector.size(); i++)
        {
            if (teacherInstancesVector[i]->getID() == personID)
            {
                teacherInstancesVector.erase(teacherInstancesVector.begin()+i);
                acceptedInputsPersonID.clear();
            }
        }
        for (int i = 0; i < staffInstancesVector.size(); i++)
        {
            if (staffInstancesVector[i]->getID() == personID)
            {
                staffInstancesVector.erase(staffInstancesVector.begin() + i);
                acceptedInputsPersonID.clear();
            }
        }
        saveData(); // Saves data after deleting person
    }
}

void modifyPerson()
{
    int categoryChoice = -1;
    vector<int> categoryOptions = { 1, 2 };
    int teacherID = -1;
    int staffID = -1;
    int newHoursWorked = -1;
    int newWorkload = -1;
    int newDegreeChoice = -1;
    int newAge = -1;
    string newDegree;
    string newName;
    string newSpeciality;
    string newDuty;

    Teacher* modifiedTeacher;
    Staff* modifiedStaff;

    vector<int> acceptedInputsTeacherID = {};
    for (Teacher* t : teacherInstancesVector)
    {
        acceptedInputsTeacherID.push_back(t->getID());
    }

    vector<int> acceptedInputsStaffID = {};
    for (Staff* s : staffInstancesVector)
    {
        acceptedInputsStaffID.push_back(s->getID());
    }

    displayPageTitle("Modify Person");
    // createPerson
    cout << "Please indicate the category of the person you wish to modify.\n";
    cout << "1--Teacher               2--Staff";
    cout << endl << "\n*********************************************\n\n";
    cout << endl << endl;

    if (teacherInstancesVector.size() == 0 && staffInstancesVector.size() == 0)
    {
        cout << "The college currently has no employee to modify.\nReturn to main menu.\n";
        system("pause");
        menuState = MAIN_MENU;
    }
    else
    {
        if (teacherInstancesVector.size() == 0) categoryOptions = { 2 };
        else if (staffInstancesVector.size() == 0) categoryOptions = { 1 };
        displayUserPrompt("Please indicate your desired category: ");
        getExpectedIntInput(&categoryChoice, categoryOptions, "The Category you tried to modify is either empty or non-existent: ");

        vector<int> acceptedInputs;
        int propertyInput = -1;
        switch (categoryChoice)
        {
        case 1:

            displayAllTeachers();
            displayUserPrompt("\nPlease type the ID number of the teacher you wish to modify: ");
            getExpectedIntInput(&teacherID, acceptedInputsTeacherID, "Invalid input. Please type a valid teacher ID: ");
            modifiedTeacher = getTeacherById(teacherID);
            displayTeacherInformation(*modifiedTeacher);
            if (modifiedTeacher->getType() == "full-time") acceptedInputs = { 1, 2, 3, 4 };
            else acceptedInputs = { 1, 2, 3, 4, 5 };

            displayUserPrompt("\nPlease type the property you wish to modify: ");
            getExpectedIntInput(&propertyInput, acceptedInputs, "Invalid input. Please type a valid and modifiable property: ");
            switch (propertyInput)
            {
                case 1:
                    displayUserPrompt("\nPlease type the desired new name for " + modifiedTeacher->getName() + ": ");
                    getline(cin, newName);
                    modifiedTeacher->setName(newName);
                    break;
                case 2:
                    displayUserPrompt("\nPlease type the desired new age for " + modifiedTeacher->getName() + ": ");
                    getExpectedIntInput(&newAge, { -1, 16, 125 }, "Accepted age range is from 16-125 years old: ");
                    modifiedTeacher->setAge(newAge);
                    break;
                case 3:
                    displayUserPrompt("\nPlease type the desired new speciality for " + modifiedTeacher->getName() + ": ");
                    getline(cin, newSpeciality);
                    modifiedTeacher->setSpeciality(newSpeciality);
                    break;
                case 4:
                    displayUserPrompt("\nPlease type the desired new degree for " + modifiedTeacher->getName() + ".\n1--Bachelor         2--Master         3--PhD\n\nDegree: ");
                    getExpectedIntInput(&newDegreeChoice, { 1, 2, 3 }, "Incorrect input, please type a correct degree: ");
                    switch (newDegreeChoice)
                    {
                    case 1:
                        newDegree = "Bachelor";
                        break;
                    case 2:
                        newDegree = "Master";
                        break;
                    case 3:
                        newDegree = "PhD";
                        break;
                    }
                    modifiedTeacher->setDegree(newDegree);
                    break;
                case 5:
                    displayUserPrompt("\nPlease type the desired new work hours for " + modifiedTeacher->getName() + " (3-31 Hours/Week): ");
                    getExpectedIntInput(&newDegreeChoice, { -1, 3, 31 }, "Incorrect input, please type valid work hours: ");
                    modifiedTeacher->setHoursWorked(newDegreeChoice);
                    break;
            }
            cout << "\nTeacher modified successfully. \n\n";
            system("pause");
            menuState = MAIN_MENU;
            break;
        case 2:
            displayAllStaff();
            displayUserPrompt("\nPlease type the ID number of the staff person you wish to modify: ");
            getExpectedIntInput(&staffID, acceptedInputsStaffID, "Invalid input. Please type a valid staff person ID: ");
            modifiedStaff = getStaffById(staffID);
            displayStaffInformation(*modifiedStaff);
            acceptedInputs = { 1, 2, 3, 4 };

            displayUserPrompt("\nPlease type the property you wish to modify: ");
            getExpectedIntInput(&propertyInput, acceptedInputs, "Invalid input. Please type a valid and modifiable property: ");
            switch (propertyInput)
            {
            case 1:
                displayUserPrompt("\nPlease type the desired new name for " + modifiedStaff->getName() + ": ");
                getline(cin, newName);
                modifiedStaff->setName(newName);
                break;
            case 2:
                displayUserPrompt("\nPlease type the desired new age for " + modifiedStaff->getName() + ": ");
                getExpectedIntInput(&newAge, { -1, 16, 125 }, "Accepted age range is from 16-125 years old: ");
                modifiedStaff->setAge(newAge);
                break;
            case 3:
                displayUserPrompt("\nPlease type the desired new spociality for " + modifiedStaff->getName() + ": ");
                getline(cin, newDuty);
                modifiedStaff->setDuty(newDuty);
                break;
            case 4:
                displayUserPrompt("\nPlease type the desired new workload for " + modifiedStaff->getName() + ": ");
                getExpectedIntInput(&newWorkload, { -1, 3, 32 }, "Accepted workload ranges from 3-32 Hours/Week: ");
                modifiedStaff->setWorkload(newWorkload);
                break;
            }
            cout << "\nStaff modified successfully. \n\n";
            system("pause");
            menuState = MAIN_MENU;
            break;
        }
        saveData(); // Saves data after modifying person
    }

}

void viewDepartments()
{
    float totalPayroll = 0;
    int i = 1;
    int j;

    displayPageTitle("Departments");
    for (Department* d : departmentInstancesVector)
    {
        cout << i << ". " << d->getName() << endl;
        cout << "       Teachers:" << endl;
        vector<Teacher> deptTeacherVector = d->getTeacherList();
        j = 1;
        for (Teacher t : d->getTeacherList())
        {
            Teacher* tptr = getTeacherById(t.getID());
            cout << "           " << j << ". " << tptr->getName() << endl;
            j++;
        }
        cout << "       Staff:" << endl;
        vector<Staff> deptStaffVector = d->getStaffList();
        j = 1;
        for (Staff s : d->getStaffList())
        {
            Staff* sptr = getStaffById(s.getID());
            cout << "           " << j << ". " << sptr->getName() << endl;
            j++;
        }
        i++;
    }

    cout << "\nCollege employees average age: " << getPersonnelAvgAge() << " years old.";
    cout << "\n\nReturn to menu.\n";
    system("pause");
    menuState = MAIN_MENU;

}

void payrolls()
{
    float totalPayroll = 0;

    displayPageTitle("Payrolls");
    displayAllTeachers(true);
    displayAllStaff(true);

    for (Teacher* t : teacherInstancesVector)
    {
        totalPayroll += t->ComputePayRoll();
    }
    for (Staff* s : staffInstancesVector)
    {
        totalPayroll += s->ComputePayRoll();
    }

    cout << "\nTotal payrolls: " << fixed << setprecision(2) << totalPayroll << "$ CAD.";
    cout << "\n\nReturn to menu.\n";
    system("pause");
    menuState = MAIN_MENU;
}

void settingsMenu()
{
    int choice = -1;
    int confirmClear = -1;
    MENU_STATE nextMenuState = MAIN_MENU;

    system("CLS");
    drawAppTitle();
    displayPageTitle("Settings");
    // Settings options
    cout << "1--Save data" << endl;
    cout << "2--Clear data" << endl;
    cout << "3--Return to main menu" << endl;
    displayUserPrompt("Please type your desired action. ");
    getExpectedIntInput(&choice, { 1, 2, 3 }, "Not a valid option. (1 - 3): ");
    switch (choice)
    {
    case 1:
        cout << saveData();
        break;
    case 2:
        displayUserPrompt("Are you sure you want to permanently delete the data files? (1-yes / 2-no): ");
        getExpectedIntInput(&confirmClear, { 1, 2 }, "Please choose a valid option. ");
        switch (confirmClear)
        {
        case 1:
            cout << clearData();
            loadData();
            break;
        case 2:
            nextMenuState = SETTINGS;
            cout << "Data file deletion aborted. ";
            break;
        }
    case 3:
        break;
    }
    system("pause");
    menuState = nextMenuState;
}

void closeApp()
{
    system("CLS");
    saveData(); // Saves data upon closing application

    cout << "Thank you for using HR Management\n\n";

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
            case VIEW_DEPARTMENTS:
                viewDepartments();
                break;
            case PAYROLLS:
                payrolls();
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

