#include "sourceCodeHandler.h"
#include "student.h"
#include "project.h"
#include <QFileInfo>
#include <QCoreApplication>
#include <QProcess>
#include <QDebug>
#include <QDir>
#include <QDirIterator>

sourceCodeHandler::sourceCodeHandler(QObject *parent) : QObject(parent){}

sourceCodeHandler::~sourceCodeHandler() = default;
//TYPECHECK NEEDED
Project* sourceCodeHandler::compileAndRunAllFiles(const QString& projectName, const QString& folderPath,const QString& language, const QString& compilerParams, const QStringList& programArgs, const QString& expectedOutput) {
    QDir dir(folderPath);
    if (!dir.exists()) {
        qWarning() << "Directory does not exist: " << folderPath;
        return nullptr;
    }

    Project* m_project = new Project(projectName);

    // Get all source files in the directory and its subdirectories based on the language type
    QStringList filePaths;
    const QString l_language = language.toLower();
    QStringList filters;

    if (l_language == "c++") {
        filters << "*.cpp";
    } else if (l_language == "python") {
        filters << "*.py";
    } else if (l_language == "java") {
        filters << "*.java";
    } else if (l_language == "c") {
        filters << "*.c";
    } else {
        qWarning() << "Unsupported language: " << language;
        return nullptr;
    }

    QDirIterator it(folderPath, filters, QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        filePaths << it.next();
    }

    for (const QString& file : filePaths) {
        Student* student = compileAndRunFile(file, l_language, compilerParams, programArgs, expectedOutput);
        m_project->addStudent(student);
    }

    return m_project;
}

Student* sourceCodeHandler::compileAndRunFile(const QString& filePath, const QString& language, const QString& compilerParams, const QStringList& programArgs, const QString& expectedOutput)
{
    const QString compiler = determineCompiler(language);
    QProcess process;

    QString command = compiler;
    QStringList arguments = compilerParams.split(' ', Qt::SkipEmptyParts);

    if (language == "cpp" || language == "c") {
        arguments << filePath;
    } else if (language == "java") {
        QString className = QFileInfo(filePath).baseName();
        if (compilerParams.isEmpty()) {
            command = "javac";
            arguments << filePath;
        }

        process.start(command, arguments);
        if (!process.waitForFinished()) {
            qWarning() << "Error while compiling Java file:" << filePath;
            return new Student(filePath, "Error while compiling", false);
        }

        arguments.clear();
        arguments << className << programArgs;
        command = "java";
        qDebug() << arguments;
        qDebug() << command;
    } else if (language == "python") {
        command = "python";
        arguments << filePath << programArgs;
    } else {
        qWarning() << "Unsupported language: " << language;
        return nullptr;
    }

    process.setWorkingDirectory(QFileInfo(filePath).absolutePath());
    process.start(command, arguments);
    if (!process.waitForFinished()) {
        qWarning() << "Error while running the file:" << filePath;
        return new Student(filePath, "Error while compiling", false);
    }

    const QString output = process.readAllStandardOutput() + process.readAllStandardError();
    const bool success = (!expectedOutput.isEmpty() && output.trimmed() == expectedOutput.trimmed());
    qDebug() << "Studentname:" << filePath << "Output:" << output << "Success:" << success << "Expected:" << expectedOutput;
    return new Student(QFileInfo(QFileInfo(filePath).absolutePath()).fileName(), output, success);
}


QString sourceCodeHandler::determineCompiler(const QString& config)
{
    // Specify the compiler or interpreter based on the config for Windows:
    if (config.toLower() == "c++") {
        // For C++ code, use MinGW's "g++" or MSVC's "cl.exe"
        return "g++";  // Or specify the full path if necessary, e.g., "C:/MinGW/bin/g++"
    }
    if (config.toLower() == "c") {
        // For C code, use "gcc" (MinGW) or MSVC's "cl.exe"
        return "gcc";  // Or specify the full path if necessary
    }
    if (config.toLower() == "java") {
        // For Java, use the Java compiler and runner
        return "javac";  // "javac" for compilation, "java" will be used to run the class
    }
    if (config.toLower() == "python") {
        // For Python, use the Python interpreter
        return "python";  // Assuming Python is installed and available in PATH
    }

    // Return an empty string if the config is unsupported
    return QString();
}
