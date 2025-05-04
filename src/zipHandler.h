//
// Created by Alp Koçak on 1.05.2025.
//

#ifndef ZIPHANDLER_H
#define ZIPHANDLER_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <zlib.h>

class ZipHandler : public QObject {
    Q_OBJECT
    Q_PROPERTY(QStringList selectedFiles READ selectedFiles NOTIFY selectedFilesChanged)
    Q_PROPERTY(QString projectName READ projectName WRITE setProjectName NOTIFY projectNameChanged)

public:
    explicit ZipHandler(QObject *parent = nullptr);
    Q_INVOKABLE void openFileDialog();
    QStringList selectedFiles() const;
    QString projectName() const;
    void setProjectName(const QString &name);

    signals:
        void selectedFilesChanged();
    void projectNameChanged();

private:
    QStringList m_selectedFiles;
    QString m_projectName;

    void unzipFile(const QString &zipFilePath);
};

#endif // ZIPHANDLER_H
