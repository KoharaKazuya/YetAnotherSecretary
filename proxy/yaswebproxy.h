#ifndef YASWEBPROXY_H
#define YASWEBPROXY_H

#include <QObject>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>

class YASWebProxy : public QObject
{
    Q_OBJECT
public:
    explicit YASWebProxy(QObject *parent = 0);
    ~YASWebProxy();

signals:
    void apiResponse(QString path, QByteArray res);

public slots:

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
