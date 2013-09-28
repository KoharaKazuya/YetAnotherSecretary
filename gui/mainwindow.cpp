#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtWebKit>
#include <QWebFrame>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->webView->settings()->setAttribute(QWebSettings::PluginsEnabled, true);
    resetUrl();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resetUrl()
{
    ui->webView->load(QUrl("http://www.dmm.com/netgame/social/-/gadgets/=/app_id=854854/"));
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
    QSize size = resizeEvent->size();
    size.setHeight(size.width() *3/5);
    this->resize(size);
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
