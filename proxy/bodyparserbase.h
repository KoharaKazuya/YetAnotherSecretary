#ifndef BODYPARSERBASE_H
#define BODYPARSERBASE_H

#include <QObject>
#include <QByteArray>
#include <QStringList>

class BodyParserBase : public QObject
{
    Q_OBJECT
public:
    explicit BodyParserBase(QObject *parent = 0);

signals:

public slots:
    virtual void parse(QString path, QByteArray body) = 0;

protected:
    void runHandlers(QString eventName, QStringList args);

};

#endif // BODYPARSERBASE_H
