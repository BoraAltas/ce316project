//
// Created by Alp Koçak on 1.05.2025.
//

#ifndef ZIPHANDLER_H
#define ZIPHANDLER_H

#include <QObject>
#include <QString>
#include <zlib.h>
#include "unzip.h"

class ZipHandler : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString selectedFile READ selectedFile NOTIFY selectedFileChanged)
    Q_PROPERTY(QString projectName READ projectName WRITE setProjectName NOTIFY projectNameChanged)

public:
    explicit ZipHandler(QObject *parent = nullptr);

    Q_INVOKABLE void openFileDialog();

    QString selectedFile() const;
    QString projectName() const;
    Q_INVOKABLE void setProjectName(const QString &name);
    Q_INVOKABLE void unzipFile(const QString &zipFilePath);

    signals:
        void selectedFileChanged();
        void projectNameChanged();

private:
    QString m_selectedFile;
    QString m_projectName;
};

#endif // ZIPHANDLER_H
