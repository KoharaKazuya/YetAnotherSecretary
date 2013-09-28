#include "bodyparsermain.h"

#include <QJsonDocument>
#include <QDebug>

BodyParserMain::BodyParserMain(QObject *parent) :
    BodyParserBase(parent)
{
}

void BodyParserMain::parse(QUrl url, QByteArray body)
{
    if (url.path().startsWith("/kcsapi/")) {
        // 先頭の`svdata=`を削除
        body.remove(0, body.indexOf("=")+1);

        // JSON のパース
        QJsonDocument json = QJsonDocument::fromJson(body);

        qDebug() << json.toJson();
    }
}
