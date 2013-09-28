#include "mainwindow.h"
#include <QApplication>
#include <QNetworkProxy>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QNetworkProxy proxy(QNetworkProxy::HttpProxy, "localhost", 8888);
    QNetworkProxy::setApplicationProxy(proxy);

    MainWindow w;
    w.show();

    return a.exec();
}
