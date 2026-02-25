#include "systemproxymanager.h"
#include <QProcess>
#include <QProcessEnvironment>

SystemProxyManager::SystemProxyManager(QObject *parent)
    : QObject(parent),
      m_enabled(false)
{
}

QString SystemProxyManager::desktopEnvironment() const
{
    auto env = QProcessEnvironment::systemEnvironment();
    QString de = env.value("XDG_CURRENT_DESKTOP").toLower();
    if (de.contains("gnome"))
        return "gnome";
    if (de.contains("kde") || de.contains("plasma"))
        return "kde";
    return QString();
}

bool SystemProxyManager::enableSystemProxy(const QString &httpHost, int httpPort,
                                           const QString &socksHost, int socksPort)
{
    QString de = desktopEnvironment();
    bool ok = false;
    if (de == "gnome")
        ok = enableGnome(httpHost, httpPort, socksHost, socksPort);
    else if (de == "kde")
        ok = enableKde(httpHost, httpPort, socksHost, socksPort);
    if (ok) {
        if (!m_enabled) {
            m_enabled = true;
            emit enabledChanged();
        }
    }
    return ok;
}

bool SystemProxyManager::disableSystemProxy()
{
    QString de = desktopEnvironment();
    bool ok = false;
    if (de == "gnome")
        ok = disableGnome();
    else if (de == "kde")
        ok = disableKde();
    if (ok) {
        if (m_enabled) {
            m_enabled = false;
            emit enabledChanged();
        }
    }
    return ok;
}

bool SystemProxyManager::enabled() const
{
    return m_enabled;
}

bool SystemProxyManager::enableGnome(const QString &httpHost, int httpPort,
                                     const QString &socksHost, int socksPort)
{
    int r1 = QProcess::execute("gsettings", {"set", "org.gnome.system.proxy", "mode", "manual"});
    int r2 = QProcess::execute("gsettings", {"set", "org.gnome.system.proxy.http", "host", httpHost});
    int r3 = QProcess::execute("gsettings", {"set", "org.gnome.system.proxy.http", "port", QString::number(httpPort)});
    int r4 = QProcess::execute("gsettings", {"set", "org.gnome.system.proxy.https", "host", httpHost});
    int r5 = QProcess::execute("gsettings", {"set", "org.gnome.system.proxy.https", "port", QString::number(httpPort)});
    int r6 = QProcess::execute("gsettings", {"set", "org.gnome.system.proxy.socks", "host", socksHost});
    int r7 = QProcess::execute("gsettings", {"set", "org.gnome.system.proxy.socks", "port", QString::number(socksPort)});
    return r1 == 0 && r2 == 0 && r3 == 0 && r4 == 0 && r5 == 0 && r6 == 0 && r7 == 0;
}

bool SystemProxyManager::disableGnome()
{
    int r1 = QProcess::execute("gsettings", {"set", "org.gnome.system.proxy", "mode", "none"});
    return r1 == 0;
}

bool SystemProxyManager::enableKde(const QString &httpHost, int httpPort,
                                   const QString &socksHost, int socksPort)
{
    QString httpUrl = "http://" + httpHost + ":" + QString::number(httpPort);
    QString httpsUrl = httpUrl;
    QString socksUrl = "socks://" + socksHost + ":" + QString::number(socksPort);

    int r1 = QProcess::execute("kwriteconfig5", {"--file", "kioslaverc", "--group", "Proxy Settings", "--key", "ProxyType", "1"});
    int r2 = QProcess::execute("kwriteconfig5", {"--file", "kioslaverc", "--group", "Proxy Settings", "--key", "httpProxy", httpUrl});
    int r3 = QProcess::execute("kwriteconfig5", {"--file", "kioslaverc", "--group", "Proxy Settings", "--key", "httpsProxy", httpsUrl});
    int r4 = QProcess::execute("kwriteconfig5", {"--file", "kioslaverc", "--group", "Proxy Settings", "--key", "socksProxy", socksUrl});
    QProcess::execute("qdbus", {"org.kde.kded5", "/kded", "org.kde.kded5.reloadModule", "proxy"});
    return r1 == 0 && r2 == 0 && r3 == 0 && r4 == 0;
}

bool SystemProxyManager::disableKde()
{
    int r1 = QProcess::execute("kwriteconfig5", {"--file", "kioslaverc", "--group", "Proxy Settings", "--key", "ProxyType", "0"});
    QProcess::execute("qdbus", {"org.kde.kded5", "/kded", "org.kde.kded5.reloadModule", "proxy"});
    return r1 == 0;
}
