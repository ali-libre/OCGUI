#ifndef VPNMANAGER_H
#define VPNMANAGER_H

#include <QObject>
#include <QProcess>

class VPNManager : public QObject
{
    Q_OBJECT
public:
    explicit VPNManager(QObject *parent = nullptr);

    Q_INVOKABLE void connectVPN(const QString &server,
                                const QString &username,
                                const QString &password,
                                const QString &extraArgs);



    Q_INVOKABLE void disconnectVPN();

signals:
    void logUpdated(const QString &text);
    void connectionClosed();
    void connectionStateChanged(bool connected);

private slots:
    void handleStdout();
    void handleStderr();
    void onFinished(int exitCode, QProcess::ExitStatus status);

private:
    QProcess *m_process;
};

#endif // VPNMANAGER_H
