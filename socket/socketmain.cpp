#include "socketmain.h"

#include <QLocalSocket>

SocketMain::SocketMain(QString target, QByteArray message, QCoreApplication* parent) :
    QObject(parent)
{
    QLocalSocket* socket = new QLocalSocket();
    socket->connectToServer("yetanothersecretary_gui_" + target);
    connect(socket, SIGNAL(readChannelFinished()), this, SLOT(receiveApiReturn()));
    connect(socket, SIGNAL(disconnected()), parent, SLOT(quit()));
    if (socket->waitForConnected(10000)) {
        socket->write(message);
        socket->flush();
    } else {
        fprintf(stderr, QString("cannot open socket '" + target + "'\n"
                                + "Be sure that the YetAnotherSecretary browser has opened.\n").toLocal8Bit().data());
        return parent->exit(1);
    }
}

void SocketMain::receiveApiReturn()
{
    qDebug() << "test";
    QLocalSocket* socket = qobject_cast<QLocalSocket*>(sender());
    printf(socket->readAll());
    socket->deleteLater();
}
