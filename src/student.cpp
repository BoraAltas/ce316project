#include "student.h"
#include <QString>
#include <QObject>

Student::Student(const QString &studentID, const QString &result, const bool success, QObject *parent )
: QObject(parent), m_studentID(studentID), m_result(result), m_success(success) {}

Student::Student(QObject *parent) // default constructor
    : QObject(parent), m_studentID(""), m_result(""), m_success(false) {}

QString Student::getStudentID() const {
    return m_studentID;
}

QString Student::getResult() const { // can be converted to enum
    return m_result;
}

void Student::setStudentID(const QString &studentID) {
    if (m_studentID != studentID) {
        m_studentID = studentID;
        Q_EMIT studentIDChanged();
    }
}

void Student::setResult(const QString &result) {
    if (m_result != result) {
        m_result = result;
        Q_EMIT resultChanged();
    }
}

void Student::setSuccess(const bool success) {
    if (m_success != success) {
        m_success = success;
        Q_EMIT successChanged();
    }
}

bool Student::isSuccess() const {
    return m_success;
}
