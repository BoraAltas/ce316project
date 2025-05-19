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
    Q_PROPERTY(QString expectedOutput READ getExpectedOutput WRITE setExpectedOutput NOTIFY expectedOutputChanged)

public:
    explicit Project(const QString &projectName, QObject *parent = nullptr);
    [[nodiscard]] QString getProjectName() const;
    [[nodiscard]] QQmlListProperty<Student>getStudents();
    [[nodiscard]] QString getExpectedOutput() const;

    void setStudents(QQmlListProperty<Student> students);
    void addStudent(Student*student);
    void setProjectName(const QString&projectName);
    void setExpectedOutput(const QString& expectedOutput);

    Q_SIGNALS:
        void projectNameChanged();
        void studentsChanged();
        void expectedOutputChanged();


private:
    QString m_projectName;
    QList<Student*> m_students;
    QString m_expectedOutput;
};



#endif //PROJECT_H
