#ifndef YASWEBPROXY_H
#define YASWEBPROXY_H

#include "httpparser.h"

#include <QObject>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>

class YASWebProxy : public QObject
{
    Q_OBJECT
public:
    explicit YASWebProxy(int port, QObject *parent = 0);
    ~YASWebProxy();

signals:
    void apiResponse(QUrl, QByteArray res);

public slots:
    void onRequest(QByteArray body);
    void onResponse(QByteArray body);

private:
    QTcpServer* server;

private slots:
    void openNewSocket();
    void openTunnel();
    void forwardResponse();
    void forwardRequest();
    void closeProxySocket();

};

#endif // YASWEBPROXY_H
