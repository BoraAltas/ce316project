#include "SQLite.h"
#include "student.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

void SQLite::saveStudents(const QList<Student*>& students) {
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        qWarning() << "Database is not open!";
        return;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO Students (studentId, result, success) "
                  "VALUES (:studentId, :result, :success)");

    for (Student* student : students) {
        query.bindValue(":studentId", student->getStudentID());
        query.bindValue(":result", student->getResult());
        query.bindValue(":success", student->isSuccess());

        if (!query.exec()) {
            qWarning() << "Failed to insert student:"
                       << student->getStudentID()
                       << query.lastError().text();
        }
    }

    qDebug() << "All students saved to the database.";
}

QList<Student*> SQLite::loadStudents() {
    QList<Student*> studentList;

    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        qWarning() << "Database is not open!";
        return studentList;
    }

    QSqlQuery query("SELECT studentId, result, success FROM Students");
    while (query.next()) {
        QString id = query.value(0).toString();
        QString result = query.value(1).toString();
        bool success = query.value(2).toBool();

        Student* student = new Student(id, result, success);
        studentList.append(student);
    }

    qDebug() << "Loaded" << studentList.size() << "students from database.";
    return studentList;
}

