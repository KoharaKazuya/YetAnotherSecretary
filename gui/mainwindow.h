#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "messagebar.h"

#include <QMainWindow>
#include <QUrl>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void resetUrl();
    void resetSize();
    void toggleOnTopWindow(bool top);

signals:
    void sizeChanged(QSize size);

protected:
    void resizeEvent(QResizeEvent* resizeEvent);

private slots:
    void on_webView_loadFinished(bool ok);

    void checkFlashLoaded();

private:
    Ui::MainWindow *ui;

    MessageBar* messageBar;
    QUrl gameUrl;
};

#endif // MAINWINDOW_H
