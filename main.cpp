#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "vpnmanager.h"
#include "profilemanager.h"
#include "systemproxymanager.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    VPNManager vpnManager;
    ProfileManager profileManager;
    SystemProxyManager systemProxy;
    engine.rootContext()->setContextProperty("systemProxy", &systemProxy);
    engine.rootContext()->setContextProperty("vpn", &vpnManager);
    engine.rootContext()->setContextProperty("profiles", &profileManager);

    engine.loadFromModule("OCGUI", "Main");

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
