#ifndef PROJECT_H
#define PROJECT_H

#include "student.h"

#include <QObject>
#include <QString>
#include <QList>
#include <QQmlListProperty>

class Project final : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString projectName READ getProjectName WRITE setProjectName NOTIFY projectNameChanged)
    Q_PROPERTY(QQmlListProperty<Student> students READ getStudents WRITE setStudents NOTIFY studentsChanged)

public:
    explicit Project(const QString &projectName, QObject *parent = nullptr);
    [[nodiscard]] QString getProjectName()const;
    [[nodiscard]] QQmlListProperty<Student>getStudents();

    void setStudents(QQmlListProperty<Student> students);
    void addStudent(Student*student);
    void setProjectName(const QString&projectName);

    Q_SIGNALS:
        void projectNameChanged();
        void studentsChanged();


private:
    QString m_projectName;
    QList<Student*> m_students;
};



#endif //PROJECT_H
