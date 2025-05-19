#include "SQLite.h"
#include "student.h"
#include "project.h"
#include "iae.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

void SQLite::saveProjects(const QList<Project*>& projects) {
    if (QSqlDatabase db = QSqlDatabase::database();!db.isOpen()) {
        qWarning() << "Database is not open!";
        return;
    }

    for (Project* project : projects) {
        QString projectName = project->getProjectName();
        QQmlListProperty<Student> students = project->getStudents();

        for (int i = 0; i < students.count(&students); ++i) {
            Student* student = students.at(&students, i);

            QSqlQuery query;
            query.prepare("INSERT INTO Students (studentId, result, success, projectName) "
                          "VALUES (:studentId, :result, :success, :projectName)");
            query.bindValue(":studentId", student->getStudentID());
            query.bindValue(":result", student->getResult());
            query.bindValue(":success", student->isSuccess());
            query.bindValue(":projectName", projectName);

            if (!query.exec()) {
                qWarning() << "Failed to insert student:"
                           << student->getStudentID()
                           << query.lastError().text();
            }
        }
    }

    qDebug() << "All projects and their students saved to the database.";
}

QList<Project*> SQLite::loadProjects() {
    QList<Project*> projects;

    if (QSqlDatabase db = QSqlDatabase::database();!db.isOpen()) {
        qWarning() << "Database is not open in loadProjects!";
        return projects;
    }

    QSqlQuery projectQuery("SELECT DISTINCT projectName FROM Students");
    while (projectQuery.next()) {
        QString projectName = projectQuery.value("projectName").toString();
        Project* project = new Project(projectName);

        QSqlQuery studentQuery;
        studentQuery.prepare("SELECT studentId, result, success FROM Students WHERE projectName = :projectName");
        studentQuery.bindValue(":projectName", projectName);

        if (studentQuery.exec()) {
            while (studentQuery.next()) {
                QString studentId = studentQuery.value("studentId").toString();
                QString result = studentQuery.value("result").toString();
                bool success = studentQuery.value("success").toBool();

                Student* student = new Student(studentId, result, success);
                project->addStudent(student);
            }
        } else {
            qWarning() << "Failed to load students for project:" << projectName
                       << studentQuery.lastError().text();
        }

        projects.append(project);
    }

    qDebug() << "Loaded" << projects.size() << "projects with their students.";
    return projects;
}