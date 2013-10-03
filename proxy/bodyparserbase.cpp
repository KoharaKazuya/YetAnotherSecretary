#include "bodyparserbase.h"

#include <QProcess>
#include <QDebug>
#include <QDir>

BodyParserBase::BodyParserBase(QObject *parent) :
    QObject(parent)
{
}

void BodyParserBase::runHandlers(QString eventName, QStringList args)
{
    QDir scriptsDir(".");
    scriptsDir.mkdir("scripts");
    scriptsDir.cd("scripts");
    QStringList fileList = scriptsDir.entryList(QStringList() << eventName + "_*");
    for (QString fileName : fileList) {
        QProcess* process = new QProcess();
        process->start("./scripts/" + fileName, args);
        connect(process, SIGNAL(finished(int)), process, SLOT(deleteLater()));
        connect(process, SIGNAL(readyRead()), this, SLOT(outputError()));
    }
}

void BodyParserBase::outputError()
{
    QProcess* process = qobject_cast<QProcess*>(sender());
    qWarning() << process->readAll();
}
