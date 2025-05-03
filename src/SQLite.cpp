#include "SQLite.h"
#include "student.h"
#include "project.h"

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

void SQLite::saveProjects(const QList<Project*>& projects) {
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        qWarning() << "Database not open in saveProjects.";
        return;
    }

    for (Project* project : projects) {
        QSqlQuery insertProject;
        insertProject.prepare("INSERT INTO Projects (name) VALUES (:name)");
        insertProject.bindValue(":name", project->getProjectName());

        if (!insertProject.exec()) {
            qWarning() << "Failed to insert project:" << insertProject.lastError().text();
            continue;
        }

        int projectId = insertProject.lastInsertId().toInt();

        QQmlListProperty<Student> students = project->getStudents();
        for (qsizetype i = 0; i < students.count(&students); ++i) {
            Student* student = students.at(&students, i);
            if (!student) continue;
            QSqlQuery insertStudent;
            insertStudent.prepare("INSERT INTO Students (studentId, result, success, projectId) "
                                  "VALUES (:studentId, :result, :success, :projectId)");
            insertStudent.bindValue(":studentId", student->getStudentID());
            insertStudent.bindValue(":result", student->getResult());
            insertStudent.bindValue(":success", student->isSuccess());
            insertStudent.bindValue(":projectId", projectId);

            if (!insertStudent.exec()) {
                qWarning() << "Failed to insert student:" << insertStudent.lastError().text();
            }
        }
    }

    qDebug() << "All projects and students saved.";
}

QList<Project*> SQLite::loadProjects() {
    QList<Project*> projects;
    QSqlDatabase db = QSqlDatabase::database();

    if (!db.isOpen()) {
        qWarning() << "Database not open in loadProjects.";
        return projects;
    }

    QSqlQuery projectQuery("SELECT id, name FROM Projects");
    while (projectQuery.next()) {
        int projectId = projectQuery.value("id").toInt();
        QString projectName = projectQuery.value("name").toString();

        Project* project = new Project(projectName);

        QSqlQuery studentQuery;
        studentQuery.prepare("SELECT studentId, result, success FROM Students WHERE projectId = :pid");
        studentQuery.bindValue(":pid", projectId);

        QList<Student*> studentList;
        if (studentQuery.exec()) {
            while (studentQuery.next()) {
                QString sid = studentQuery.value(0).toString();
                QString res = studentQuery.value(1).toString();
                bool succ = studentQuery.value(2).toBool();
                Student* student = new Student(sid, res, succ, project); // Set project as parent
                studentList.append(student);
            }
        } else {
            qWarning() << "Error loading students for project" << projectId << ":" << studentQuery.lastError().text();
        }

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
        projects.append(project);
    }

    qDebug() << "Loaded" << projects.size() << "projects from database.";
    return projects;
}
