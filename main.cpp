#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

vector<pair<string, string>> problems, doctors;

int main() {
    ifstream inFile("input.txt");

    int no_problems, no_doctors;
    string name, speciality;

    inFile >> no_problems;

    for (int i = 0; i < no_problems; i++) {
        inFile >> name;
        inFile >> speciality;
        problems.push_back(make_pair(name, speciality));
    }

    inFile >> no_doctors;

    for (int i = 0; i < no_doctors; i++) {
        inFile >> name;
        inFile >> speciality;
        doctors.push_back(make_pair(name, speciality));
    }

    for (auto& problem : problems) {
        auto found_doctor = find_if(doctors.begin(), doctors.end(), [=](const pair<string, string> doctor) {
            return problem.second == doctor.second;
            });

        if (found_doctor != doctors.end()) {
            cout << found_doctor->first << ' ' << problem.first << '\n';
            doctors.erase(found_doctor);
        }
    }

    return 0;
}
