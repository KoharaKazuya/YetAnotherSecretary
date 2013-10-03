#ifndef APISERVER_H
#define APISERVER_H

#include <QObject>
#include <QLocalServer>
#include <QLocalSocket>

class ApiServer : public QObject
{
    Q_OBJECT
public:
    explicit ApiServer(QObject *parent = 0);
    ~ApiServer();

signals:
    void apiStatusLine(QLocalSocket*, QString);

public slots:

private:
    QLocalServer* statusLineServer;

private slots:
    void newApiConnection();
    void receiveApiMessage();

};

#endif // APISERVER_H
