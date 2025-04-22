//
// Created by Alp Koçak on 22.04.2025.
//

#include "filedialoghelper.h"
#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QProcess>
#include <qcoreapplication.h>

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

    // It uses QProcess to run the unzip command
    QProcess process;
    QStringList arguments;
    // If the file exist before -0 flag will overwirets
    arguments << "-o" << m_selectedFile << "-d" << srcDir.absolutePath();

    process.start("unzip", arguments);
    process.waitForFinished();

    // Its captures the process output
    QString output = process.readAllStandardOutput();
    QString errorOutput = process.readAllStandardError();

    if (process.exitStatus() == QProcess::NormalExit) {
        qDebug() << "Unzip completed successfully.";
        qDebug() << "Unzip Output:" << output;

        //In mac after unzip "__MACOSX" file would be crated. This block deletes "__MACOSX" if its crated
        QDir macosxDir(srcDir.absoluteFilePath("__MACOSX"));
        if (macosxDir.exists()) {
            macosxDir.removeRecursively();
            qDebug() << "__MACOSX folder deleted.";
        }

    } else {
        qDebug() << "Unzip failed:" << process.errorString();
        qDebug() << "Error Output:" << errorOutput;
    }
}




