#include "bodyparsermain.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <QTimer>

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
            file.flush();
        } else {
            qWarning() << "cannot open file";
        }
        file.close();

        if (url.path() == "/kcsapi/api_get_member/deck_port") {
            api_ApiGetMember_DeckPort(json.object());
        }
    }
}

void BodyParserMain::api_ApiGetMember_DeckPort(QJsonObject json)
{
    // 残り遠征時間の処理
    QDateTime now = QDateTime::currentDateTime();
    QJsonArray teams = json.take("api_data").toArray();
    for (QJsonArray::const_iterator ite=teams.constBegin(); ite!=teams.constEnd(); ++ite) {
        QJsonObject team = (*ite).toObject();
        QJsonArray misson = team.take("api_mission").toArray();
        qint64 finishTimeEpoch = misson.at(2).toDouble();
        if (finishTimeEpoch > 0) {
            QDateTime finishTime = QDateTime::fromMSecsSinceEpoch(finishTimeEpoch);
            QString teamName = team.take("api_name").toString();
            qint64 waitTime = now.secsTo(finishTime);
            if (waitTime > 0) {
                QTimer* waitTimer = findChild<QTimer*>(teamName);
                if (waitTimer == 0) {
                    // 残り時間のタイマー
                    waitTimer = new QTimer(this);
                    waitTimer->setObjectName(teamName);
                    waitTimer->setSingleShot(true);
                    connect(waitTimer, SIGNAL(timeout()), this, SLOT(api_timeoutEnsei()));
                    // 一秒間刻みのタイマー
                    QTimer* intervalTimer = new QTimer(waitTimer);
                    intervalTimer->setSingleShot(false);
                    intervalTimer->setInterval(1000);
                    intervalTimer->start();
                    connect(intervalTimer, SIGNAL(timeout()), this, SLOT(api_timeoutInterval()));
                }
                waitTimer->start(waitTime * 1000);
            }
        }
    }
//    runHandlers("ensei_wait", message);
}

void BodyParserMain::api_timeoutInterval()
{
    QTimer* intervalTimer = qobject_cast<QTimer*>(sender());
    QTimer* waitTimer = qobject_cast<QTimer*>(intervalTimer->parent());
    QString teamName = waitTimer->objectName();
    runHandlers("ensei_time", QStringList() << teamName << QString::number(waitTimer->remainingTime() / 1000));
}

void BodyParserMain::api_timeoutEnsei()
{
    QTimer* waitTimer = qobject_cast<QTimer*>(sender());
    QString teamName = waitTimer->objectName();
    runHandlers("ensei_finish", QStringList() << teamName);
    waitTimer->deleteLater();
}
