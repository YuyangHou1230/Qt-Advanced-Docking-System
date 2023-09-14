#include "DockAreaWidget.h"
#include "customfloatingwidget.h"
#include "titlebar.h"
#include <QApplication>
#include <QVBoxLayout>
#include <qapplication.h>
#include <windows.h>
#include <windowsx.h>
const int boundaryWidth = 6;
namespace ads {

CustomFloatingWidget::CustomFloatingWidget(ads::CDockManager *DockManager) :
    CFloatingDockContainer (DockManager)
{
    init();

}

CustomFloatingWidget::CustomFloatingWidget(ads::CDockAreaWidget *DockArea):
    CFloatingDockContainer (DockArea)
{
    init();
}

CustomFloatingWidget::CustomFloatingWidget(ads::CDockWidget *DockWidget) :
    CFloatingDockContainer(DockWidget)
{
    init();
}

void CustomFloatingWidget::reflectWindowsTitle(QString text)
{
    titleBar->SetDialogText(text);
}

void CustomFloatingWidget::turnView()
{

}

void CustomFloatingWidget::updateTitle(QString text)
{
    titleBar->SetDialogText(text);
}

void CustomFloatingWidget::init()
{
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    titleBar = new TableTitleBar(this);

    QVBoxLayout *l = dynamic_cast<QVBoxLayout*>(layout());
    if(l){
        l->insertWidget(0, titleBar);
    }

    connect(titleBar, &TableTitleBar::mouseLeftPress, this, &CustomFloatingWidget::onMouseLeftPress);
    connect(titleBar, &TableTitleBar::mouseMoving, this, &CustomFloatingWidget::onMouseMoving);
    connect(titleBar, &TableTitleBar::mouseLeftRelease, this, &CustomFloatingWidget::onMouseLeftRelease);
    connect(titleBar,&TableTitleBar::closeFloating,this,&CustomFloatingWidget::onCloseFloating);

    titleBar->SetDialogText(windowTitle());

    m_isMoving = false;
}

bool CustomFloatingWidget::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    QWidget::nativeEvent(eventType, message, result);
//    MSG *msg = static_cast<MSG*>(message);
//    switch (msg->message)
//    {
//    case WM_NCHITTEST:
//        int xPos = GET_X_LPARAM(msg->lParam) - this->frameGeometry().x();
//        int yPos = GET_Y_LPARAM(msg->lParam) - this->frameGeometry().y();
//        if(xPos < boundaryWidth && yPos<boundaryWidth)                    //左上角
//            *result = HTTOPLEFT;
//        else if(xPos>=width()-boundaryWidth&&yPos<boundaryWidth)          //右上角
//            *result = HTTOPRIGHT;
//        else if(xPos<boundaryWidth&&yPos>=height()-boundaryWidth)         //左下角
//            *result = HTBOTTOMLEFT;
//        else if(xPos>=width()-boundaryWidth&&yPos>=height()-boundaryWidth)//右下角
//            *result = HTBOTTOMRIGHT;
//        else if(xPos < boundaryWidth)                                     //左边
//            *result =  HTLEFT;
//        else if(xPos>=width()-boundaryWidth)                              //右边
//            *result = HTRIGHT;
//        else if(yPos<boundaryWidth)                                       //上边
//            *result = HTTOP;
//        else if(yPos>=height()-boundaryWidth)                             //下边
//            *result = HTBOTTOM;
////        return true;
//    }
    return false;
}

void CustomFloatingWidget::mousePressEvent(QMouseEvent *e)
{
    qDebug()<<"m_pos = "<<m_pos;
    if(!m_isMoving){
        m_pos = e->globalPos();
        qDebug()<<"m_pos = "<<m_pos;
        //记录当前floatingContainer大小
        m_priSize.setWidth(this->width());
        m_priSize.setHeight(this->height());
    }
    if(e->x()<boundaryWidth || e->x()>this->width()-boundaryWidth){
        setCursor(Qt::SizeHorCursor);
        //记录当前鼠标点击全局坐标
    }else if(e->y()<this->height()&&e->y()>this->height()-boundaryWidth){
        setCursor(Qt::SizeVerCursor);
        //记录当前鼠标点击全局坐标

    }else {
        setCursor(Qt::ArrowCursor);
    }
    return QWidget::mousePressEvent(e);
}

void CustomFloatingWidget::mouseMoveEvent(QMouseEvent *e)
{
//    m_pos = e->globalPos();//记录全局坐标
    m_isMoving = true;
    QPoint movingMouse = e->globalPos();
    qDebug()<<"m_pos = "<<m_pos;
    qDebug()<<"movingMouse = "<<movingMouse;
    int deltaX = movingMouse.rx()-m_pos.rx();
    int deltaY = movingMouse.ry()-m_pos.ry();
    qDebug()<<"deltaX = "<<deltaX;
    qDebug()<<"deltaY = "<<deltaY;
    if(e->x()<boundaryWidth || e->x()>this->width()-boundaryWidth){
        setCursor(Qt::SizeHorCursor);
        resize(m_priSize.width()+deltaX,m_priSize.height()+deltaY);
//        setGeometry(m_pos.rx(),m_pos.ry(),m_priSize.width()+deltaX,m_priSize.height()+deltaY);
    }else if(e->y()<this->height()&&e->y()>this->height()-boundaryWidth){
        setCursor(Qt::SizeVerCursor);
        resize(m_priSize.width()+deltaX,m_priSize.height()+deltaY);
//        setGeometry(m_pos.rx(),m_pos.ry(),m_priSize.width()+deltaX,m_priSize.height()+deltaY);
    }else {
        setCursor(Qt::ArrowCursor);
    }

    return QWidget::mouseMoveEvent(e);
}

void CustomFloatingWidget::enterEvent(QEvent *event)
{
//    QEnterEvent* e = static_cast<QEnterEvent*>(event);
//    qDebug()<<e->pos();
//    if(e->x()<boundaryWidth || e->x()>this->width()-boundaryWidth){
//        setCursor(Qt::SizeVerCursor);
//    }else if(e->y()<this->height()&&e->y()>this->height()-boundaryWidth){
//        setCursor(Qt::SizeHorCursor);
//    }
}

void CustomFloatingWidget::leaveEvent(QEvent *event)
{

}

}
