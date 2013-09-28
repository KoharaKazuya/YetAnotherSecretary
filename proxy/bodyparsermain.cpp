#include "bodyparsermain.h"

BodyParserMain::BodyParserMain(QObject *parent) :
    BodyParserBase(parent)
{
}

void BodyParserMain::parse(QString path, QByteArray body)
{
    if (path.startsWith("/kcsapi/")) {
        QStringList args;
        args << path << QString::fromUtf8(body);
        runHandlers("test", args);
    }
}
