#ifndef SOURCECODEHANDLER_H
#define SOURCECODEHANDLER_H

#include <QObject>
#include <QString>
#include <QDir>
#include <QProcess>
#include <QList>
#include "project.h"

class sourceCodeHandler : public QObject
{
    Q_OBJECT

public:
    explicit sourceCodeHandler(QObject *parent = nullptr);
    ~sourceCodeHandler();

    Project* compileAndRunAllFiles(const QString& projectname, const QString& folderPath, const QString& language, const QString& compilerParams, const QStringList& programArgs, const QString& expectedOutput);

    signals:
        void compilationFinished(const QString& result);

private:
    void compileAndRunFile(const QString&filePath, const QString&language, const QString& compilerParams, const QStringList&programArgs, const QString&expectedOutput, Student* student);
    QString determineCompiler(const QString& config) ;
};

#endif // SOURCECODEHANDLER_H
