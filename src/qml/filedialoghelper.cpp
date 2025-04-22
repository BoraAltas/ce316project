//
// Created by Alp Koçak on 22.04.2025.
//

#include "filedialoghelper.h"
#include <QFileDialog>

FileDialogHelper::FileDialogHelper(QObject *parent) : QObject(parent) {}

void FileDialogHelper::openFileDialog() {
    QString file = QFileDialog::getOpenFileName(nullptr, "Dosya Seç", "", "Tüm Dosyalar (*.*)");
    if (!file.isEmpty()) {
        m_selectedFile = file;
        emit selectedFileChanged();
    }
}

QString FileDialogHelper::selectedFile() const {
    return m_selectedFile;
}

