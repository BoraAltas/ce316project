//
// Created by Alp Koçak on 1.05.2025.
//

#ifndef ZIPHANDLER_H
#define ZIPHANDLER_H

#include <QObject>
#include <QString>
#include <zlib.h>

class ZipHandler : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString selectedFile READ selectedFile NOTIFY selectedFileChanged)

public:
    explicit ZipHandler(QObject *parent = nullptr);
    Q_INVOKABLE void openFileDialog();
    QString selectedFile() const;

    signals:
        void selectedFileChanged();

private:
    QString m_selectedFile;

    void unzipFile();
};


#endif //ZIPHANDLER_H
