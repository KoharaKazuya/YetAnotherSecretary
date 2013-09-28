#include "yaswebproxy.h"
#include <QStringList>
#include <QUrl>
#include <QRegExp>

YASWebProxy::YASWebProxy(int port, QObject *parent) :
    QObject(parent)
{
    server = new QTcpServer(this);
    server->listen(QHostAddress("127.0.0.1"), port);
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
    HttpParser* parser = new HttpParser(HttpParser::REQUEST, socket);
    parser->setObjectName("requestParser");
    connect(parser, SIGNAL(completeMessage(QByteArray)), this, SLOT(onRequest(QByteArray)));
}

/**
 * ブラウザとのソケットと本来のホストとのソケットを接続するトンネルを作成
 */
void YASWebProxy::openTunnel()
{
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    QByteArray data = socket->readAll();

    HttpParser* parser = socket->findChild<HttpParser*>("requestParser");
    parser->input(data);

    QTcpSocket* proxySocket = socket->findChild<QTcpSocket*>("tunnel");
    if (!proxySocket) {
        // 本来のホストへのソケットを作成
        proxySocket = new QTcpSocket(socket);
        proxySocket->setObjectName("tunnel");
        proxySocket->connectToHost(parser->url.host(), parser->url.port(80));

        connect(proxySocket, SIGNAL(disconnected()), this, SLOT(closeProxySocket()));
        connect(proxySocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(closeProxySocket()));

        connect(proxySocket, SIGNAL(readyRead()), this, SLOT(forwardResponse()));
        if (parser->method == "CONNECT") {
            disconnect(socket, SIGNAL(readyRead()), this, SLOT(openTunnel()));
            connect(socket, SIGNAL(readyRead()), this, SLOT(forwardRequest()));
        } else {
            HttpParser* resParser = new HttpParser(HttpParser::RESPONSE, proxySocket);
            resParser->setObjectName("responseParser");
            resParser->setProperty("url", parser->url.toString());
            connect(resParser, SIGNAL(completeMessage(QByteArray)), this, SLOT(onResponse(QByteArray)));
        }
    }

    if (proxySocket->waitForConnected()) {
        if (parser->method == "CONNECT") {
            socket->write("HTTP/1.0 200 Connection established\r\n\r\n");
        } else {
            proxySocket->write(parser->dequeueData());
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

    HttpParser* parser = proxySocket->findChild<HttpParser*>("responseParser");
    if (parser != 0) {
        parser->input(res);
        socket->write(parser->dequeueData());
    } else {
        socket->write(res);
    }
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

void YASWebProxy::onRequest(QByteArray body)
{
//    emit apiResponse(url, body);
}

void YASWebProxy::onResponse(QByteArray body)
{
    HttpParser* resParser = qobject_cast<HttpParser*>(sender());
    QUrl url = QUrl(resParser->property("url").toString());
    emit apiResponse(url, body);
}
