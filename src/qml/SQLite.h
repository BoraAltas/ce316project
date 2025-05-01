//
// Created by efeso on 24.04.2025.
//

#ifndef SQLITE_H
#define SQLITE_H

#include <QObject>
#include <QList>

class Student;
class SQLite {
public:
    static void saveStudents(const QList<Student*>& students);
    static QList<Student*> loadStudents();
};



#endif //SQLITE_H
