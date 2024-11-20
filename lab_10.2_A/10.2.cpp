#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <Windows.h>

using namespace std;

enum Specialization { COMPUTER_SCIENCE, INFORMATICS, MATH_ECONOMICS, PHYSICS_INFORMATICS, LABOR_EDUCATION };
string specializationStr[] = { "����'����� �����", "�����������", "���������� �� ��������", "Գ���� �� �����������", "������� ��������" };

struct Student {
    string surname;
    int course; // ���� ��������
    Specialization specialization;
    int physics;
    int mathematics;
    int informatics;
};

void InputStudents(Student* students, int N);
void PrintStudents(const Student* students, int N);
double CalculateAverageGrade(const Student& student);
void PhysicalSort(Student* students, int N);
void IndexSort(const Student* students, int N, int* indexArray);
int BinarySearch(const Student* students, const int* indexArray, int N, const string& surname, int profileGrade, double avgGrade);

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    int numStudents;
    cout << "������ ������� ��������: ";
    cin >> numStudents;

    Student* students = new Student[numStudents];
    int* indexArray = new int[numStudents];

    int menuItem;
    do {
        cout << "\n����:\n";
        cout << "1 - �������� ����� ��������\n";
        cout << "2 - ���� ����� ��������\n";
        cout << "3 - Գ����� ���������� ��������\n";
        cout << "4 - �������� ���������� ��������\n";
        cout << "5 - ����� ��������\n";
        cout << "0 - �����\n";
        cout << "������� ��: ";
        cin >> menuItem;

        switch (menuItem) {
        case 1:
            InputStudents(students, numStudents);
            break;
        case 2:
            PrintStudents(students, numStudents);
            break;
        case 3:
            PhysicalSort(students, numStudents);
            cout << "������ �������� ������� �����������.\n";
            break;
        case 4:
            IndexSort(students, numStudents, indexArray);
            cout << "������ �������� �������� �����������.\n";
            break;
        case 5: {
            string surname;
            int profileGrade;
            double avgGrade;
            cout << "������ ������� ��� ������: ";
            cin >> surname;
            cout << "������ ������ � ����������� ��� ������: ";
            cin >> profileGrade;
            cout << "������ ������� ��� ��� ������: ";
            cin >> avgGrade;

            int result = BinarySearch(students, indexArray, numStudents, surname, profileGrade, avgGrade);
            if (result != -1)
                cout << "������� ��������� �� ��������: " << result << "\n";
            else
                cout << "������� �� ���������.\n";
            break;
        }
        case 0:
            break;
        default:
            cout << "������������ ����. ��������� �� ���.\n";
        }
    } while (menuItem != 0);

    delete[] students;
    delete[] indexArray;
    return 0;
}

void InputStudents(Student* students, int N) {
    int spec;
    for (int i = 0; i < N; i++) {
        cout << "������� � " << i + 1 << ":\n";
        cin.ignore();
        cout << " �������: "; getline(cin, students[i].surname);
        cout << " ����: "; cin >> students[i].course;
        cout << " ������������ (0 - ����'����� �����, 1 - �����������, 2 - ���������� �� ��������, 3 - Գ���� �� �����������, 4 - ������� ��������): ";
        cin >> spec;
        students[i].specialization = static_cast<Specialization>(spec);
        cout << " ������ � ������: "; cin >> students[i].physics;
        cout << " ������ � ����������: "; cin >> students[i].mathematics;
        cout << " ������ � �����������: "; cin >> students[i].informatics;
    }
}

void PrintStudents(const Student* students, int N) {
    cout << "===================================================================================================\n";
    cout << "| � | �������           | ���� | ������������           | Գ���� | ���������� | ����������� |\n";
    cout << "---------------------------------------------------------------------------------------------------\n";
    for (int i = 0; i < N; i++) {
        cout << "| " << setw(2) << left << i + 1 << " | " << setw(18) << students[i].surname << " | " << setw(4) << students[i].course << " | ";
        cout << setw(21) << specializationStr[students[i].specialization] << " | " << setw(6) << students[i].physics << " | ";
        cout << setw(11) << students[i].mathematics << " | " << setw(11) << students[i].informatics << " |\n";
    }
    cout << "===================================================================================================\n";
}

double CalculateAverageGrade(const Student& student) {
    return (student.physics + student.mathematics + student.informatics) / 3.0;
}

void PhysicalSort(Student* students, int N) {
    sort(students, students + N, [](const Student& a, const Student& b) {
        if (a.informatics != b.informatics) return a.informatics > b.informatics;
        double avgA = CalculateAverageGrade(a);
        double avgB = CalculateAverageGrade(b);
        if (avgA != avgB) return avgA > avgB;
        return a.surname < b.surname;
        });
}

void IndexSort(const Student* students, int N, int* indexArray) {
    for (int i = 0; i < N; i++) {
        indexArray[i] = i;
    }
    sort(indexArray, indexArray + N, [&](int a, int b) {
        if (students[a].informatics != students[b].informatics) return students[a].informatics > students[b].informatics;
        double avgA = CalculateAverageGrade(students[a]);
        double avgB = CalculateAverageGrade(students[b]);
        if (avgA != avgB) return avgA > avgB;
        return students[a].surname < students[b].surname;
        });
}

int BinarySearch(const Student* students, const int* indexArray, int N, const string& surname, int profileGrade, double avgGrade) {
    int left = 0, right = N - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        int idx = indexArray[mid];
        const Student& student = students[idx];

        if (student.surname == surname && student.informatics == profileGrade && CalculateAverageGrade(student) == avgGrade) {
            return idx;
        }
        else if (student.informatics < profileGrade || (student.informatics == profileGrade && CalculateAverageGrade(student) < avgGrade) ||
            (student.informatics == profileGrade && CalculateAverageGrade(student) == avgGrade && student.surname > surname)) {
            right = mid - 1;
        }
        else {
            left = mid + 1;
        }
    }
    return -1;
}
