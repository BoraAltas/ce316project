//
// Created by Alp Koçak on 22.04.2025.
//

#include "filedialoghelper.h"
#include <QFileDialog>
#include <QDebug>

FileDialogHelper::FileDialogHelper(QObject *parent) : QObject(parent) {}

void FileDialogHelper::openFileDialog() {
    QString file = QFileDialog::getOpenFileName(nullptr, "Select a Zip File", "", tr("ZIP Archives (*.zip)"));

    if (!file.isEmpty()) {
        m_selectedFile = file;

        if (m_selectedFile.endsWith(".zip", Qt::CaseInsensitive)) {
            qDebug() << "zip file" << m_selectedFile;
        } else {
            qDebug() << "not a zip file" << m_selectedFile;
        }

        emit selectedFileChanged();
    }
}

QString FileDialogHelper::selectedFile() const {
    return m_selectedFile;
}
