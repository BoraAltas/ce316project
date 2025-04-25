#ifndef IAE_H
#define IAE_H

#include <QObject>
#include <QString>
#include <QQmlEngine>

class iaeBackend;

class Config {
public:
    QString m_configName; // will be null if not saved
    QString m_lang;
    QString m_compileParams;
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

public:
    explicit IAE(QObject *parent = nullptr);
    ~IAE() override;

    [[nodiscard]] QString status() const; // [[nodiscard]] means the caller must use the return value, can't just call it for funzies

    Q_INVOKABLE void setStatus(const QString &status);
    Q_INVOKABLE void Initialize();
    Q_INVOKABLE void saveConfig(const Config &config);
    Q_INVOKABLE void saveProjects();

    void saveAllConfigs();
    void loadConfigs(const QString &folderPath);
    void loadProjects();

    Q_SIGNALS:
        void statusChanged();

private:
    std::unique_ptr<iaeBackend> iae;
};

#endif // IAE_H
