//
// Created by Alp Koçak on 22.04.2025.
//

#ifndef FILEDIALOGHELPER_H
#define FILEDIALOGHELPER_H

#include <QObject>
#include <QString>

class FileDialogHelper : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString selectedFile READ selectedFile NOTIFY selectedFileChanged)

public:
    explicit FileDialogHelper(QObject *parent = nullptr);
    Q_INVOKABLE void openFileDialog();
    QString selectedFile() const;

    signals:
        void selectedFileChanged();

private:
    QString m_selectedFile;

    void unzipFile();
};

#endif // FILEDIALOGHELPER_H
