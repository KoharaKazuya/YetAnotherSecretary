#ifndef BODYPARSERMAIN_H
#define BODYPARSERMAIN_H

#include "bodyparserbase.h"

class BodyParserMain : public BodyParserBase
{
    Q_OBJECT
public:
    explicit BodyParserMain(QObject *parent = 0);

signals:

public slots:
    void parse(QUrl url, QByteArray body);

};

#endif // BODYPARSERMAIN_H
