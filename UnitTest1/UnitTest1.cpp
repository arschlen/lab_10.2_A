#include "pch.h"
#include "CppUnitTest.h"
#include <string>
#include <algorithm>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

// Копіюємо необхідні частини коду з основного файла
enum Specialization { COMPUTER_SCIENCE, INFORMATICS, MATH_ECONOMICS, PHYSICS_INFORMATICS, LABOR_EDUCATION };

struct Student {
    string surname;
    int course;
    Specialization specialization;
    int physics;
    int mathematics;
    int informatics;
};

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

// Тести
namespace UnitTest1
{
    TEST_CLASS(UnitTest1)
    {
    public:

        TEST_METHOD(TestCalculateAverageGrade)
        {
            Student student = { "Ivanov", 2, COMPUTER_SCIENCE, 90, 85, 80 };
            Assert::AreEqual(85.0, CalculateAverageGrade(student), 0.001, L"Average grade calculation is incorrect.");
        }

        TEST_METHOD(TestBinarySearch)
        {
            Student students[3] = {
                {"Ivanov", 2, COMPUTER_SCIENCE, 90, 85, 80},
                {"Petrov", 1, INFORMATICS, 70, 75, 80},
                {"Sidorov", 3, MATH_ECONOMICS, 80, 90, 85}
            };
            int indexArray[3];
            IndexSort(students, 3, indexArray);

            int result = BinarySearch(students, indexArray, 3, "Sidorov", 85, 85.0);
            Assert::AreEqual(2, result, L"Binary search failed.");
        }
    };
}
