#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "messagebar.h"

#include <QMainWindow>
#include <QUrl>
#include <QLocalSocket>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    MessageBar* messageBar;

public slots:
    void resetUrl();
    void resetSize();
    void toggleOnTopWindow(bool top);
    void api_statusline(QLocalSocket* socket, QString message);

signals:
    void sizeChanged(QSize size);

protected:
    void resizeEvent(QResizeEvent* resizeEvent);

private slots:
    void on_webView_loadFinished(bool ok);

    void checkFlashLoaded();

private:
    Ui::MainWindow *ui;

    QUrl gameUrl;
};

#endif // MAINWINDOW_H
