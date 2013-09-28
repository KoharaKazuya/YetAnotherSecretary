#include "bodyparserbase.h"

#include <QDebug>

BodyParserBase::BodyParserBase(QObject *parent) :
    QObject(parent)
{
}

void BodyParserBase::runHandlers(QString eventName, QStringList args)
{
    qDebug() << eventName << ":" << args.join(",");
}
