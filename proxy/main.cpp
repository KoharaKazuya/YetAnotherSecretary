#include "yaswebproxy.h"
#include "bodyparsermain.h"
#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    YASWebProxy* proxy = new YASWebProxy();
    BodyParserMain* mainParser = new BodyParserMain();
    QObject::connect(proxy, SIGNAL(apiResponse(QString,QByteArray)), mainParser, SLOT(parse(QString,QByteArray)));

    return a.exec();
}
