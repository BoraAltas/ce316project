#include "sourceCodeHandler.h"
#include "student.h"
#include "project.h"
#include <QFileInfo>
#include <QCoreApplication>
#include <QProcess>
#include <QDebug>
#include <QDir>
#include <QDirIterator>
#include <QtConcurrent>
#include <QTimer>
#include <QEventLoop>

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

    QList<QFuture<void>> futures;
    for (const QString& file : filePaths) {
        Student* student = new Student(QFileInfo(file).fileName(), "", false);
        m_project->addStudent(student);

        futures.append(QtConcurrent::run([=]() {
            this->compileAndRunFile(file, l_language, compilerParams, programArgs, expectedOutput, student);
        }));
    }

    for (auto& future : futures) {
        future.waitForFinished();
    }

    return m_project;
}

void sourceCodeHandler::compileAndRunFile(const QString& filePath, const QString& language, const QString& compilerParams, const QStringList& programArgs, const QString& expectedOutput, Student* student) {
    const QString compiler = determineCompiler(language);
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

        QProcess process;
        process.start(command, arguments);
        if (!process.waitForFinished()) {
            qWarning() << "Error while compiling Java file:" << filePath;
            student->setResult("Error while compiling");
            student->setSuccess(false);
        }

        arguments.clear();
        arguments << className << programArgs;
        command = "java";
    } else if (language == "python") {
        command = "python";
        arguments << filePath << programArgs;
    } else {
        qWarning() << "Unsupported language: " << language;
        student->setResult("Unsupported language");
        student->setSuccess(false);
        return;
    }

    QProcess process;
    process.setWorkingDirectory(QFileInfo(filePath).absolutePath());

    QTimer timer;
    timer.setSingleShot(true);
    QEventLoop loop;

    connect(&timer, &QTimer::timeout, [&]() {
        if (process.state() == QProcess::Running) {
            process.kill();
            qWarning() << "Process timed out and was terminated:" << filePath;
        }
        loop.quit();
    });

    connect(&process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), [&]() {
        loop.quit();
    });

    process.start(command, arguments);
    timer.start(1000); // 10 seconds timeout
    loop.exec();

    if (process.state() == QProcess::Running) {
        process.kill();
        student->setSuccess(false);
        student->setResult("Error: Process timed out");
        return;
    }

    const QString output = process.readAllStandardOutput() + process.readAllStandardError();
    const bool success = (!expectedOutput.isEmpty() && output.trimmed() == expectedOutput.trimmed());
    qDebug() << "Studentname:" << filePath << "Output:" << output << "Success:" << success << "Expected:" << expectedOutput;

    student->setSuccess(success);
    student->setResult(output);
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
