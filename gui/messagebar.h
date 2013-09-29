#ifndef MESSAGEBAR_H
#define MESSAGEBAR_H

#include <QLabel>
#include <QSize>
#include <QMouseEvent>

class MessageBar : public QLabel
{
    Q_OBJECT
public:
    explicit MessageBar(QWidget *parent = 0);

signals:

public slots:
    void setPreferGeometory(QSize size);

protected:
    void mousePressEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);
    void mouseDoubleClickEvent(QMouseEvent *);
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);

private:
    QString qss;

};

#endif // MESSAGEBAR_H
