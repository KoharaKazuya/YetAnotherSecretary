#ifndef HTTPPARSER_H
#define HTTPPARSER_H

#include <QObject>
#include <QUrl>

class HttpParser : public QObject
{
    Q_OBJECT
public:
    enum REQRES {
        REQUEST = 0,
        RESPONSE = 1
    };

    explicit HttpParser(REQRES rr, QObject *parent = 0);

    void input(QByteArray in);
    QByteArray dequeueData();

    REQRES reqres;
    QUrl url;
    QString method;
    QString address;
    QString version;

signals:
    void completeMessage(QByteArray message);

public slots:

private:
    void reset();
    void enqueueData(QByteArray data);
    void appendBody(QByteArray body);

    int dataRemain;
    bool chunkMode;
    QByteArray dataToForward;
    QByteArray structingBody;

};

#endif // HTTPPARSER_H
