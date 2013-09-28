#include "yaswebproxy.h"
#include <QStringList>
#include <QUrl>
#include <QRegExp>

YASWebProxy::YASWebProxy(QObject *parent) :
    QObject(parent)
{
    server = new QTcpServer(this);
    server->listen(QHostAddress::Any, 8888);
    connect(server, SIGNAL(newConnection()), this, SLOT(openNewSocket()));
    qDebug() << "listen start on Port " << server->serverPort();
}

YASWebProxy::~YASWebProxy()
{
    server->close();
}

/**
 * ブラウザとの間に新しいソケットを開く
 */
void YASWebProxy::openNewSocket()
{
    QTcpSocket* socket = server->nextPendingConnection();
    connect(socket, SIGNAL(readyRead()), this, SLOT(openTunnel()));
    connect(socket, SIGNAL(disconnected()), socket, SLOT(deleteLater()));
}

/**
 * ブラウザとのソケットと本来のホストとのソケットを接続するトンネルを作成
 */
void YASWebProxy::openTunnel()
{
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    QByteArray data = socket->readAll();

    // 前回の残り
    // POST のボディ部など
    if (!socket->property("remainToRead").isNull()) {
        QTcpSocket* prevProxySocket = socket->findChild<QTcpSocket*>();
        prevProxySocket->write(data);
        socket->setProperty("remainToRead", QVariant());
        return;
    }

    // ヘッダのパース
    int pos = data.indexOf("\r\n\r\n");
    QByteArray header = data.left(pos);
    data.remove(0, pos+4);
    QTextStream headerIn(header);
    QString requestLine = headerIn.readLine();
    QHash<QString, QString> headerHash;
    QString line;
    QString headers;
    while (!(line = headerIn.readLine()).isNull()) {
        QStringList kv = line.split(':');
        headerHash[kv.value(0).trimmed()] = kv.value(1).trimmed();
        headers += line + "\r\n";
    }

    // リクエスト (1 行目) のパース
    QStringList requestList = requestLine.split(' ');
    QString method = requestList.value(0);
    QString address = requestList.value(1);
    QString version = requestList.value(2);

    bool isConnect = method == "CONNECT";
    socket->setProperty("isCONNECT", isConnect);

    // 本来の宛先のパース
    QUrl url((isConnect ? "https://" : "") + address);
    if (!url.isValid()) {
        qWarning() << "Invalid URL: " << url;
        socket->disconnectFromHost();
        return;
    }
    QString host = url.host();
    int port = url.port(80);

    // 送信するデータの作成
    address.remove(QRegExp("^[^:]+://[^:/]+(:\\d+)?"));
    QByteArray message = (method + " " + address + " " + version + "\r\n"
                          + headers).toLatin1() + "\r\n" + data;

    // Content-Length の処理
    if (!headerHash["Content-Length"].isNull()) {
        int remain = headerHash["Content-Length"].toInt();
        remain -= data.size();
        socket->setProperty("remainToRead", remain);
    }

    QTcpSocket* proxySocket = socket->findChild<QTcpSocket*>("tunnel");
    if (!proxySocket) {
        // 本来のホストへのソケットを作成
        proxySocket = new QTcpSocket(socket);
        proxySocket->setObjectName("tunnel");
        proxySocket->connectToHost(host, port);

        proxySocket->setProperty("path", url.path());

        connect(proxySocket, SIGNAL(disconnected()), this, SLOT(closeProxySocket()));
        connect(proxySocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(closeProxySocket()));

        connect(proxySocket, SIGNAL(readyRead()), this, SLOT(forwardResponse()));
        if (isConnect) {
            disconnect(socket, SIGNAL(readyRead()), this, SLOT(openTunnel()));
            connect(socket, SIGNAL(readyRead()), this, SLOT(forwardRequest()));
        }
    }

    if (proxySocket->waitForConnected()) {
        if (isConnect) {
            socket->write("HTTP/1.0 200 Connection established\r\n\r\n");
        } else {
            proxySocket->write(message);
        }
    } else {
        proxySocket->disconnect();
    }
}

/**
 * レスポンスを転送する
 */
void YASWebProxy::forwardResponse()
{
    QTcpSocket* proxySocket = qobject_cast<QTcpSocket*>(sender());
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(proxySocket->parent());
    QByteArray res = proxySocket->readAll();
    socket->write(res);

    QString path = proxySocket->property("path").toString();
    int pos = res.indexOf("\r\n\r\n");
    QByteArray body = res.right(res.size() - pos - 4);
    emit apiResponse(path, body);
}

/**
 * リクエストを転送する
 */
void YASWebProxy::forwardRequest()
{
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    QTcpSocket* proxySocket = socket->findChild<QTcpSocket*>("tunnel");
    QByteArray req = socket->readAll();
    proxySocket->write(req);
}

/**
 * 接続を閉じる
 */
void YASWebProxy::closeProxySocket()
{
    QTcpSocket* proxySocket = qobject_cast<QTcpSocket*>(sender());
    if (proxySocket) {
        QTcpSocket* socket = qobject_cast<QTcpSocket*>(proxySocket->parent());
        if (socket)
            socket->disconnectFromHost();
        proxySocket->deleteLater();
    }
}
