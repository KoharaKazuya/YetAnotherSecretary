#include "apiserver.h"

ApiServer::ApiServer(QObject *parent) :
    QObject(parent)
{
    statusLineServer = new QLocalServer(this);
    statusLineServer->listen("yetanothersecretary_gui_statusline");
    statusLineServer->setObjectName("statusline");
    connect(statusLineServer, SIGNAL(newConnection()), this, SLOT(newApiConnection()));
}

ApiServer::~ApiServer()
{
    statusLineServer->close();
}

void ApiServer::newApiConnection()
{
    QLocalServer* server = qobject_cast<QLocalServer*>(sender());
    QLocalSocket* socket = server->nextPendingConnection();
    connect(socket, SIGNAL(readChannelFinished()), this, SLOT(receiveApiMessage()));
    connect(socket, SIGNAL(disconnected()), socket, SLOT(deleteLater()));
}

void ApiServer::receiveApiMessage()
{
    QLocalSocket* socket = qobject_cast<QLocalSocket*>(sender());
    QByteArray message = socket->readAll();
    QString api = socket->parent()->objectName();

    // API の振り分け
    if (api == "statusline")
        emit apiStatusLine(message);
}
