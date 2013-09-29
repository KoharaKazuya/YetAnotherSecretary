#include <QLocalSocket>

int main(int argc, char *argv[])
{
    QString target(argv[1]);
    QByteArray message(argv[2]);

    if (argc != 3) {
        fprintf(stderr, "usage: socket API_NAME MESSAGE\n");
        return 1;
    }

    QLocalSocket* socket = new QLocalSocket();
    socket->connectToServer("yetanothersecretary_gui_" + target, QIODevice::WriteOnly);
    if (socket->waitForConnected(10000)) {
        socket->write(message);
        socket->flush();
        socket->close();
        return 0;
    } else {
        fprintf(stderr, QString("cannot open socket '" + target + "'\n"
                                + "Be sure that the YetAnotherSecretary browser has opened.\n").toLocal8Bit().data());
        return 1;
    }
}
