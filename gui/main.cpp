#include "mainwindow.h"
#include "apiserver.h"

#include <QApplication>
#include <QNetworkProxy>

void showHelp()
{
    QTextStream out(stdout);
    out << "usage: gui [PROXY_HOST:PROXY_PORT]";
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 引数の処理
    QString host;
    if (argc == 1) {
    } else if (argc == 2) {
        QStringList entries = QString(argv[1]).split(':');
        QString host = entries.value(0);
        int port = entries.value(1).toInt();
        if (port == 0) {
            showHelp();
            return 1;
        }
        QNetworkProxy proxy(QNetworkProxy::HttpProxy, host, port);
        QNetworkProxy::setApplicationProxy(proxy);
    } else {
        showHelp();
        return 1;
    }

    MainWindow w;

    // API サーバーのセットアップ
    ApiServer api;
    QObject::connect(&api, SIGNAL(apiStatusLine(QLocalSocket*, QString)), &w, SLOT(api_statusline(QLocalSocket*, QString)));

    w.show();

    return a.exec();
}
