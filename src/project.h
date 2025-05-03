#ifndef PROJECT_H
#define PROJECT_H

#include "student.h"

#include <QObject>
#include <QString>
#include <QList>
#include <QQmlListProperty>

class Project final : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString projectName READ getProjectName WRITE setProjectName)
    Q_PROPERTY(QQmlListProperty<Student> students READ getStudents WRITE setStudents)

public:
    explicit Project(const QString &projectName, QObject *parent = nullptr);
    [[nodiscard]] QString getProjectName()const;
    [[nodiscard]] QQmlListProperty<Student>getStudents();

    void setStudents(QQmlListProperty<Student> students);
    void setProjectName(const QString&projectName);

private:
    QString m_projectName;
    QList<Student*> m_students;
};



#endif //PROJECT_H
