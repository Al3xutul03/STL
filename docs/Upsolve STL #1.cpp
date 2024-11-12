#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct Problem
{
    string name;
    string speciality;
};

struct Doctor
{
    string id;
    string speciality;
};

int main()
{
    ifstream inFile("input.txt");

    int n, m;
    inFile >> n;

    vector<Problem> problems(n);
    for (Problem &p : problems)
    {
        inFile >> p.name >> p.speciality;
    }

    inFile >> m;
    vector<Doctor> doctors(m);
    for (Doctor &d : doctors)
    {
        inFile >> d.id >> d.speciality;
    }

    for (const Problem &p : problems)
    {
        bool found = false;
        for (const Doctor &d : doctors)
        {
            if (d.speciality == p.speciality)
            {
                found = true;
                break;
            }
        }

        if (found)
            cout << p.name << " Acceptat" << endl;
        else
            cout << p.name << " Respins" << endl;
    }

    return 0;
}
