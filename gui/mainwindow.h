#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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

protected:
    void resizeEvent(QResizeEvent* resizeEvent);

private slots:
    void on_webView_loadFinished(bool ok);

    void checkFlashLoaded();

private:
    Ui::MainWindow *ui;

    const QUrl gameUrl = QUrl("http://www.dmm.com/netgame/social/-/gadgets/=/app_id=854854/");
};

#endif // MAINWINDOW_H
