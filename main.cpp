#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>

using namespace std;

class Problem {
public:
    // Constructors
    Problem(const string& name, const string& speciatly, const int& arrival_hour, const int& duration, const int& severity_grade);

    // Getters
    inline string getName() const { return this->name; }
    inline string getSpecialty() const { return this->specialty; }
    inline int getArrivalHour() const { return this->arrival_hour; }
    inline int getDuration() const { return this->duration; }
    inline int getSeverity() const { return this->severity_grade; }

    // Operators
    bool operator<(const Problem& other) const;

private:
    string name;
    string specialty;
    int arrival_hour;
    int duration;
    int severity_grade;
};

// Constructor with parameters
Problem::Problem(const string& name, const string& speciatly, const int& arrival_hour, const int& duration, const int& severity_grade) :
    name(name), specialty(speciatly), arrival_hour(arrival_hour), duration(duration), severity_grade(severity_grade) {}

// Operator for prioritizing according to arrival hour and severity
bool Problem::operator<(const Problem& other) const {
    if (this->arrival_hour != other.arrival_hour)
        return this->arrival_hour > other.arrival_hour;

    return this->severity_grade < other.severity_grade;
}

class Doctor {
public:
    // Constructors
    Doctor(const string& name, const vector<string>& specialties, const int& hours_left);

    // Getters
    inline static int getMaxHours() { return max_hours; }

    inline string getName() const { return this->name; }
    inline int getHoursLeft() const { return this->hours_left; }
    inline const vector<Problem>* getResolvedProblems() const { return &this->resolved_problems; }

    // Setters
    inline void setHoursLeft(const int& hours_left) { this->hours_left = hours_left; }

    // Methods
    inline void addProblem(const Problem& problem);
    bool hasSpecialty(const string& specialty) const;
    bool isAvailable(const Problem& problem) const;

private:
    static const int max_hours = 8;

    string name;
    vector<string> specialties;
    int hours_left;
    vector<Problem> resolved_problems;
};

// Constructor with parameters
Doctor::Doctor(const string& name, const vector<string>& specialties, const int& hours_left = max_hours) :
    name(name), specialties(specialties), hours_left(hours_left), resolved_problems({}) {}

// Method for adding a problem to the doctor's list
void Doctor::addProblem(const Problem& problem) {
	this->resolved_problems.push_back(problem);

    // Decrease remaining time of the doctor
    int relative_arrival_time = problem.getArrivalHour() - 9;
    this->setHoursLeft(Doctor::getMaxHours() - relative_arrival_time - problem.getDuration());
}

// Method for confirming that a doctor has a certain specialty
bool Doctor::hasSpecialty(const string& specialty) const {
    for (const string& current_specialty : specialties)
        if (current_specialty == specialty)
            return true;

    return false;
}

// Method for confirming the availability of a doctor
bool Doctor::isAvailable(const Problem& problem) const {
    int relative_arrival_time = problem.getArrivalHour() - 9; // Time passed since working hours started
    int relative_worked_time = Doctor::max_hours - this->getHoursLeft(); // Time worked since working hours started

    // Check if the doctor is already busy
    if (relative_arrival_time < relative_worked_time)
        return false;

    // Check if the problem would take too much time to solve
    if (relative_arrival_time + problem.getDuration() > Doctor::max_hours)
        return false;

    return true;
}

// Static class for I/O handling
class IOHelper {
public:
    static void readInput(priority_queue<Problem>& problems, vector<Doctor>& doctors);
    static void writeOutput(vector<Doctor>& doctors);
};

// Method for reading the input
void IOHelper::readInput(priority_queue<Problem>& problems, vector<Doctor>& doctors) {
    ifstream inFile("input2.txt");

    string name, specialty;
    int no_problems, no_doctors, arrival_hour, duration, severity_grade, no_specialties;

    // Read problems
    inFile >> no_problems;
    for (int i = 0; i < no_problems; i++) {
        inFile >> name >> specialty >> arrival_hour >> duration >> severity_grade;

        Problem new_problem(name, specialty, arrival_hour, duration, severity_grade);
        problems.push(new_problem);
    }

    // Read doctors
    inFile >> no_doctors;
    for (int i = 0; i < no_doctors; i++) {
        inFile >> name;
        inFile >> no_specialties;

        vector<string> specialties;
        specialties.resize(no_specialties);
        for (string& current_specialty : specialties)
            inFile >> current_specialty;

        Doctor new_doctor(name, specialties);
        doctors.push_back(new_doctor);
    }
}

// Method for writing the output
void IOHelper::writeOutput(vector<Doctor>& doctors) {
    // For each doctor
    for (auto& doctor : doctors) {
        const vector<Problem>* current_problems = doctor.getResolvedProblems();
        // If the doctor has problems
        if (!current_problems->empty()) {
            cout << doctor.getName() << ' ' << current_problems->size() << ' ';

            // Print each problem
            for (auto& problem : *current_problems) {
                cout << problem.getName() << ' ' << problem.getArrivalHour() << ' ';
            }
            cout << '\n';
        }
    }
}

int main() {
    priority_queue<Problem> problems;
    vector<Doctor> doctors;

    IOHelper::readInput(problems, doctors);

    // For each problem
    while (!problems.empty()) {
        Problem problem = problems.top();
        problems.pop();

        // Try to find the appropiate doctor that has enough time
        auto found_doctor = find_if(doctors.begin(), doctors.end(), [=](const Doctor& doctor) {
            return doctor.hasSpecialty(problem.getSpecialty()) && doctor.isAvailable(problem);
            });

		bool isDoctorAvailable = found_doctor != doctors.end();
        if (isDoctorAvailable) {
            // Add problem to the doctor's list
            found_doctor->addProblem(problem);
        }
    }

    IOHelper::writeOutput(doctors);

    return 0;
}
