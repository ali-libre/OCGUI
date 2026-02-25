#include "profilemanager.h"
#include <QFile>
#include <QJsonDocument>
#include <QStandardPaths>

ProfileManager::ProfileManager(QObject *parent)
    : QObject(parent)
{
    filePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
               + "/profiles.json";
    qDebug() << "Loading profiles from:" << filePath;
    loadFile();
}

void ProfileManager::loadFile()
{
    QFile f(filePath);
    if (!f.exists()) {
        root["profiles"] = QJsonArray();
        return;
    }

    if (f.open(QIODevice::ReadOnly)) {
        root = QJsonDocument::fromJson(f.readAll()).object();
        f.close();
    }
}

void ProfileManager::saveFile()
{
    QFile f(filePath);
    f.open(QIODevice::WriteOnly);
    f.write(QJsonDocument(root).toJson());
    f.close();
}

QStringList ProfileManager::getProfileNames()
{
    QStringList list;
    QJsonArray arr = root["profiles"].toArray();

    for (auto v : arr)
        list << v.toObject()["name"].toString();

    return list;
}

QVariantMap ProfileManager::loadProfile(const QString &name)
{
    QJsonArray arr = root["profiles"].toArray();

    for (auto v : arr) {
        QJsonObject o = v.toObject();
        if (o["name"].toString() == name) {
            return {
                {"server", o["server"].toString()},
                {"username", o["username"].toString()},
                {"password", o["password"].toString()},
                {"args", o["args"].toString()}
            };
        }
    }

    return {};
}

QVariantList ProfileManager::getProfiles()
{
    QVariantList list;
    QJsonArray arr = root["profiles"].toArray();

    for (auto v : arr) {
        QJsonObject o = v.toObject();
        QVariantMap m;
        m["name"] = o["name"].toString();
        m["server"] = o["server"].toString();
        m["username"] = o["username"].toString();
        m["password"] = o["password"].toString();
        m["args"] = o["args"].toString();
        list.append(m);
    }

    return list;
}


void ProfileManager::deleteProfile(const QString &name)
{
    QJsonArray arr = root["profiles"].toArray();
    QJsonArray newArr;

    for (auto v : arr) {
        QJsonObject o = v.toObject();
        if (o["name"].toString() != name)
            newArr.append(o);
    }

    root["profiles"] = newArr;
    saveFile();
}

void ProfileManager::saveProfile(const QString &name,
                                 const QString &server,
                                 const QString &user,
                                 const QString &pass,
                                 const QString &args)
{
    QJsonArray arr = root["profiles"].toArray();

    // اگر پروفایل وجود داشت، آپدیتش کن
    for (int i = 0; i < arr.size(); i++) {
        QJsonObject o = arr[i].toObject();
        if (o["name"].toString() == name) {
            o["server"] = server;
            o["username"] = user;
            o["password"] = pass;
            o["args"] = args;
            arr[i] = o;
            root["profiles"] = arr;
            saveFile();
            return;
        }
    }

    // پروفایل جدید
    QJsonObject o;
    o["name"] = name;
    o["server"] = server;
    o["username"] = user;
    o["password"] = pass;
    o["args"] = args;

    arr.append(o);
    root["profiles"] = arr;
    saveFile();
}
