#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtWebKit>
#include <QWebFrame>
#include <QTimer>
#include <QFile>
#include <QNetworkCookie>
#include <QNetworkCookieJar>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // メッセージバーのセットアップ
    messageBar = new MessageBar(ui->webView);
    connect(this, SIGNAL(sizeChanged(QSize)), messageBar, SLOT(setPreferGeometory(QSize)));

    gameUrl = QUrl("http://www.dmm.com/netgame/social/-/gadgets/=/app_id=854854/");

    // 以前の Cookie を保存したファイルがあれば読み込む
    QFile cookieFile(".cookie");
    if (cookieFile.open(QFile::ReadOnly)) {
        QList<QNetworkCookie> list;
        QByteArray line;
        while (!(line = cookieFile.readLine()).isEmpty()) {
            list.append(QNetworkCookie::parseCookies(line));
        }
        cookieFile.close();
        QNetworkCookieJar* cookieJar = new QNetworkCookieJar(this);
        cookieJar->setCookiesFromUrl(list, gameUrl);
        ui->webView->page()->networkAccessManager()->setCookieJar(cookieJar);
    }

    ui->webView->settings()->setAttribute(QWebSettings::PluginsEnabled, true);
    resetUrl();
}

MainWindow::~MainWindow()
{
    // Cookie をファイルに保存する
    QFile cookieFile(".cookie");
    if (cookieFile.open(QFile::WriteOnly)) {
        QNetworkCookieJar* cookieJar = ui->webView->page()->networkAccessManager()->cookieJar();
        QList<QNetworkCookie> list = cookieJar->cookiesForUrl(gameUrl);
        for (int i=0; i<list.size(); ++i) {
            cookieFile.write(list.at(i).toRawForm());
            cookieFile.write("\n");
        }
        cookieFile.close();
    }

    delete ui;
}

void MainWindow::resetUrl()
{
    ui->webView->load(gameUrl);
}

void MainWindow::resetSize()
{
    if (this->isFullScreen()) {
        this->showNormal();
    }
    this->resize(this->baseSize());
}

void MainWindow::resizeEvent(QResizeEvent* resizeEvent)
{
    // アスペクト比の固定
    QSize size = resizeEvent->size();
    size.setWidth(size.height() *5/3);
    this->resize(size);

    emit sizeChanged(this->size());
}

void MainWindow::toggleOnTopWindow(bool)
{
//    setWindowFlags(Qt::WindowStaysOnTopHint);
}

void MainWindow::on_webView_loadFinished(bool)
{
    QList<QWebFrame*> childFrames = ui->webView->page()->mainFrame()->childFrames();
    if (childFrames.size() > 0) {
        QTimer* timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(checkFlashLoaded()));
        timer->start(100);
    }
}

void MainWindow::checkFlashLoaded()
{
    QList<QWebFrame*> childFrames = ui->webView->page()->mainFrame()->childFrames();
    QWebFrame* iframe = childFrames.value(0);
    QWebElement flashElem = iframe->findFirstElement("#externalswf");
    if (!flashElem.isNull()) {
        QTimer* timer = qobject_cast<QTimer*>(sender());
        timer->stop();
        QString src = flashElem.attribute("src");
        QString base = src.split(QRegExp("[^/]+\\.swf")).value(0);

        qDebug() << src;
        ui->webView->page()->mainFrame()->setHtml(QString() + "<html><head><style>"
                                                  + "  * { margin: 0; padding: 0; width: 100%; height: 100%; border: none; }"
                                                  + "</style></head><body>"
                                                  + "<embed id='container' wmode='opaque' quality='best' bgcolor='#000000'"
                                                  + " allowscriptaccess='always' type='application/x-shockwave-flash'"
                                                  + " src='" + src + "'"
                                                  + " base='" + base + "' />"
                                                  + "</body></html>");
    }
}
