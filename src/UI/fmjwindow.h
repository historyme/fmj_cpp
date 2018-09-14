#ifndef FMJWINDOW_H
#define FMJWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QWidget>
#include <QObject>
#include <QKeyEvent>

namespace Ui {
    class fmjWindow;
}

class fmjWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit fmjWindow(QWidget *parent = 0);
    ~fmjWindow();

    void run(void);

private:
    Ui::fmjWindow *ui;
    int convertKey(int key);

public slots:
    void doShow();

protected:
    void paintEvent(QPaintEvent *);
    bool eventFilter(QObject *obj, QEvent *ev);
};


class fmgimage : public QObject
{
    Q_OBJECT

public:
    fmgimage(){}
    ~fmgimage(){}

    static fmgimage *getInstance();
    void notifyShow(QImage *image);

signals:
    void needShow();
public:
    QImage image;
};



#endif // FMJWINDOW_H
