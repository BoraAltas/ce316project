//
// Created by Alp Koçak on 22.04.2025.
//

#include "filedialoghelper.h"
#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QProcess>
#include <qcoreapplication.h>
#include <zlib.h>
#include "unzip.h"


FileDialogHelper::FileDialogHelper(QObject *parent) : QObject(parent) {}

void FileDialogHelper::openFileDialog() {
    QString file = QFileDialog::getOpenFileName(nullptr, "Select a Zip File", "", tr("ZIP Archives (*.zip)"));

    if (!file.isEmpty()) {
        m_selectedFile = file;

        if (m_selectedFile.endsWith(".zip", Qt::CaseInsensitive)) {
            qDebug() << "zip file" << m_selectedFile;
            unzipFile();

        } else {
            qDebug() << "not a zip file" << m_selectedFile;
        }

        emit selectedFileChanged();
    }
}

QString FileDialogHelper::selectedFile() const {
    return m_selectedFile;
}
void FileDialogHelper::unzipFile() {
    QString projectRootDir = QCoreApplication::applicationDirPath();

    // Go one directory up from the build folder (cmake-build-debug)
    QDir projectDir(projectRootDir);
    projectDir.cdUp(); //Goes up to the parent directory

    // Makes sure its goes to the parent directorys "src"
    QDir srcDir(projectDir.absoluteFilePath("src"));
    if (!srcDir.exists()) {
        if (!srcDir.mkpath(".")) {
            qDebug() << "Failed to create src directory.";
            return;
        }
    }

    // Log the src directory path to check if it's correct
    qDebug() << "Extracting to:" << srcDir.absolutePath();

    QString zipFilePath = m_selectedFile;
    unzFile zipfile = unzOpen(zipFilePath.toStdString().c_str());

    if (zipfile == nullptr) {
        qDebug() << "Cannot open zip file.";
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

        QString fullPath = srcDir.absoluteFilePath(QString::fromUtf8(filename));

        if (filename[strlen(filename) - 1] == '/') {
            // It's a directory
            QDir().mkpath(fullPath);
        } else {
            // It's a file
            QFile file(fullPath);
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
    qDebug() << "Unzip completed successfully.";
}




