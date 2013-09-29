#include "bodyparsermain.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDateTime>
#include <QDebug>
#include <QFile>

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

        // デバッグ用に JSON をファイルに
        QFile file("debug_json/" + url.path().replace("/", "_") + ".json");
        if (file.open(QIODevice::WriteOnly)) {
            file.write(body);
        } else {
            qWarning() << "cannot open file";
        }
        file.close();

        if (url.path() == "/kcsapi/api_get_member/deck_port") {
            QStringList message;
            QDateTime now = QDateTime::currentDateTime();
            QJsonArray teams = json.object().take("api_data").toArray();
            for (QJsonArray::const_iterator ite=teams.constBegin(); ite!=teams.constEnd(); ++ite) {
                QJsonObject team = (*ite).toObject();
                QJsonArray misson = team.take("api_mission").toArray();
                qint64 finishTimeEpoch = misson.at(2).toDouble();
                if (finishTimeEpoch > 0) {
                    QDateTime finishTime = QDateTime::fromMSecsSinceEpoch(finishTimeEpoch);
                    message << team.take("api_name").toString() + ":" + QString::number(now.secsTo(finishTime));
                }
            }
            runHandlers("ensei_wait", message);
        }
    }
}
