#ifndef DATABASE_H
#define DATABASE_H

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#endif

#include <string>
#include <vector>
#include <mutex>
#include <fstream>
#include <sstream>
#include <algorithm>

// ============================================================================
// SYSTEM CONFIGURATION
// ============================================================================
extern const std::string ADMIN_USERNAME;
extern const std::string ADMIN_PASSWORD;
extern const std::string ADMIN_COOKIE_NAME;
extern const std::string ADMIN_SESSION_TOKEN;
extern const int EXAM_DURATION_SECONDS_CFG;

// ============================================================================
// DATABASE SCHEMAS
// ============================================================================
struct Student {
    std::string rollNumber;
    std::string name;
    std::string password;
};

struct Question {
    int id;
    std::string text;
    std::string optionA;
    std::string optionB;
    std::string optionC;
    std::string optionD;
    char correctOption; // 'A', 'B', 'C', 'D'
    int marks;
};

struct Result {
    std::string rollNumber;
    std::string studentName;
    int score;
    int totalMarks;
    std::string timestamp;
};

// Helper split utility
inline std::vector<std::string> dbSplitString(const std::string& s, char delim) {
    std::vector<std::string> result;
    size_t start = 0;
    size_t end = s.find(delim);
    while (end != std::string::npos) {
        result.push_back(s.substr(start, end - start));
        start = end + 1;
        end = s.find(delim, start);
    }
    result.push_back(s.substr(start));
    return result;
}

// ============================================================================
// SQL-LIKE FILE DATABASE MANAGER (Thread-Safe)
// ============================================================================
class DBManager {
private:
    std::string dataDir = "data/";
    std::string studentsFile = dataDir + "students.txt";
    std::string questionsFile = dataDir + "questions.txt";
    std::string resultsFile = dataDir + "results.txt";
    
    std::vector<Student> students;
    std::vector<Question> questions;
    std::vector<Result> results;
    std::mutex dbMutex;

    void createDirIfNeeded() {
        #ifdef _WIN32
        CreateDirectoryA(dataDir.c_str(), NULL);
        #else
        system("mkdir -p data");
        #endif
    }

public:
    DBManager() {
        createDirIfNeeded();
        loadAll();
    }

    void loadAll() {
        std::lock_guard<std::mutex> lock(dbMutex);
        loadStudents();
        loadQuestions();
        loadResults();
    }

    void saveAll() {
        std::lock_guard<std::mutex> lock(dbMutex);
        saveStudents();
        saveQuestions();
        saveResults();
    }

    void loadStudents() {
        students.clear();
        std::ifstream file(studentsFile);
        if (!file.is_open()) return;
        std::string line;
        while (std::getline(file, line)) {
            if (line.empty()) continue;
            auto fields = dbSplitString(line, '|');
            if (fields.size() >= 3) {
                students.push_back({fields[0], fields[1], fields[2]});
            }
        }
    }

    void saveStudents() {
        std::ofstream file(studentsFile);
        if (!file.is_open()) return;
        for (const auto& s : students) {
            file << s.rollNumber << "|" << s.name << "|" << s.password << "\n";
        }
    }

    void loadQuestions() {
        questions.clear();
        std::ifstream file(questionsFile);
        if (!file.is_open()) return;
        std::string line;
        while (std::getline(file, line)) {
            if (line.empty()) continue;
            auto fields = dbSplitString(line, '|');
            if (fields.size() >= 8) {
                Question q;
                q.id = std::stoi(fields[0]);
                q.text = fields[1];
                q.optionA = fields[2];
                q.optionB = fields[3];
                q.optionC = fields[4];
                q.optionD = fields[5];
                q.correctOption = fields[6].empty() ? 'A' : fields[6][0];
                q.marks = std::stoi(fields[7]);
                questions.push_back(q);
            }
        }
    }

    void saveQuestions() {
        std::ofstream file(questionsFile);
        if (!file.is_open()) return;
        for (const auto& q : questions) {
            file << q.id << "|" << q.text << "|" 
                 << q.optionA << "|" << q.optionB << "|" 
                 << q.optionC << "|" << q.optionD << "|" 
                 << q.correctOption << "|" << q.marks << "\n";
        }
    }

    void loadResults() {
        results.clear();
        std::ifstream file(resultsFile);
        if (!file.is_open()) return;
        std::string line;
        while (std::getline(file, line)) {
            if (line.empty()) continue;
            auto fields = dbSplitString(line, '|');
            if (fields.size() >= 5) {
                results.push_back({fields[0], fields[1], std::stoi(fields[2]), std::stoi(fields[3]), fields[4]});
            }
        }
    }

    void saveResults() {
        std::ofstream file(resultsFile);
        if (!file.is_open()) return;
        for (const auto& r : results) {
            file << r.rollNumber << "|" << r.studentName << "|" << r.score << "|" << r.totalMarks << "|" << r.timestamp << "\n";
        }
    }

    bool addStudent(const std::string& roll, const std::string& name, const std::string& password) {
        std::lock_guard<std::mutex> lock(dbMutex);
        for (const auto& s : students) {
            if (s.rollNumber == roll) return false; 
        }
        students.push_back({roll, name, password});
        saveStudents();
        return true;
    }

    bool deleteStudent(const std::string& roll) {
        std::lock_guard<std::mutex> lock(dbMutex);
        auto it = std::remove_if(students.begin(), students.end(), [&](const Student& s) {
            return s.rollNumber == roll;
        });
        if (it != students.end()) {
            students.erase(it, students.end());
            saveStudents();
            return true;
        }
        return false;
    }

    bool validateStudent(const std::string& roll, const std::string& password, std::string& nameOut) {
        std::lock_guard<std::mutex> lock(dbMutex);
        for (const auto& s : students) {
            if (s.rollNumber == roll) {
                if (!password.empty() && s.password == password) {
                    nameOut = s.name;
                    return true;
                }
                return false;
            }
        }
        return false;
    }

    bool getStudentName(const std::string& roll, std::string& nameOut) {
        std::lock_guard<std::mutex> lock(dbMutex);
        for (const auto& s : students) {
            if (s.rollNumber == roll) {
                nameOut = s.name;
                return true;
            }
        }
        return false;
    }

    bool addQuestion(const std::string& text, const std::string& a, const std::string& b,
                     const std::string& c, const std::string& d, char correct, int marks) {
        std::lock_guard<std::mutex> lock(dbMutex);
        int maxId = 0;
        for (const auto& q : questions) {
            if (q.id > maxId) maxId = q.id;
        }
        questions.push_back({maxId + 1, text, a, b, c, d, correct, marks});
        saveQuestions();
        return true;
    }

    bool deleteQuestion(int id) {
        std::lock_guard<std::mutex> lock(dbMutex);
        auto it = std::remove_if(questions.begin(), questions.end(), [&](const Question& q) {
            return q.id == id;
        });
        if (it != questions.end()) {
            questions.erase(it, questions.end());
            saveQuestions();
            return true;
        }
        return false;
    }

    bool addResult(const std::string& roll, int score, int totalMarks, const std::string& timestamp) {
        std::lock_guard<std::mutex> lock(dbMutex);
        std::string sName = "Unknown Student";
        for (const auto& s : students) {
            if (s.rollNumber == roll) {
                sName = s.name;
                break;
            }
        }
        auto it = std::remove_if(results.begin(), results.end(), [&](const Result& r) {
            return r.rollNumber == roll;
        });
        results.erase(it, results.end());

        results.push_back({roll, sName, score, totalMarks, timestamp});
        saveResults();
        return true;
    }

    std::vector<Student> getStudentsCopy() { std::lock_guard<std::mutex> lock(dbMutex); return students; }
    std::vector<Question> getQuestionsCopy() { std::lock_guard<std::mutex> lock(dbMutex); return questions; }
    std::vector<Result> getResultsCopy() { std::lock_guard<std::mutex> lock(dbMutex); return results; }

    bool getStudentResult(const std::string& roll, Result& resOut) {
        std::lock_guard<std::mutex> lock(dbMutex);
        for (const auto& r : results) {
            if (r.rollNumber == roll) {
                resOut = r;
                return true;
            }
        }
        return false;
    }

    void loadSampleData() {
        std::lock_guard<std::mutex> lock(dbMutex);
        students.clear();
        students.push_back({"101", "Abraar Beigh", "1234"});
        students.push_back({"102", "John Doe", "password123"});
        students.push_back({"103", "Alice Smith", "alice123"});
        saveStudents();

        questions.clear();
        questions.push_back({1, "Which of the following is the address-of operator in C++?", "&", "*", "->", "&&", 'A', 2});
        questions.push_back({2, "Which data type is used to represent true/false values in C++?", "int", "float", "bool", "char", 'C', 2});
        questions.push_back({3, "What does STL stand for in C++?", "Simple Template Library", "Standard Template Library", "System Template Linker", "Standard Tool Language", 'B', 2});
        questions.push_back({4, "Which keyword is used to allocate memory dynamically in C++?", "malloc", "alloc", "new", "create", 'C', 2});
        questions.push_back({5, "Which C++ standard introduced auto, nullptr, and lambda expressions?", "C++98", "C++03", "C++11", "C++20", 'C', 2});
        saveQuestions();
        
        results.clear();
        saveResults();
    }
};

#endif // DATABASE_H
