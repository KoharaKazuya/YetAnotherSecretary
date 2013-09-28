#include "yaswebproxy.h"
#include "bodyparsermain.h"
#include <QCoreApplication>
#include <QTextStream>

void showHelp()
{
    QTextStream out(stdout);
    out << "usage: proxy PORT" << endl;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    int port;
    if (argc == 1) {
        port = 8080;
    } else if (argc == 2) {
        if ((port = QString(argv[1]).toInt()) == 0) {
            showHelp();
            return 1;
        }
    } else {
        showHelp();
        return 1;
    }

    YASWebProxy* proxy = new YASWebProxy(port);
    BodyParserMain* mainParser = new BodyParserMain();
    QObject::connect(proxy, SIGNAL(apiResponse(QString,QByteArray)), mainParser, SLOT(parse(QString,QByteArray)));

    return a.exec();
}
