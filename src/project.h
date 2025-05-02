#ifndef PROJECT_H
#define PROJECT_H

#include "student.h"

#include <QObject>
#include <QString>
#include <QList>

class Project final : public QObject {
    Q_OBJECT

public:
    explicit Project(const QString &projectName, QObject *parent = nullptr);
    [[nodiscard]] QString getProjectName()const;
    [[nodiscard]] QList<Student*> getStudents()const;

    void setStudents(const QList<Student*>& students);
    void setProjectName(const QString &projectName);

private:
    QString m_projectName;
    QList<Student*> m_students;
};



#endif //PROJECT_H
