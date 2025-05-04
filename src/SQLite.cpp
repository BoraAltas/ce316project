#include "SQLite.h"
#include "student.h"
#include "project.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

void SQLite::saveStudents(const QList<Student*>& students, int projectId) {
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        qWarning() << "Database is not open!";
        return;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO Students (studentId, result, success, projectId) "
                  "VALUES (:studentId, :result, :success, :projectId)");

    for (Student* student : students) {
        query.bindValue(":studentId", student->getStudentID());
        query.bindValue(":result", student->getResult());
        query.bindValue(":success", student->isSuccess());
        query.bindValue(":projectId", projectId); // dışarıdan verilen ID

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

void SQLite::saveProjects(const QList<Project*>& projects) {
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        qWarning() << "Database not open in saveProjects.";
        return;
    }

    for (Project* project : projects) {
        QQmlListProperty<Student> students = project->getStudents();
        QString studentId;

        if (students.count(&students) > 0) {
            Student* student = students.at(&students, 0);
            studentId = student->getStudentID();

            QSqlQuery checkQuery;
            checkQuery.prepare("SELECT COUNT(*) FROM Students WHERE studentId = :sid");
            checkQuery.bindValue(":sid", studentId);
            if (checkQuery.exec() && checkQuery.next() && checkQuery.value(0).toInt() == 0) {
                QSqlQuery insertStudent;
                insertStudent.prepare("INSERT INTO Students (studentId, result, success) "
                                      "VALUES (:studentId, :result, :success)");
                insertStudent.bindValue(":studentId", student->getStudentID());
                insertStudent.bindValue(":result", student->getResult());
                insertStudent.bindValue(":success", student->isSuccess());

                if (!insertStudent.exec()) {
                    qWarning() << "Failed to insert student:" << insertStudent.lastError().text();
                }
            }
        }
        QSqlQuery insertProject;
        insertProject.prepare("INSERT INTO Projects (name, studentId) VALUES (:name, :studentId)");
        insertProject.bindValue(":name", project->getProjectName());
        insertProject.bindValue(":studentId", studentId);

        if (!insertProject.exec()) {
            qWarning() << "Failed to insert project:" << insertProject.lastError().text();
            continue;
        }
    }
    qDebug() << "All projects saved with their associated students.";
}

QList<Project*> SQLite::loadProjects() {
    QList<Project*> projects;
    QSqlDatabase db = QSqlDatabase::database();

    if (!db.isOpen()) {
        qWarning() << "Database not open in loadProjects.";
        return projects;
    }

    QSqlQuery projectQuery("SELECT id, name, studentId FROM Projects");
    while (projectQuery.next()) {
        int projectId = projectQuery.value("id").toInt();
        QString projectName = projectQuery.value("name").toString();
        QString studentId = projectQuery.value("studentId").toString();

        Student* student = nullptr;
        if (!studentId.isEmpty()) {
            QSqlQuery studentQuery;
            studentQuery.prepare("SELECT result, success FROM Students WHERE studentId = :sid");
            studentQuery.bindValue(":sid", studentId);

            if (studentQuery.exec() && studentQuery.next()) {
                QString result = studentQuery.value(0).toString();
                bool success = studentQuery.value(1).toBool();
                student = new Student(studentId, result, success);
            } else {
                qWarning() << "Failed to load student for project ID" << projectId;
            }
        }

        Project* project = new Project(projectName);

        if (student) {
            QList<Student*> studentList{student};

            QQmlListProperty<Student> studentProp(
                project,
                &studentList,
                nullptr,
                [](QQmlListProperty<Student>* prop) -> qsizetype {
                    return static_cast<QList<Student*>*>(prop->data)->size();
                },
                [](QQmlListProperty<Student>* prop, qsizetype index) -> Student* {
                    return static_cast<QList<Student*>*>(prop->data)->at(index);
                },
                nullptr
            );

            project->setStudents(studentProp);
        }
        projects.append(project);
    }

    qDebug() << "Loaded" << projects.size() << "projects with their students.";
    return projects;
}
