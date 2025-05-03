#ifndef IAE_H
#define IAE_H

#include <QObject>
#include <QString>
#include <QQmlEngine>
#include <QQmlListProperty>
#include "project.h"

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

    Q_PROPERTY(QString status READ status WRITE setStatus NOTIFY statusChanged)
    Q_PROPERTY(QQmlListProperty<Project> projects READ projects NOTIFY projectsChanged)
    Q_PROPERTY(QQmlListProperty<Config> configs READ configs NOTIFY configsChanged)

public:
    explicit IAE(QObject *parent = nullptr);
    ~IAE() override;

    [[nodiscard]] QString status() const; // [[nodiscard]] means the caller must use the return value, can't just call it for funzies
    [[nodiscard]] QQmlListProperty<Project> projects();
    [[nodiscard]] QQmlListProperty<Config> configs();

    Q_INVOKABLE void setStatus(const QString &status);
    Q_INVOKABLE void Initialize();
    Q_INVOKABLE void saveConfig(QString configName, QString lang, QString compileParams);
    Q_INVOKABLE void saveProjects();
    Q_INVOKABLE void editConfig(const QString& configName, const QString& lang, const QString& compileParams);

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
