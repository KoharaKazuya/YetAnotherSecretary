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

private:
    void api_ApiGetMember_DeckPort(QJsonObject json);

private slots:
    void api_timeoutInterval();
    void api_timeoutEnsei();

};

#endif // BODYPARSERMAIN_H
