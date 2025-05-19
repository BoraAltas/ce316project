#include "iae.h"
#include "SQLite.h"
#include "project.h"
#include "sourceCodeHandler.h"
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QIODevice>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSqlQuery>
#include <iostream>
#include <utility>
#include <QFileDialog>
#include <QSqlError>

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
    QList<Project*> m_projects;
    QList<Config*> m_configs;
};

IAE::IAE(QObject *parent) : QObject(parent), iae(std::make_unique<iaeBackend>()) {} //constructor

QString IAE::openFileDialog() {
    return QFileDialog::getOpenFileName(nullptr, "Select File", "", "All Files (*.*)");
}

IAE::~IAE() { //destructor , todo
    std::cout << "hello!";

}

void IAE::isEmpty() {
    qDebug() << iae->m_projects.isEmpty();
    qDebug() << iae->m_projects.size();
}

void IAE::Initialize() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("example.db");

    if (!db.open()) {
        qWarning() << "Failed to open database:" << db.lastError().text();
        return;
    } else {
        qDebug() << "Database connection opened successfully.";
    }

    QSqlQuery query;
    if (!query.exec("CREATE TABLE IF NOT EXISTS Students ("
                    "studentId TEXT PRIMARY KEY, "
                    "result TEXT, "
                    "success INTEGER, "
                    "projectId INTEGER)")) {
        qWarning() << "Failed to create Students table:" << query.lastError().text();
                    }

    if (!query.exec("CREATE TABLE IF NOT EXISTS Projects ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                    "name TEXT, "
                    "studentId TEXT, "
                    "FOREIGN KEY(studentId) REFERENCES Students(studentId))")) {
        qWarning() << "Failed to create Projects table:" << query.lastError().text();
                    }

    const QString folderPath = QDir::currentPath() + "/configs";
    loadConfigs(folderPath);
    loadProjects();
}

QVariantList IAE::getConfigsAsVariantList() const {
    QVariantList configList;
    for (const auto &config : iae->m_configs) {
        QVariantMap configMap;
        configMap["name"] = config->configName();
        configMap["language"] = config->lang();
        configMap["compileParams"] = config->compileParams();
        configList.append(configMap);
    }
    return configList;
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
    QString const filePath = "configs/" + config->m_configName + ".json";
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
    Q_EMIT configsChanged();
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
            saveConfig(cfg->m_configName, cfg->m_lang, cfg->m_compileParams);
            qDebug() << "Config updated:" << configName;
            Q_EMIT configsChanged();
            return;
        }
    }
}

void IAE::loadConfigs(const QString &folderPath){
    QDir const dir(folderPath);

    if (!dir.exists()) {
        if (!QDir().mkpath(folderPath)) {
            qWarning() << "Failed to create folder:" << folderPath;
            return;
        }
        qDebug() << "Folder created:" << folderPath;
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
        config->m_lang = jsonObj.value("language").toString();
        config->m_compileParams = jsonObj.value("compileParams").toString();

        if (config->m_configName.isEmpty()) {
            qWarning() << "Config name is empty in file:" << filePath;
            continue;
        }

        iae->m_configs.append(config.release());
    }

    Q_EMIT configsChanged();
    qDebug() << "Loaded" << iae->m_configs.size() << "config(s) from folder.";
}

void IAE::importConfig(const QString& importFilePath) {
    QFile file(importFilePath);

    if (!file.exists()) {
        qWarning() << "Import file does not exist:" << importFilePath;
        return;
    }

    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open import file:" << importFilePath;
        return;
    }

    QByteArray fileData = file.readAll();
    file.close();

    QJsonParseError parseError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(fileData, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        qWarning() << "JSON parse error:" << parseError.errorString();
        return;
    }

    if (!jsonDoc.isObject()) {
        qWarning() << "Invalid JSON structure in import file.";
        return;
    }

    QJsonObject jsonObj = jsonDoc.object();

    QString configName = jsonObj.value("configName").toString();
    QString lang = jsonObj.value("language").toString();
    QString compileParams = jsonObj.value("compileParams").toString();

    if (configName.isEmpty()) {
        qWarning() << "Imported config name is empty. Skipping.";
        return;
    }

    // Cheking if there are a config file with the same name
    bool updated = false;
    for (Config* existing : iae->m_configs) {
        if (existing->m_configName == configName) {
            existing->m_lang = lang;
            existing->m_compileParams = compileParams;
            updated = true;
            qDebug() << "Existing config updated:" << configName;
            break;
        }
    }


    if (!updated) {
        auto config = std::make_unique<Config>();
        config->m_configName = configName;
        config->m_lang = lang;
        config->m_compileParams = compileParams;
        iae->m_configs.append(config.release());
        qDebug() << "New config imported:" << configName;
    }

    saveAllConfigs();
}

void IAE::exportConfig(const QString& configName, const QString& exportFilePath) {
    for (const Config* config : iae->m_configs) {
        if (config->m_configName == configName) {
            QJsonObject jsonObject;
            jsonObject["configName"] = config->m_configName;
            jsonObject["lang"] = config->m_lang;
            jsonObject["compileParams"] = config->m_compileParams;

            QJsonDocument jsonDoc(jsonObject);
            QFile file(exportFilePath);

            if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
                qWarning() << "Failed to open export file for writing:" << exportFilePath;
                return;
            }

            file.write(jsonDoc.toJson());
            file.close();

            qDebug() << "Config exported to:" << exportFilePath;
            return;
        }
    }

    qWarning() << "Config not found for export:" << configName;
}

void IAE::removeConfig(const QString &configName) {
    for (int i = 0; i < iae->m_configs.size(); ++i) {
        Config *config = iae->m_configs[i];
        if (config->m_configName == configName) {
            QString filePath = "configs/" + configName + ".json";
            QFile::remove(filePath);

            delete config;
            iae->m_configs.removeAt(i);

            qDebug() << "Config removed:" << configName;
            Q_EMIT configsChanged();
            return;
        }
    }

    qWarning() << "Config not found for removal:" << configName;
}

void IAE::saveProjects() {
    if (!iae) {
        qWarning() << "iaeBackend not initialized.";
        return;
    }

    SQLite::saveProjects(iae->m_projects);
    qDebug() << "Projects saved through IAE::saveProjects.";
}

void IAE::loadProjects() {
    if (!iae) {
        qWarning() << "iaeBackend not initialized.";
        return;
    }

    iae->m_projects = SQLite::loadProjects();
    qDebug() << "Projects loaded into iae->m_projects.";
}

QQmlListProperty<Project> IAE::projects() {
    return {this,&iae->m_projects};
}

QQmlListProperty<Config> IAE::configs() {
    return {this,&iae->m_configs};
}

void IAE::createProject(const QString& projectName, const QString& configName, const QStringList& programArgs, const QString& expectedOutput, const bool isArgsFile, const bool isOutputFile) {
    qDebug() << "isArgsFile:" << isArgsFile << ", isOutputFile:" << isOutputFile;

    sourceCodeHandler handler;
        const Config* conf = nullptr;
        qDebug() << "Configname:" << configName;
        for (Config* cfg : iae->m_configs) {
            if (cfg->m_configName == configName) {
                conf = cfg;
                break;
            }
        }
        if (conf == nullptr) {
            qWarning() << "Config with name" << configName << "not found.";
            return;
        }
        qDebug() << "lang:" << conf->m_lang;

        QStringList mutableProgramArgs = programArgs;
        QString mutableExpectedOutput = expectedOutput;

        if (isArgsFile) {
            QFile argsFile(mutableProgramArgs.first());
            if (argsFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
                mutableProgramArgs.clear();
                while (!argsFile.atEnd()) {
                    mutableProgramArgs.append(argsFile.readLine().trimmed());
                }
                argsFile.close();
            } else {
                qWarning() << "Failed to open program arguments file:" << mutableProgramArgs.first();
                return;
            }
        }

        if (isOutputFile) {
            QFile outputFile(mutableExpectedOutput);
            if (outputFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
                mutableExpectedOutput = outputFile.readAll().trimmed();
                outputFile.close();
            } else {
                qWarning() << "Failed to open expected output file:" << mutableExpectedOutput;
                return;
            }
        }

        QString projectPath = QDir::currentPath() + "/unzip/" + projectName;
        Project* project = handler.compileAndRunAllFiles(projectName, projectPath, conf->m_lang, conf->m_compileParams, mutableProgramArgs, mutableExpectedOutput);
        if (project != nullptr) {
            iae->m_projects.append(project);
            Q_EMIT projectsChanged();
            qDebug() << "Projects created:" << project->getProjectName();
        }
}