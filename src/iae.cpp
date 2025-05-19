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
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>


class iaeBackend {
public:
    QList<Project*> m_projects;
    QList<Config*> m_configs;
};

IAE::IAE(QObject *parent) : QObject(parent), iae(std::make_unique<iaeBackend>()) {} //constructor

IAE::~IAE() { //destructor , todo
    std::cout << "hello!";
    saveProjectsToDatabase();

}

void IAE::isEmpty() {
    qDebug() << iae->m_projects.isEmpty();
    qDebug() << iae->m_projects.size();
}

void IAE::Initialize() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("iae.db");

    if (!db.open()) {
        qDebug() << "Error: Connection with database failed:" << db.lastError().text();
        return;
    }
    qDebug() << "Database connection established";

    QSqlQuery query;
    const QString createTable = "CREATE TABLE IF NOT EXISTS Projects ("
                                "projectName TEXT, "
                                "expectedOutput TEXT,"
                                "studentID TEXT, "
                                "result TEXT, "
                                "success INTEGER, "
                                "PRIMARY KEY (projectName, studentID))";

    if (!query.exec(createTable)) {
        qDebug() << "Error: table creation failed:" << query.lastError().text();
        return;
    }

    const QString selectQuery = "SELECT projectName, expectedOutput, studentID, result, success FROM Projects";
    if (!query.exec(selectQuery)) {
        qDebug() << "Error: failed to execute select query:" << query.lastError().text();
        return;
    }

    QMap<QString, QList<Student*>> projectStudentsMap;
    QMap<QString, QString> projectExpectedOutputs;

    while (query.next()) {
        QString projectName = query.value("projectName").toString();
        QString expectedOutput = query.value("expectedOutput").toString();
        QString studentID = query.value("studentID").toString();
        QString result = query.value("result").toString();
        bool success = query.value("success").toBool();

        Student* student = new Student(studentID, result, success);
        projectStudentsMap[projectName].append(student);

        // Store the expected output for the project
        if (!projectStudentsMap.contains(projectName)) {
            projectStudentsMap[projectName] = QList<Student*>();
        }
        projectStudentsMap[projectName].append(student);
        projectExpectedOutputs[projectName] = expectedOutput;
    }

    for (auto it = projectStudentsMap.begin(); it != projectStudentsMap.end(); ++it) {
        Project* project = new Project(it.key());
        project->setExpectedOutput(projectExpectedOutputs[it.key()]); // Set expected output
        for (Student* student : it.value()) {
            project->addStudent(student);
        }

        iae->m_projects.append(project);
    }

    qDebug() << "Projects and students loaded into m_projects.";
    Q_EMIT projectsChanged();

    const QString folderPath = QDir::currentPath() + "/configs";
    loadConfigs(folderPath);
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
            project->setExpectedOutput(mutableExpectedOutput);
            iae->m_projects.append(project);
            Q_EMIT projectsChanged();
            qDebug() << "Projects created:" << project->getProjectName();
        }
}

void IAE::saveProjectsToDatabase() {
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isValid()) {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("iae.db");
        if (!db.open()) {
            qWarning() << "Failed to open database connection:" << db.lastError().text();
            return;
        }
    }
    if (!db.isOpen()) {
        qWarning() << "Database is not open. Cannot save projects.";
        return;
    }

    QSqlQuery query;
    const QString insertQuery = "INSERT OR REPLACE INTO Projects (projectName, expectedOutput, studentID, result, success) "
                                "VALUES (:projectName, :expectedOutput, :studentID, :result, :success)";

    for (Project* project : iae->m_projects) {
        QString projectName = project->getProjectName();
        QString expectedOutput = project->getExpectedOutput();
        QQmlListProperty<Student> students = project->getStudents();

        for (int i = 0; i < students.count(&students); ++i) {
            Student* student = students.at(&students, i);

            query.prepare(insertQuery);
            query.bindValue(":projectName", projectName);
            query.bindValue(":expectedOutput", expectedOutput);
            query.bindValue(":studentID", student->getStudentID());
            query.bindValue(":result", student->getResult());
            query.bindValue(":success", student->isSuccess());
            if (!query.exec()) {
                qWarning() << "Failed to insert student into database:" << query.lastError().text();
            }
        }
    }

    qDebug() << "Projects, expected outputs, and students saved to database.";
}