#include "messagebar.h"

MessageBar::MessageBar(QWidget *parent) :
    QLabel(parent)
{
    setText("test test test");
    qss = QString("background-color: rgba(0,0,0,0);"
                  "color: rgba(0,0,0,0);"
                  "padding-left: 1em;");
    setStyleSheet(qss);
}

void MessageBar::setPreferGeometory(QSize size)
{
    // 黒帯の大きさを変更する
    int height = size.height() / 10;
    setGeometry(0, size.height() - height, size.width(), height);

    // フォントの大きさを変更する
    QFont font;
    font.setPixelSize(height / 2);
    setFont(font);
}

// マウスイベントを Flash に伝える
void MessageBar::mousePressEvent(QMouseEvent *ev)       { ev->ignore(); }
void MessageBar::mouseReleaseEvent(QMouseEvent *ev)     { ev->ignore(); }
void MessageBar::mouseMoveEvent(QMouseEvent *ev)        { ev->ignore(); }
void MessageBar::mouseDoubleClickEvent(QMouseEvent *ev) { ev->ignore(); }

void MessageBar::enterEvent(QEvent *)
{
    qss.replace(QRegExp("^background-color:[^;]*;"), "background-color: rgba(0,0,0,0.25);");
    qss.replace(QRegExp(";color:[^;]*;"), ";color: white;");
    setStyleSheet(qss);
}

void MessageBar::leaveEvent(QEvent *)
{
    qss.replace(QRegExp("^background-color:[^;]*;"), "background-color: rgba(0,0,0,0);");
    qss.replace(QRegExp(";color:[^;]*;"), ";color: rgba(0,0,0,0);");
    setStyleSheet(qss);
}
