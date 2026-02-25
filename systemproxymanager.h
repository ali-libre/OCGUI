#ifndef SYSTEMPROXYMANAGER_H
#define SYSTEMPROXYMANAGER_H

#include <QObject>

class SystemProxyManager : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool enabled READ enabled NOTIFY enabledChanged)

public:
    explicit SystemProxyManager(QObject* parent = nullptr);

    Q_INVOKABLE bool enableSystemProxy(const QString& httpHost, int httpPort,
        const QString& socksHost, int socksPort);
    Q_INVOKABLE bool disableSystemProxy();
    bool enabled() const;

signals:
    void enabledChanged();

private:
    bool m_enabled;
    QString desktopEnvironment() const;
    bool enableGnome(const QString& httpHost, int httpPort,
        const QString& socksHost, int socksPort);
    bool disableGnome();
    bool enableKde(const QString& httpHost, int httpPort,
        const QString& socksHost, int socksPort);
    bool disableKde();
};

#endif
