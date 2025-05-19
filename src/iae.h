#ifndef IAE_H
#define IAE_H

#include <QObject>
#include <QString>
#include <QQmlEngine>
#include <QQmlListProperty>
#include "project.h"
#include "sourceCodeHandler.h"

class iaeBackend;

class Config : public QObject {
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QString configName READ configName)
    Q_PROPERTY(QString lang READ lang)
    Q_PROPERTY(QString compileParams READ compileParams)
public:
    QString m_configName; // will be null if not saved
    QString m_lang;
    QString m_compileParams;

    [[nodiscard]] QString configName() const { return m_configName; }
    [[nodiscard]] QString lang() const { return m_lang; }
    [[nodiscard]] QString compileParams() const { return m_compileParams; }

};

class ProjectConfig {
public:
    Config m_config;
    QString m_input;
    QString m_expectedOutput;
};

class IAE : public QObject {
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(QQmlListProperty<Project> projects READ projects NOTIFY projectsChanged)
    Q_PROPERTY(QQmlListProperty<Config> configs READ configs NOTIFY configsChanged)

public:
    explicit IAE(QObject *parent = nullptr);
    ~IAE() override;
    Q_INVOKABLE void isEmpty();

    [[nodiscard]] QQmlListProperty<Project> projects();
    [[nodiscard]] QQmlListProperty<Config> configs();

    Q_INVOKABLE void createProject(const QString& projectName, const QString& configName, const QStringList& programArgs, const QString& expectedOutput, const bool isArgsFile, const bool isOutputFile);
    Q_INVOKABLE QString openFileDialog();
    Q_INVOKABLE void Initialize();
    Q_INVOKABLE void saveConfig(QString configName, QString lang, QString compileParams);
    Q_INVOKABLE void saveProjects();
    Q_INVOKABLE void editConfig(const QString& configName, const QString& lang, const QString& compileParams);

    Q_INVOKABLE QVariantList getConfigsAsVariantList() const;

    Q_INVOKABLE void importConfig(const QString& importFilePath);
    Q_INVOKABLE void exportConfig(const QString& configName, const QString& exportFilePath);
    Q_INVOKABLE void removeConfig(const QString &configName);

    void saveAllConfigs();
    void loadConfigs(const QString &folderPath);
    void loadProjects();

    Q_SIGNALS:
        void statusChanged();
        void projectsChanged();
        void configsChanged();

private:
    std::unique_ptr<iaeBackend> iae;
};

#endif // IAE_H
