#include "project.h"
#include "student.h"
#include <QString>
#include <QObject>

Project::Project(const QString &projectName , QObject * parent)
    : QObject(parent), m_projectName(projectName) {}

QString Project::getProjectName() const {
    return m_projectName;
}

QString Project::getExpectedOutput() const {
    return m_expectedOutput;
}

void Project::setProjectName(const QString &projectName) {
    if (m_projectName != projectName) {
        m_projectName = projectName;
    }
}

void Project::setExpectedOutput(const QString& expectedOutput) {
    if (m_expectedOutput != expectedOutput) {
        m_expectedOutput = expectedOutput;
    }
}

QQmlListProperty<Student> Project::getStudents() {
    return {this,&m_students};
}

void Project::setStudents(QQmlListProperty<Student> students) {
    m_students.clear();
    for (int i = 0; i < students.count(&students); ++i) {
        m_students.append(students.at(&students, i));
    }
}

void Project::addStudent(Student* student) {
    if (student != nullptr) {
        m_students.append(student);
    }
    Q_EMIT studentsChanged();
}