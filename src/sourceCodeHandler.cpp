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
Project* sourceCodeHandler::compileAndRunAllFiles(const QString& folderPath, QString& language, const QStringList& compilerParams, const QStringList& programArgs, const QString& expectedOutput)
{
    QDir dir(folderPath);
    if (!dir.exists()) {
        qWarning() << "Directory does not exist: " << folderPath;
        return nullptr;
    }

    Project* m_project = new Project(folderPath);

    // Get all source files in the directory and its subdirectories based on the language type
    QStringList filePaths;
    language = language.toLower();
    QStringList filters;

    if (language == "c++") {
        filters << "*.cpp";
    } else if (language == "python") {
        filters << "*.py";
    } else if (language == "java") {
        filters << "*.java";
    } else if (language == "c") {
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
        Student* student = compileAndRunFile(file, language, compilerParams, programArgs, expectedOutput);
        m_project->addStudent(student);
    }

    return m_project;
}

Student* sourceCodeHandler::compileAndRunFile(const QString& filePath, const QString& language, const QStringList& compilerParams, const QStringList& programArgs, const QString& expectedOutput)
{
    const QString compiler = determineCompiler(language);
    QProcess process;

    QString command = compiler;
    QStringList arguments = compilerParams;

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
    } else if (language == "python") {
        command = "python";
        arguments << filePath << programArgs;
    } else {
        qWarning() << "Unsupported language: " << language;
        return nullptr;
    }

    process.start(command, arguments);
    if (!process.waitForFinished()) {
        qWarning() << "Error while running the file:" << filePath;
        return new Student(filePath, "Error while compiling", false);
    }

    const QString output = process.readAllStandardOutput();
    const bool success = (!expectedOutput.isEmpty() && output.trimmed() == expectedOutput.trimmed());
    return new Student(filePath, output, success);
}


QString sourceCodeHandler::determineCompiler(const QString& config)
{
    // Specify the compiler or interpreter based on the config for Windows:
    if (config == "c++") {
        // For C++ code, use MinGW's "g++" or MSVC's "cl.exe"
        return "g++";  // Or specify the full path if necessary, e.g., "C:/MinGW/bin/g++"
    }
    if (config == "c") {
        // For C code, use "gcc" (MinGW) or MSVC's "cl.exe"
        return "gcc";  // Or specify the full path if necessary
    }
    if (config == "java") {
        // For Java, use the Java compiler and runner
        return "javac";  // "javac" for compilation, "java" will be used to run the class
    }
    if (config == "python") {
        // For Python, use the Python interpreter
        return "python";  // Assuming Python is installed and available in PATH
    }

    // Return an empty string if the config is unsupported
    return QString();
}
