//
// Created by Alp Koçak on 1.05.2025.
//

#include "zipHandler.h"
#include <QDebug>
#include <QDirIterator>
#include <QDir>
#include <QFileDialog>
#include <QFile>
#include <QCoreApplication>
#include "unzip.h"

ZipHandler::ZipHandler(QObject *parent) : QObject(parent) {}

QString ZipHandler::selectedFile() const {
    return m_selectedFile;
}

QString ZipHandler::projectName() const {
    return m_projectName;
}

void ZipHandler::setProjectName(const QString &name) {
    if (m_projectName != name) {
        m_projectName = name;
        emit projectNameChanged();
    }
}



void ZipHandler::openFileDialog() {
    QString folder = QFileDialog::getExistingDirectory(nullptr, "Select Folder Containing Zip Files", "");
    if (folder.isEmpty()) {
        qDebug() << "No folder selected";
        return;
    }

    QDirIterator it(folder, QStringList() << "*.zip", QDir::Files, QDirIterator::Subdirectories);
    bool foundAny = false;
    while (it.hasNext()) {
        QString zipFilePath = it.next();
        foundAny = true;
        m_selectedFile = zipFilePath;
        qDebug() << "Processing zip file:" << zipFilePath;
        unzipFile(zipFilePath);
        emit selectedFileChanged();
    }

    if (!foundAny) {
        qDebug() << "No zip files found in folder (including subfolders):" << folder;
    }
}


void ZipHandler::unzipFile(const QString &zipFilePath) {
    if (m_projectName.isEmpty()) {
        qWarning() << "Project name not set. Aborting unzip.";
        return;
    }

    // Zip dosyasının adı (örneğin: Alp.zip → Alp)
    QString zipBaseName = QFileInfo(zipFilePath).baseName();

    // Hedef klasör: src/unzip/projectName/zipBaseName/
    QDir unzipDir(QDir::currentPath() + "/unzip/");
    if (!unzipDir.exists()) {
        if (!unzipDir.mkpath(".")) {
            qDebug() << "Failed to create target directory:" << unzipDir.absolutePath();
            return;
        }
    }

    // Zip dosyasını aç
    unzFile zipfile = unzOpen(zipFilePath.toStdString().c_str());
    if (!zipfile) {
        qDebug() << "Cannot open zip file:" << zipFilePath;
        return;
    }

    unz_global_info global_info;
    if (unzGetGlobalInfo(zipfile, &global_info) != UNZ_OK) {
        qDebug() << "Could not read global info.";
        unzClose(zipfile);
        return;
    }

    for (uLong i = 0; i < global_info.number_entry; ++i) {
        unz_file_info file_info;
        char filename[256];

        if (unzGetCurrentFileInfo(zipfile, &file_info, filename, sizeof(filename), nullptr, 0, nullptr, 0) != UNZ_OK) {
            qDebug() << "Could not read file info.";
            unzClose(zipfile);
            return;
        }

        QString relativePath = QString::fromUtf8(filename);

        // MacOS sistem dosyalarını atla
        if (relativePath.startsWith("__MACOSX") || relativePath.contains("/__MACOSX")) {
            qDebug() << "Skipping system folder:" << relativePath;
            continue;
        }

        // Gerçek dosya yolu: src/unzip/projectName/zipBaseName/...
        QString fullPath = unzipDir.filePath(zipBaseName + "/" + relativePath);

        // Klasör mü dosya mı?
        if (filename[strlen(filename) - 1] == '/') {
            QDir().mkpath(fullPath);
        } else {
            QFile file(fullPath);
            QDir().mkpath(QFileInfo(fullPath).absolutePath());

            if (!file.open(QIODevice::WriteOnly)) {
                qDebug() << "Could not open file for writing:" << fullPath;
                unzClose(zipfile);
                return;
            }

            if (unzOpenCurrentFile(zipfile) != UNZ_OK) {
                qDebug() << "Could not open current file in zip.";
                file.close();
                unzClose(zipfile);
                return;
            }

            char buffer[8192];
            int readBytes;
            while ((readBytes = unzReadCurrentFile(zipfile, buffer, sizeof(buffer))) > 0) {
                file.write(buffer, readBytes);
            }

            file.close();
            unzCloseCurrentFile(zipfile);
        }

        // Sonraki dosyaya geç
        if ((i + 1) < global_info.number_entry) {
            if (unzGoToNextFile(zipfile) != UNZ_OK) {
                qDebug() << "Could not go to next file.";
                unzClose(zipfile);
                return;
            }
        }
    }

    unzClose(zipfile);
    qDebug() << "Unzipped successfully to:" << unzipDir.filePath(zipBaseName);
}
