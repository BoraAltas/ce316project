#include "project.h"
#include "student.h"
#include <QString>
#include <QObject>

Project::Project(const QString &projectName , QObject * parent)
    : QObject(parent), m_projectName(projectName) {}

QString Project::getProjectName() const {
    return m_projectName;
}

void Project::setProjectName(const QString &projectName) {
    if (m_projectName != projectName) {
        m_projectName = projectName;
    }
}

QList<Student> Project::getStudents() const {
    return m_students;
}

void Project::setStudents(const QList<Student> &students) {
    m_students = students;
}