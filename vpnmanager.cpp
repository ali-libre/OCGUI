#include "vpnmanager.h"
#include <QDebug>

VPNManager::VPNManager(QObject *parent)
    : QObject(parent),
      m_process(new QProcess(this))
{
    connect(m_process, &QProcess::readyReadStandardOutput,
            this, &VPNManager::handleStdout);
    connect(m_process, &QProcess::readyReadStandardError,
            this, &VPNManager::handleStderr);
    connect(m_process,
            QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &VPNManager::onFinished);
}

void VPNManager::connectVPN(const QString &server,
                            const QString &username,
                            const QString &password,
                            const QString &extraArgs)
{
    if (m_process->state() != QProcess::NotRunning) {
        emit logUpdated("Already connected...\n");
        return;
    }

    QStringList args;
    args << server
         << "--user=" + username
         << "--passwd-on-stdin";

    if (!extraArgs.trimmed().isEmpty()) {
        args.append(QProcess::splitCommand(extraArgs));
    }

    emit logUpdated("Starting openconnect via pkexec...\n");

    QStringList pk;
    pk << "--disable-internal-agent"
       << "openconnect";
    pk.append(args);

    m_process->start("pkexec", pk);

    if (!m_process->waitForStarted(3000)) {
        emit logUpdated("Failed to start pkexec/openconnect.\n");
        return;
    }

    m_process->write(password.toUtf8() + "\n");

    emit connectionStateChanged(true);
}

void VPNManager::disconnectVPN()
{
    if (m_process->state() != QProcess::NotRunning) {
        emit logUpdated("Disconnecting...\n");
        m_process->kill();
        emit connectionStateChanged(false);
    }
}

void VPNManager::onFinished(int exitCode, QProcess::ExitStatus status)
{
    emit logUpdated("OpenConnect finished.\n");

    // اگر openconnect سریع fail کرد، pkexec را ببند
    if (exitCode != 0) {
        m_process->kill();
    }

    emit connectionStateChanged(false);
}

void VPNManager::handleStdout()
{
    const QString out = QString::fromUtf8(m_process->readAllStandardOutput());
    emit logUpdated(out);
}

void VPNManager::handleStderr()
{
    const QString err = QString::fromUtf8(m_process->readAllStandardError());
    emit logUpdated(err);
}
