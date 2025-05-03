#include "iae.h"
#include "project.h"
#include <iostream>
#include <QDebug>
#include <QDir>
#include <utility>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include <QIODevice>

/*
TODO: x-y | x = is logic done, y = is it implemented
-- Link projects with config +:-
they will not be linked with configs, upon compilation and running, the method will take
the config and projectConfig as parameters and use them, when the method completes its job,
configs are not needed since they will not be shown anywhere. All the necessary info will be
included in the student objects.
-- Config creation steps via gui +:-
Create an object, fill the fields using gui inputs, add to the list if user wants to save.
-- Import export config +:
Export will take a file path(probably using file dialog), then it can be saved using already
existing methods. Importing works the same.
-- Select path for config storage ~:-
Not critical
-- Compiler logic, handling files -:-
-- Project creation with additional fields from gui +:-
They will be created using textfield inputs as args
-- Project save/load +:-
In db, students will be seperated with project names, all content will be parsed to
import students and their results.
-- CHECK WINDOWS COMPATIBILITY +:-
POLICE STEADY WATCHING ME, EVERY DAY THEY CLOCKING ME
RED ALERT, ARMED AND DANGEROUS, I KEEP THAT GLOCK ON ME
-- Gui integration +:-
-- Unzipping logic for folder handling -:-
Our dearest friend kocakalp will solve this. Bless his soul <3.
-- Find a way to deploy the project on w10 -:-
*/

class iaeBackend {
public:
    QString m_status; // for example
    QList<Project*> m_projects;
    QList<Config*> m_configs;
};

IAE::IAE(QObject *parent) : QObject(parent), iae(std::make_unique<iaeBackend>()) {} //constructor

IAE::~IAE() { //destructor , todo
    std::cout << "hello!";
    //close connections when necessary.
}

QString IAE::status() const { //read
    return iae->m_status;
}

void IAE::setStatus(const QString &status) { //write
    iae->m_status = status;
    qDebug() << iae->m_status;
    Q_EMIT statusChanged(); //notify
}

void IAE::Initialize() {
    // TODO: make a method to add every project to the m_projects list
    const QString folderPath = QDir::currentPath() + "/configs";
    loadConfigs(folderPath);
    loadProjects();
}

void IAE::saveConfig(QString configName, QString lang, QString compileParams) {
    auto  config = std::make_unique<Config>();
    config->m_configName = configName;
    config->m_lang = lang;
    config->m_compileParams = compileParams;

    if (configName == "") {
        qWarning() << "Config name is empty. Cannot create file.";
        return;
    }

    QJsonObject jsonObject;
    jsonObject["configName"] = config->m_configName;
    jsonObject["language"] = config->m_lang;
    jsonObject["compileParams"] = config->m_compileParams;

    QJsonDocument const jsonDoc(jsonObject);
    QString const filePath = config->m_configName + ".json";
    QFile file(filePath);

    if (file.exists()) {
        qDebug() << "File already exists. Overriding:" << filePath;
    }

    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        file.write(jsonDoc.toJson());
        file.close();
    } else {
        qWarning() << "Failed to open file for writing:" << filePath;
    }

    iae->m_configs.append(config.release());
}

void IAE::saveAllConfigs() {
    for (Config* config : iae->m_configs) {
        saveConfig(config->m_configName, config->m_lang, config->m_compileParams);
    }
}

// might not be efficient, can be optimized
void IAE::editConfig(const QString& configName, const QString& lang, const QString& compileParams) {
    for (Config* cfg :  iae->m_configs) {
        if (cfg->m_configName == configName) {
            cfg->m_lang = lang;
            cfg->m_compileParams = compileParams;
            qDebug() << "Config updated:" << configName;
            return;
        }
    }
}

void IAE::loadConfigs(const QString &folderPath){
    QDir const dir(folderPath);

    if (!dir.exists()) {
        qWarning() << "Folder does not exist:" << folderPath;
        return;
    }

    QStringList const jsonFiles = dir.entryList(QStringList() << "*.json", QDir::Files);
    if (jsonFiles.isEmpty()) {
        qDebug() << "No JSON files found in:" << folderPath;
        return;
    }

    iae->m_configs.clear(); // clear existing configs if needed

    for (const QString &fileName : jsonFiles) {
        QString filePath = dir.absoluteFilePath(fileName);
        QFile file(filePath);

        if (!file.open(QIODevice::ReadOnly)) {
            qWarning() << "Failed to open file:" << filePath;
            continue;
        }

        QByteArray fileData = file.readAll();
        file.close();

        QJsonParseError parseError;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(fileData, &parseError);

        if (parseError.error != QJsonParseError::NoError) {
            qWarning() << "Failed to parse JSON:" << parseError.errorString() << "in file:" << filePath;
            continue;
        }

        if (!jsonDoc.isObject()) {
            qWarning() << "Invalid JSON structure in file:" << filePath;
            continue;
        }

        QJsonObject const jsonObj = jsonDoc.object();
        auto config = std::make_unique<Config>();
        config->m_configName = jsonObj.value("configName").toString();
        config->m_lang = jsonObj.value("lang").toString();
        config->m_compileParams = jsonObj.value("compileParams").toString();

        if (config->m_configName.isEmpty()) {
            qWarning() << "Config name is empty in file:" << filePath;
            continue;
        }

        iae->m_configs.append(config.release());
    }

    qDebug() << "Loaded" << iae->m_configs.size() << "config(s) from folder.";
}

void IAE::loadProjects() {
    // TODO: create project objects from db, add them to m_projects
    qDebug("hoopla");
}

void IAE::saveProjects() { // all projects will be saved upon exit
    // TODO: save contents of m_projects to db
}

QQmlListProperty<Project> IAE::projects() {
    return {this,&iae->m_projects};
}

QQmlListProperty<Config> IAE::configs() {
    return {this,&iae->m_configs};
}