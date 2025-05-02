//
// Created by Alp Koçak on 1.05.2025.
//

#include "zipHandler.h"
#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QFile>
#include <QCoreApplication>
#include "unzip.h"

ZipHandler::ZipHandler(QObject *parent) : QObject(parent) {}

void ZipHandler::openFileDialog() {
    QStringList files = QFileDialog::getOpenFileNames(nullptr, "Select Zip Files", "", tr("ZIP Archives (*.zip)"));

    if (!files.isEmpty()) {
        m_selectedFiles = files;

        for (const QString &file : m_selectedFiles) {
            if (file.endsWith(".zip", Qt::CaseInsensitive)) {
                qDebug() << "Processing zip file:" << file;
                unzipFile(file);
            } else {
                qDebug() << "Skipped non-zip file:" << file;
            }
        }

        emit selectedFilesChanged();
    }
}

QStringList ZipHandler::selectedFiles() const {
    return m_selectedFiles;
}

void ZipHandler::unzipFile(const QString &zipFilePath) {
    QString projectRootDir = QCoreApplication::applicationDirPath();
    QDir projectDir(projectRootDir);
    projectDir.cdUp();
    QDir srcDir(projectDir.absoluteFilePath("src"));

    if (!srcDir.exists()) {
        if (!srcDir.mkpath(".")) {
            qDebug() << "Failed to create src directory.";
            return;
        }
    }
    QDir unzipDir(srcDir.filePath("unzip"));
    if (!unzipDir.exists()) {
        if (!unzipDir.mkpath(".")) {
            qDebug() << "Failed to create unzip directory.";
            return;
        }
    }

    qDebug() << "Extracting to:" << unzipDir.absolutePath();

    unzFile zipfile = unzOpen(zipFilePath.toStdString().c_str());
    if (zipfile == nullptr) {
        qDebug() << "Cannot open zip file:" << zipFilePath;
        return;
    }

    unz_global_info global_info;
    if (unzGetGlobalInfo(zipfile, &global_info) != UNZ_OK) {
        qDebug() << "Could not read file global info.";
        unzClose(zipfile);
        return;
    }

    for (uLong i = 0; i < global_info.number_entry; ++i) {
        unz_file_info file_info;
        char filename[256];

        if (unzGetCurrentFileInfo(zipfile, &file_info, filename, sizeof(filename), NULL, 0, NULL, 0) != UNZ_OK) {
            qDebug() << "Could not read file info.";
            unzClose(zipfile);
            return;
        }

        QString relativePath = QString::fromUtf8(filename);
        QString fullPath = unzipDir.absoluteFilePath(relativePath);

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
                qDebug() << "Could not open current file inside zip.";
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

        if ((i + 1) < global_info.number_entry) {
            if (unzGoToNextFile(zipfile) != UNZ_OK) {
                qDebug() << "Could not go to next file.";
                unzClose(zipfile);
                return;
            }
        }
    }

    unzClose(zipfile);
    qDebug() << "Unzip completed for:" << zipFilePath;
}