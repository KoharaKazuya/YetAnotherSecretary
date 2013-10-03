#ifndef SOCKETMAIN_H
#define SOCKETMAIN_H

#include <QCoreApplication>

class SocketMain : public QObject
{
    Q_OBJECT
public:
    explicit SocketMain(QString target, QByteArray message, QCoreApplication* parent = 0);

signals:

public slots:
    void receiveApiReturn();

};

#endif // SOCKETMAIN_H
