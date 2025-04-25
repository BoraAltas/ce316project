#ifndef STUDENT_H
#define STUDENT_H

#include <QObject>
#include <QString>

class Student final : public QObject { //property and signal usage might be unnecessary here, we will see.
    Q_OBJECT

    Q_PROPERTY(QString studentID READ getStudentID WRITE setStudentID NOTIFY studentIDChanged)
    Q_PROPERTY(QString result READ getResult WRITE setResult NOTIFY resultChanged)
    Q_PROPERTY(bool success READ isSuccess WRITE setSuccess NOTIFY successChanged)

public:
    Student(const QString &studentID, const QString &result, bool success, QObject *parent = nullptr);
    explicit Student(QObject*parent = nullptr); // default constructor

    [[nodiscard]] QString getStudentID()const; // [[nodiscard]] means the caller must use the return value
    [[nodiscard]] QString getResult()const;
    [[nodiscard]] bool isSuccess()const;

    void setStudentID(const QString &studentID);
    void setResult(const QString &result);
    void setSuccess(bool success);

    Q_SIGNALS:
        void studentIDChanged();
        void resultChanged();
        void successChanged();

private:
    QString m_studentID;
    QString m_result;
    bool m_success;
};

#endif //STUDENT_H
