//
// Created by Alp Koçak on 1.05.2025.
//

#include "zipHandler.h"
#include <QDebug>
#include <QDirIterator>
#include <QDir>
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

void ZipHandler::openFile(const QString &filePath) {
    QString folder = filePath;

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

    QString zipBaseName = QFileInfo(zipFilePath).baseName();

    QDir unzipDir(QDir::currentPath() + "/unzip/" + m_projectName + "/" + zipBaseName + "/");
    if (!unzipDir.exists() && !unzipDir.mkpath(".")) {
        qDebug() << "Failed to create target directory:" << unzipDir.absolutePath();
        return;
    }

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
        qDebug() << "Extracting file:" << relativePath;

        if (relativePath.startsWith("_MACOSX") || relativePath.contains("/_MACOSX")) {
            qDebug() << "Skipping system folder:" << relativePath;
            continue;
        }

        QString fullPath = unzipDir.filePath(relativePath);
        qDebug() << "Full path for extraction:" << fullPath;

        if (filename[strlen(filename) - 1] == '/') {
            if (!QDir().mkpath(fullPath)) {
                qDebug() << "Failed to create directory:" << fullPath;
            }
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
                if (file.write(buffer, readBytes) != readBytes) {
                    qDebug() << "Failed to write data to file:" << fullPath;
                }
            }

            file.close();
            unzCloseCurrentFile(zipfile);
        }

        if ((i + 1) < global_info.number_entry) {
            if (unzGoToNextFile(zipfile) != UNZ_OK) {
                qDebug() << "Could not go to next file.";
                unzClose(zipfile);
                return;
            }
        }
    }

    unzClose(zipfile);
    qDebug() << "Unzipped successfully to:" << unzipDir.absolutePath();
}