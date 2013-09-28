#include <QCoreApplication>
#include <QProcess>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QProcess* proxy;
    QProcess* gui;

    QStringList proxyArgs;
    proxyArgs << "8080";
    QStringList guiArgs;
    guiArgs << "localhost:8080";

    proxy->start("../proxy/proxy", proxyArgs);
    gui->start("../gui/gui", guiArgs);

    return a.exec();
//    return 0;
}
