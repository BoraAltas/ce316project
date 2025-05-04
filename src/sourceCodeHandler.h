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

    Project* compileAndRunAllFiles(const QString& folderPath, const QString& language, const QString& compilerParams, const QStringList& programArgs, const QString& expectedOutput);

    signals:
        void compilationFinished(const QString& result);

private:
    Student* compileAndRunFile(const QString&filePath, const QString&language, const QString& compilerParams, const QStringList&programArgs, const QString&expectedOutput);
    QString determineCompiler(const QString& config) ;
};

#endif // SOURCECODEHANDLER_H
