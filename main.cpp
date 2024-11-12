#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class Problem {
public:
    // Constructors
    Problem(string& name, string& speciatly, int& duration);

    // Getters
    inline string getName() const { return this->name; }
    inline string getSpecialty() const { return this->specialty; }
    inline int getDuration() const { return this->duration; }
private:
    string name;
    string specialty;
    int duration;
};

// Constructor with parameters
Problem::Problem(string& name, string& speciatly, int& duration) :
    name(name), specialty(speciatly), duration(duration) {};

class Doctor {
public:
    // Constructors
    Doctor(string& name, string& specialty, int hours_left);

    // Getters
    inline static int getMaxHours() { return max_hours; }

    inline string getName() const { return this->name; }
    inline string getSpecialty() const { return this->specialty; }
    inline int getHoursLeft() const { return this->hours_left; }
    inline const vector<Problem>* getResolvedProblems() const { return &this->resolved_problems; }

    // Setters
    inline void setHoursLeft(const int& hours_left) { this->hours_left = hours_left; }

    // Methods
    inline void addProblem(const Problem& problem) { resolved_problems.push_back(problem); }

private:
    static const int max_hours = 8;

    string name;
    string specialty;
    int hours_left;
    vector<Problem> resolved_problems;
};

// Constructor with parameters
Doctor::Doctor(string& name, string& specialty, int hours_left = max_hours) :
    name(name), specialty(specialty), hours_left(hours_left), resolved_problems({}) {};

int main() {
    vector<Problem> problems;
    vector<Doctor> doctors;

    ifstream inFile("input4_bonus.txt");

    int no_problems, no_doctors, duration;
    string name, specialty;

    // Read problems
    inFile >> no_problems;
    for (int i = 0; i < no_problems; i++) {
        inFile >> name;
        inFile >> specialty;
        inFile >> duration;
        Problem* new_problem = new Problem(name, specialty, duration);
        problems.push_back(*new_problem);
        delete new_problem;
    }

    // Read doctors
    inFile >> no_doctors;
    for (int i = 0; i < no_doctors; i++) {
        inFile >> name;
        inFile >> specialty;
        Doctor* new_doctor = new Doctor(name, specialty);
        doctors.push_back(*new_doctor);
        delete new_doctor;
    }

    // For each problem
    for (auto& problem : problems) {
        // Try to find the appropiate doctor that has enough time
        auto found_doctor = find_if(doctors.begin(), doctors.end(), [=](const Doctor& doctor) {
            return problem.getSpecialty() == doctor.getSpecialty() && doctor.getHoursLeft() >= problem.getDuration();
            });

        // If doctor is found
        if (found_doctor != doctors.end()) {
            // Add problem to the doctor's list
            found_doctor->addProblem(problem);

            // Decrease remaining time of the doctor
            found_doctor->setHoursLeft(found_doctor->getHoursLeft() - problem.getDuration());
        }
    }

    // For each doctor
    for (auto& doctor : doctors) {
        const vector<Problem>* current_problems = doctor.getResolvedProblems();
        // If the doctor has problems
        if (!current_problems->empty()) {
            cout << doctor.getName() << ' ' << current_problems->size() << ' ';
            
            // Print each problem
            for (auto& problem : *current_problems) {
                cout << problem.getName() << ' ';
            }
            cout << '\n';
        }

        // Delete the pointer to the current doctor's problems
        current_problems = nullptr;
        delete current_problems;
    }

    return 0;
}
