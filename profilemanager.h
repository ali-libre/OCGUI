#ifndef PROFILEMANAGER_H
#define PROFILEMANAGER_H

#include <QObject>
#include <QJsonArray>
#include <QJsonObject>

class ProfileManager : public QObject
{
    Q_OBJECT
public:
    explicit ProfileManager(QObject *parent = nullptr);

    Q_INVOKABLE QStringList getProfileNames();
    Q_INVOKABLE QVariantMap loadProfile(const QString &name);
    Q_INVOKABLE QVariantList getProfiles();
    Q_INVOKABLE void deleteProfile(const QString &name);

    Q_INVOKABLE void saveProfile(const QString &name,
                                 const QString &server,
                                 const QString &user,
                                 const QString &pass,
                                 const QString &args);

private:
    QString filePath;
    QJsonObject root;

    void loadFile();
    void saveFile();
};

#endif // PROFILEMANAGER_H
