#include "httpparser.h"

#include <QTextStream>
#include <QStringList>
#include <QHash>
#include <QDebug>

HttpParser::HttpParser(REQRES rr, QObject *parent) :
    QObject(parent)
{
    reqres = rr;
    reset();
}

void HttpParser::reset()
{
    dataRemain = 0;
    url = QUrl();
    method = QString();
    address = QString();
    version = QString();
    structingBody = QByteArray();
    chunkMode = false;
}

void HttpParser::input(QByteArray in)
{
    if (!chunkMode && dataRemain <= 0) {
        reset();
        // ヘッダのパース
        int pos = in.indexOf("\r\n\r\n");
        if (pos <= 0) {
            qWarning() << "cannot parse header:\n" << in;
            return;
        }
        QByteArray header = in.left(pos);
        in.remove(0, pos+4);
        QTextStream headerIn(header);
        QString firstLine = headerIn.readLine();

        QHash<QString, QString> headerHash;
        QStringList headers;
        QString line;
        while (!(line = headerIn.readLine()).isNull()) {
            QStringList kv = line.split(':');
            headerHash[kv.value(0).trimmed()] = kv.value(1).trimmed();
            headers << line;
        }

        // リクエスト (1 行目) のパース
        QStringList firstLineEntries = firstLine.split(' ');
        if (reqres == REQUEST) {
            method = firstLineEntries.value(0);
            address = firstLineEntries.value(1);
            version = firstLineEntries.value(2);
        } else if (reqres == RESPONSE) {
            version = firstLineEntries.value(0);
            // firstLineEntries.value(1); // status
            // firstLineEntries.value(2); // additional message
        }

        // Transfer-Encoding: chunked をセット
        if (headerHash["Transfer-Encoding"] == "chunked") {
            chunkMode = true;
        }
        // Content-Length をセット
        else if (!headerHash["Content-Length"].isNull()) {
            dataRemain = headerHash["Content-Length"].toInt();
        }

        // 本来の宛先のパース
        url = QUrl((method == "CONNECT" ? "https://" : "") + address);

        // URL 部分を絶対アドレスから相対アドレスへ変換
        address.remove(QRegExp("^[^:]+://[^:/]+(:\\d+)?"));

        // 送信すべきデータを登録
        switch (reqres) {
        case REQUEST:
            enqueueData((method + " " + address + " " + version + "\r\n").toLatin1());
            break;
        case RESPONSE:
            enqueueData(firstLine.toLatin1());
            enqueueData("\r\n");
            break;
        }
        enqueueData(headers.join("\r\n").toLatin1());
        enqueueData("\r\n\r\n");
    }
    enqueueData(in);

    // body 部分を読み込み
    if (chunkMode && dataRemain <= 0) {
        int pos = in.indexOf("\r\n");
        dataRemain = QString(in.left(pos)).toInt(0, 16);
        in.remove(0, pos + 2);
        chunkMode = dataRemain > 0;
    }
    QByteArray body = in.left(dataRemain);
    in.remove(0, dataRemain);
    dataRemain -= body.size();
    appendBody(body);

    // HTTP メッセージ終了
    if (!chunkMode && dataRemain == 0) {
        emit completeMessage(structingBody);
    }

    // 区切りの改行は次の読み込みには不必要
    if (in.startsWith("\r\n")) {
        in.remove(0, 2);
    }
    // 読み込み終わっていないデータが残っていれば処理する
    if (in.size() > 0) {
        input(in);
    }
}

void HttpParser::enqueueData(QByteArray data)
{
    dataToForward.append(data);
}

QByteArray HttpParser::dequeueData()
{
    QByteArray tmp = dataToForward;
    dataToForward = QByteArray();
    return tmp;
}

void HttpParser::appendBody(QByteArray body)
{
    structingBody.append(body);
}
