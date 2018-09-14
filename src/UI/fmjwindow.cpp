#include "fmjwindow.h"
#include "ui_fmjwindow.h"

#include "GameMain.h"
#include "Manager.h"

class QtHMMathod :public HMMathod
{
public:
    virtual void* InitBitmap(int w, int h, int type);
    virtual void DeleteBitmap(void *haldle);
    virtual void SetPixel(void *haldle, int w, int h, int color);
    virtual void DrawBitmap(void *distHaldle, void *srcHaldle, int x, int y);
    virtual void DrawLine(void *distHaldle, int startX, int startY, int stopX, int stopY, int color);
    virtual void DrawRect(void *distHaldle, int x, int y, int i, int j, int color);
    virtual void FillRect(void *distHaldle, int x, int y, int i, int j, int fillColor);
    virtual void FillColor(void *handle, int color);
    virtual void DrawScreen(void *haldle);
    virtual void pollEvent(void);
};

void* QtHMMathod::InitBitmap(int w, int h, int type)
{
    QImage *image = new QImage(w, h, QImage::Format_ARGB32);
    return image;
}
void QtHMMathod::SetPixel(void *haldle, int w, int h, int color)
{
    QImage *image = (QImage *)haldle;
    image->setPixel(w, h, (uint)color);
}

void QtHMMathod::DrawBitmap(void *distHaldle, void *srcHaldle, int x, int y)
{
    QImage *dist = (QImage *)distHaldle;
    QImage *src = (QImage *)srcHaldle;

    QPainter p(dist);
    p.drawImage(x, y, *src);
}

void QtHMMathod::DrawLine(void *distHaldle, int startX, int startY, int stopX, int stopY, int color)
{
    QImage *image = (QImage *)distHaldle;

    QPainter p(image);
    p.setPen(QColor(color));
    p.drawLine(startX, startY, stopX, stopY);
}

void QtHMMathod::DrawRect(void *distHaldle, int x, int y, int i, int j, int color)
{
    QImage *image = (QImage *)distHaldle;

    QPainter p(image);
    p.setPen(QColor(color));
    p.drawRect(x, y, i - x, j - y);
}

void QtHMMathod::FillRect(void *distHaldle, int x, int y, int i, int j, int fillColor)
{
    QImage *image = (QImage *)distHaldle;

    QPainter p(image);
    p.fillRect(x, y, i - x, j - y, fillColor);
}

void QtHMMathod::DrawScreen(void *haldle)
{
    QImage *image = (QImage *)haldle;
    fmgimage *fmj = fmgimage::getInstance();
    fmj->notifyShow(image);
}

void QtHMMathod::pollEvent(void)
{
    qApp->processEvents();
}

void QtHMMathod::DeleteBitmap(void *haldle)
{
    QImage *image = (QImage *)haldle;
    delete image;
}

void QtHMMathod::FillColor(void *haldle, int color)
{
    QImage *image = (QImage *)haldle;
    image->fill(color);
}


fmjWindow::fmjWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::fmjWindow)
{
    ui->setupUi(this);

    fmgimage *image = fmgimage::getInstance();
    connect(image, SIGNAL(needShow()), this, SLOT(doShow()));
    installEventFilter(this);

    Manager *man = Manager::getInstance();
    man->setMethod(new QtHMMathod);
}

fmjWindow::~fmjWindow()
{
    delete ui;
}

void fmjWindow::run(void)
{
    gameMain *game_main = new gameMain();
}

void fmjWindow::doShow()
{
    update();
}

void fmjWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QImage image = fmgimage::getInstance()->image.scaled(480, 288);;
    painter.drawImage(0, 0, image);
}

bool fmjWindow::eventFilter(QObject *obj, QEvent *ev)
{
    if (ev->type() == QEvent::MouseButtonPress)
    {
        return true;
    }

    if (ev->type() == QEvent::KeyPress)
    {
        int key = static_cast<QKeyEvent *>(ev)->key();
        int retKey = convertKey(key);
        if (-1 != retKey)
        {
            Manager::getInstance()->onKeyEvent(1, retKey);
        }    
        return true;
    }

    if (ev->type() == QEvent::KeyRelease)
    {
        int key = static_cast<QKeyEvent *>(ev)->key();
        int retKey = convertKey(key);
        if (-1 != retKey)
        {
            Manager::getInstance()->onKeyEvent(0, retKey);
        }
        return true;
    }

    return false;
}

int fmjWindow::convertKey(int key)
{
    int retKey = -1;
    switch (key)
    {
    case Qt::Key_Enter:
    case Qt::Key_Return:
        retKey = KEY_ENTER;
        break;
    case Qt::Key_Escape:
        retKey = KEY_CANCEL;
        break;
    case Qt::Key_Up:
        retKey = KEY_UP;
        break;
    case Qt::Key_Down:
        retKey = KEY_DOWN;
        break;
    case Qt::Key_Left:
        retKey = KEY_LEFT;
        break;
    case Qt::Key_Right:
        retKey = KEY_RIGHT;
        break;
    case Qt::Key_PageUp:
        retKey = KEY_PAGEUP;
        break;
    case Qt::Key_PageDown:
        retKey = KEY_PAGEDOWN;
        break;
    default:
        break;
    }

    return retKey;
}

fmgimage * fmgimage::getInstance()
{
    static fmgimage instance;
    return &instance;
}

void fmgimage::notifyShow(QImage *image)
{
    this->image = *image;
    emit needShow();
}
