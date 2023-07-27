#include <iostream>
#include <string>
#include <memory>
#include <map>
#include <vector>
#include <algorithm>
#include <fstream>

using namespace std;

class Course
{
private:
    string course_title;
    string course_description;
    string course_instructor;
    int course_number;

public:
    Course() : course_title(""), course_description(""), course_instructor(""), course_number(0) {}

    Course(const string &title, const string &description, const string &instructor, int number)
    {
        course_title = title;
        course_description = description;
        course_instructor = instructor;
        course_number = number;
    }

    int getCourseNumber() const { return course_number; }
    const string &getCourseTitle() const { return course_title; }
    const string &getDescription() const { return course_description; }
    const string &getInstructor() const { return course_instructor; }

    ~Course() {}
};

class Employee
{
private:
    string employee_name;
    string position;
    string department;
    int employee_id;
    int performance;

public:
    Employee() : employee_name(""), position(""), department(""), employee_id(0), performance(0) {}

    Employee(const string &name, const string &pos, const string &dept, int id, int perf)
    {
        employee_name = name;
        position = pos;
        department = dept;
        employee_id = id;
        performance = perf;
    }

    int getEmployeeId() const { return employee_id; }
    const string &getEmployeeName() const { return employee_name; }
    const string &getPosition() const { return position; }
    const string &getDepartment() const { return department; }
    int getPerformance() const { return performance; }
    void setPerformance(int perf) { performance = perf; }

    ~Employee() {}
};

class Enrollment
{
private:
    const Employee *employee;
    const Course *enrolled_course;

public:
    Enrollment() : employee(nullptr), enrolled_course(nullptr) {}

    Enrollment(const Employee &emp, const Course &crs) : employee(&emp), enrolled_course(&crs) {}

    const Employee &getEmployee() const { return *employee; }
    const Course &getEnrolledCourse() const { return *enrolled_course; }

    ~Enrollment() {}
};

class EnrollmentManager
{
private:
    map<int, Enrollment> enrollments;

public:
    void enrollEmployee(const Employee &employee, const Course &course)
    {
        Enrollment newEnrollment(employee, course);
        enrollments[employee.getEmployeeId()] = newEnrollment;
        cout << "Enrollment successful!" << endl;
    }

    Enrollment *getEnrollment(int employeeId)
    {
        auto it = enrollments.find(employeeId);
        return (it != enrollments.end()) ? &it->second : nullptr;
    }

    map<int, Enrollment> &getEnrollments()
    {
        return enrollments;
    }

    ~EnrollmentManager() {}
};

template <typename T>
class Report
{
public:
    void generateReport(EnrollmentManager &enrollmentManager)
    {

        if constexpr (std::is_same_v<T, Employee>)
        {

            for (const auto &[employeeId, enrollment] : enrollmentManager.getEnrollments())
            {
                const Employee &employee = enrollment.getEmployee();
                const Course &course = enrollment.getEnrolledCourse();

                std::cout << "---------------------\n";

                std::cout << "Employee Name: " << employee.getEmployeeName() << std::endl;
                std::cout << "Position: " << employee.getPosition() << std::endl;
                std::cout << "Department: " << employee.getDepartment() << std::endl;
                std::cout << "Employee ID: " << employee.getEmployeeId() << std::endl;
                std::cout << "Course Title: " << course.getCourseTitle() << std::endl;
                std::cout << "Description: " << course.getDescription() << std::endl;
                std::cout << "Instructor: " << course.getInstructor() << std::endl;
                std::cout << "Course Number: " << course.getCourseNumber() << std::endl;
                std::cout << "Performance: " << employee.getPerformance() << std::endl;

                std::cout << "---------------------\n";
            }
        }
        else
        {
            std::cout << "Report generation not supported for this type." << std::endl;
        }
    }
};

class TrainingManager
{
private:
    vector<unique_ptr<Course>> courses;
    vector<unique_ptr<Employee>> employees;
    EnrollmentManager enrollmentManager;

public:
    void addCourse(const Course &course)
    {
        courses.push_back(make_unique<Course>(course));
    }

    void addEmployee(const Employee &employee)
    {
        employees.push_back(make_unique<Employee>(employee));
    }

    void enrollEmployeeInCourse(const Employee &employee, const Course &course)
    {
        enrollmentManager.enrollEmployee(employee, course);
    }

    EnrollmentManager &getEnrollmentManager()
    {
        return enrollmentManager;
    }

    vector<unique_ptr<Course>> &getCourses()
    {
        return courses;
    }

    vector<unique_ptr<Employee>> &getEmployees()
    {
        return employees;
    }
};

Employee *findEmployeeById(vector<unique_ptr<Employee>> &employees, int employeeId)
{
    for (auto &employee : employees)
    {
        if (employee->getEmployeeId() == employeeId)
        {
            return employee.get();
        }
    }
    return nullptr;
}

Course *findCourseByNumber(vector<unique_ptr<Course>> &courses, int courseNumber)
{
    for (auto &course : courses)
    {
        if (course->getCourseNumber() == courseNumber)
        {
            return course.get();
        }
    }
    return nullptr;
}

class Logger
{
private:
    std::ofstream logFile;
    static Logger *instance;

    Logger()
    {
        logFile.open("log.txt");
    }

public:
    static Logger &getInstance()
    {
        if (!instance)
        {
            instance = new Logger();
        }
        return *instance;
    }

    void log(const std::string &message)
    {
        logFile << message << std::endl;
    }

    ~Logger()
    {
        logFile.close();
    }
};

Logger *Logger::instance = nullptr;

int main()
{
    TrainingManager trainingManager;

    Logger &logger = Logger::getInstance();

    int numCourses;
    cout << "Enter the number of courses: ";
    cin >> numCourses;
    cin.ignore();

    for (int i = 0; i < numCourses; i++)
    {
        string title, description, instructor;
        int number;

        cout << "Enter details for Course " << i + 1 << ":" << endl;
        cout << "Course Title: ";
        getline(cin, title);

        cout << "Description: ";
        getline(cin, description);

        cout << "Instructor: ";
        getline(cin, instructor);

        cout << "Course Number: ";
        cin >> number;

        cin.ignore();

        trainingManager.addCourse(Course(title, description, instructor, number));
    }

    int numEmployees;
    cout << "\nEnter the number of employees: ";
    cin >> numEmployees;
    cin.ignore();
    for (int i = 0; i < numEmployees; i++)
    {
        string name, pos, dept;
        int id, performance;

        cout << "Enter details for Employee " << i + 1 << ":" << endl;
        cout << "Employee Name: ";
        getline(cin, name);

        cout << "Position: ";
        getline(cin, pos);

        cout << "Department: ";
        getline(cin, dept);

        cout << "Employee ID: ";
        cin >> id;

        cout << "Employee Performance: ";
        cin >> performance;

        cin.ignore();

        trainingManager.addEmployee(Employee(name, pos, dept, id, performance));
    }

    int choice;
    do
    {
        cout << "\nMenu:" << endl;
        cout << "1. Add Course" << endl;
        cout << "2. Add Employee" << endl;
        cout << "3. Remove Course" << endl;
        cout << "4. Remove Employee" << endl;
        cout << "5. Enroll Employee" << endl;
        cout << "6. View Enrollment Details" << endl;
        cout << "7. Generate Report for employee performance" << endl;
        cout << "8. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
        {
            // Add Course
            string title, description, instructor;
            int number;

            cout << "Enter details for the new Course:" << endl;
            cin.ignore();
            cout << "Course Title: ";
            getline(cin, title);

            cout << "Description: ";
            getline(cin, description);

            cout << "Instructor: ";
            getline(cin, instructor);

            cout << "Course Number: ";
            cin >> number;

            cin.ignore();

            trainingManager.addCourse(Course(title, description, instructor, number));

            cout << "Course added successfully!" << endl;
            break;
        }
        case 2:
        {
            // Add Employee
            string name, pos, dept;
            int id, performance;

            cout << "Enter details for the new Employee:" << endl;
            cin.ignore();
            cout << "Employee Name: ";
            getline(cin, name);

            cout << "Position: ";
            getline(cin, pos);

            cout << "Department: ";
            getline(cin, dept);

            cout << "Employee ID: ";
            cin >> id;

            cout << "Enter Employee Performance: ";
            cin >> performance;
            cin.ignore(); // Clear the newline from the previous input
            trainingManager.addEmployee(Employee(name, pos, dept, id, performance));
            cout << "Employee added successfully!" << endl;
            break;
        }
        case 3:
        {
            // Remove Course
            int courseNumber;
            cout << "Enter the Course Number to remove: ";
            cin >> courseNumber;

            Course *foundCourse = findCourseByNumber(trainingManager.getCourses(), courseNumber);
            if (foundCourse)
            {

                trainingManager.getCourses().erase(remove_if(trainingManager.getCourses().begin(), trainingManager.getCourses().end(), [&](const unique_ptr<Course> &c)
                                                             { return c.get() == foundCourse; }),
                                                   trainingManager.getCourses().end());

                cout << "Course removed successfully!" << endl;
            }
            else
            {
                cout << "Course not found. Please check the Course Number." << endl;
            }
            break;
        }
        case 4:
        {
            // Remove Employee
            int employeeId;
            cout << "Enter the Employee ID to remove: ";
            cin >> employeeId;

            Employee *foundEmployee = findEmployeeById(trainingManager.getEmployees(), employeeId);
            if (foundEmployee)
            {

                trainingManager.getEmployees().erase(remove_if(trainingManager.getEmployees().begin(), trainingManager.getEmployees().end(), [&](const unique_ptr<Employee> &e)
                                                               { return e.get() == foundEmployee; }),
                                                     trainingManager.getEmployees().end());

                cout << "Employee removed successfully!" << endl;
            }
            else
            {
                cout << "Employee not found. Please check the Employee ID." << endl;
            }
            break;
        }
        case 5:
        {
            // Enroll employee
            int employeeId, courseNumber, performance;

            cout << "Enter Employee ID to enroll: ";
            cin >> employeeId;

            cout << "Enter Course Number to enroll: ";
            cin >> courseNumber;

            cout << "Enter Employee Performance: ";
            cin >> performance;

            Employee *foundEmployee = findEmployeeById(trainingManager.getEmployees(), employeeId);
            Course *foundCourse = findCourseByNumber(trainingManager.getCourses(), courseNumber);

            if (foundEmployee && foundCourse)
            {
                trainingManager.enrollEmployeeInCourse(*foundEmployee, *foundCourse);
                foundEmployee->setPerformance(performance);
            }
            else
            {
                cout << "Employee or Course not found. Please check the provided IDs." << endl;
            }
            break;
        }
        case 6:
        {
            // View enrollment details
            int employeeIdToView;
            cout << "Enter the Employee ID to view enrollment details: ";
            cin >> employeeIdToView;

            Enrollment *foundEnrollment = trainingManager.getEnrollmentManager().getEnrollment(employeeIdToView);
            if (foundEnrollment)
            {

                cout << "\nEnrollment Details:" << endl;
                cout << "Employee Name: " << foundEnrollment->getEmployee().getEmployeeName() << endl;
                cout << "Course Title: " << foundEnrollment->getEnrolledCourse().getCourseTitle() << endl;
                cout << "Description: " << foundEnrollment->getEnrolledCourse().getDescription() << endl;
                cout << "Instructor: " << foundEnrollment->getEnrolledCourse().getInstructor() << endl;
                cout << "Course Number: " << foundEnrollment->getEnrolledCourse().getCourseNumber() << endl;
                cout << "Employee ID: " << foundEnrollment->getEmployee().getEmployeeId() << endl;
            }
            else
            {
                cout << "\nEnrollment not found. Please check the Employee ID." << endl;
            }
            break;
        }
        case 7:
        {
            // Generate Employee-wise Report
            Report<Employee> empReport;
            empReport.generateReport(trainingManager.getEnrollmentManager());
            break;
        }
        case 8:
            cout << "Exiting..." << endl;
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
            break;
        }
    } while (choice != 8);

    return 0;
}