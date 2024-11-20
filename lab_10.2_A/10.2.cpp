#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <Windows.h>

using namespace std;

enum Specialization { COMPUTER_SCIENCE, INFORMATICS, MATH_ECONOMICS, PHYSICS_INFORMATICS, LABOR_EDUCATION };
string specializationStr[] = { "Комп'ютерні науки", "Інформатика", "Математика та економіка", "Фізика та інформатика", "Трудове навчання" };

struct Student {
    string surname;
    int course; // Курс студента
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
    cout << "Введіть кількість студентів: ";
    cin >> numStudents;

    Student* students = new Student[numStudents];
    int* indexArray = new int[numStudents];

    int menuItem;
    do {
        cout << "\nМеню:\n";
        cout << "1 - Введення даних студентів\n";
        cout << "2 - Вивід даних студентів\n";
        cout << "3 - Фізичне сортування студентів\n";
        cout << "4 - Індексне сортування студентів\n";
        cout << "5 - Пошук студента\n";
        cout << "0 - Вихід\n";
        cout << "Виберіть дію: ";
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
            cout << "Список студентів фізично відсортовано.\n";
            break;
        case 4:
            IndexSort(students, numStudents, indexArray);
            cout << "Список студентів індексно відсортовано.\n";
            break;
        case 5: {
            string surname;
            int profileGrade;
            double avgGrade;
            cout << "Введіть прізвище для пошуку: ";
            cin >> surname;
            cout << "Введіть оцінку з інформатики для пошуку: ";
            cin >> profileGrade;
            cout << "Введіть середній бал для пошуку: ";
            cin >> avgGrade;

            int result = BinarySearch(students, indexArray, numStudents, surname, profileGrade, avgGrade);
            if (result != -1)
                cout << "Студент знайдений за індексом: " << result << "\n";
            else
                cout << "Студент не знайдений.\n";
            break;
        }
        case 0:
            break;
        default:
            cout << "Неправильний вибір. Спробуйте ще раз.\n";
        }
    } while (menuItem != 0);

    delete[] students;
    delete[] indexArray;
    return 0;
}

void InputStudents(Student* students, int N) {
    int spec;
    for (int i = 0; i < N; i++) {
        cout << "Студент № " << i + 1 << ":\n";
        cin.ignore();
        cout << " Прізвище: "; getline(cin, students[i].surname);
        cout << " Курс: "; cin >> students[i].course;
        cout << " Спеціальність (0 - Комп'ютерні науки, 1 - Інформатика, 2 - Математика та економіка, 3 - Фізика та інформатика, 4 - Трудове навчання): ";
        cin >> spec;
        students[i].specialization = static_cast<Specialization>(spec);
        cout << " Оцінка з фізики: "; cin >> students[i].physics;
        cout << " Оцінка з математики: "; cin >> students[i].mathematics;
        cout << " Оцінка з інформатики: "; cin >> students[i].informatics;
    }
}

void PrintStudents(const Student* students, int N) {
    cout << "===================================================================================================\n";
    cout << "| № | Прізвище           | Курс | Спеціальність           | Фізика | Математика | Інформатика |\n";
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
