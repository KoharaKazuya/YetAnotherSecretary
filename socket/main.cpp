#include "socketmain.h"

#include <QCoreApplication>
#include <QLocalSocket>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    if (argc < 3) {
        fprintf(stderr, "usage: socket API_NAME MESSAGE\n");
        return 1;
    }

    QString target(argv[1]);
    QByteArray message;
    for (int i=2; i<argc; ++i) {
        message += QByteArray(argv[i]) + ' ';
    }
    message.remove(message.size() - 1, 1);

    SocketMain s(target, message, &a);

    return a.exec();
}
