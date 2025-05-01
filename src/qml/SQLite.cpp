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
