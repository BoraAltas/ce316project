//
// Created by Alp Koçak on 3.05.2025.
//

#ifndef SQLITE_H
#define SQLITE_H

#include <QObject>
#include <QList>

class Student;
class Project;
class SQLite final {
public:
    static void saveStudents(const QList<Student*>& students);
    static QList<Student*> loadStudents();
    static void saveProjects(const QList<Project*>& projects);
    static QList<Project*> loadProjects();
};



#endif //SQLITE_H
