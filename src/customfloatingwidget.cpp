#include "DockAreaWidget.h"
#include "customfloatingwidget.h"
#include "titlebar.h"
#include <QApplication>
#include <QVBoxLayout>
#include <qapplication.h>
#include <windows.h>
#include <windowsx.h>
const int boundaryWidth = 30;
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

void CustomFloatingWidget::setCursorIcon(QMouseEvent *e)
{
    if(e->x()<boundaryWidth&&e->y()<this->height()-boundaryWidth){
        //left
        setCursor(Qt::SizeHorCursor);
        m_boundType = LeftBound;
    }else if(e->x()<boundaryWidth&&e->y()>=this->height()-boundaryWidth){
        //leftBottom
        setCursor(Qt::SizeBDiagCursor);
         m_boundType = LeftBottomBound;
    }else if(e->y()<this->height()&&e->y()>this->height()-boundaryWidth&&e->x()>boundaryWidth&&e->x()<this->width()-boundaryWidth){
        //bottom
        setCursor(Qt::SizeVerCursor);
        m_boundType = BottomBound;
    }else if(e->x()>this->width()-boundaryWidth&&e->y()>=this->height()-boundaryWidth){
        //rightBottom
        setCursor(Qt::SizeFDiagCursor);
        m_boundType = RightBottomBound;
    }else if(e->x()>this->width()-boundaryWidth){
        //right
        setCursor(Qt::SizeHorCursor);
        m_boundType = RightBound;
    }else {
        setCursor(Qt::ArrowCursor);
        m_boundType = NoBound;
    }
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

    setMouseTracking(true);
    m_isMoving = false;
    m_boundType = NoBound;
//    setMinimumSize(1000,1000);
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
//        return true;
//    }
    return false;
}

void CustomFloatingWidget::mousePressEvent(QMouseEvent *e)
{
    if(!m_isMoving){//避免每次移动时也更新点击坐标与窗口坐标
        m_clickPos = e->globalPos();
#ifdef QT_DEBUG
        qDebug()<<"m_clickPos = "<<m_clickPos;
#endif
        //记录当前floatingContainer 位置以及大小
        m_widgetPos.setX(this->geometry().x());
        m_widgetPos.setY(this->geometry().y());
#ifdef QT_DEBUG
        qDebug()<<"m_widgetPos = "<<m_widgetPos;
#endif
        m_priSize.setWidth(this->width());
        m_priSize.setHeight(this->height());
    }
    setCursorIcon(e);
    return QWidget::mousePressEvent(e);
}

void CustomFloatingWidget::mouseMoveEvent(QMouseEvent *e)
{
    m_isMoving = true;
    QPoint movingMouse = e->globalPos();
#ifdef QT_DEBUG
    qDebug()<<"m_clickPos = "<<m_clickPos;
    qDebug()<<"movingMouse = "<<movingMouse;
#endif
    int deltaX = movingMouse.rx()-m_clickPos.rx();
    int deltaY = movingMouse.ry()-m_clickPos.ry();
#ifdef QT_DEBUG
    qDebug()<<"deltaX = "<<deltaX;
    qDebug()<<"deltaY = "<<deltaY;
#endif
    switch (m_boundType) {
    case ads::CustomFloatingWidget::NoBound:
        break;
    case ads::CustomFloatingWidget::LeftBound:
    {
        if(m_priSize.width()-deltaX <this->minimumWidth()){
            return QWidget::mouseMoveEvent(e);
        }
        setGeometry(m_widgetPos.x()+deltaX,m_widgetPos.y(),m_priSize.width()-deltaX,m_priSize.height());
    }
        break;
    case ads::CustomFloatingWidget::LeftBottomBound:
    {
        if(m_priSize.width()-deltaX <this->minimumWidth()){
            return QWidget::mouseMoveEvent(e);
        }
        setGeometry(m_widgetPos.x()+deltaX,m_widgetPos.y(),m_priSize.width()-deltaX,m_priSize.height()+deltaY);
    }
        break;
    case ads::CustomFloatingWidget::BottomBound:
    {
         resize(m_priSize.width(),m_priSize.height()+deltaY);
    }
        break;
    case ads::CustomFloatingWidget::RightBottomBound:
    {
         resize(m_priSize.width()+deltaX,m_priSize.height()+deltaY);
    }
        break;
    case ads::CustomFloatingWidget::RightBound:
    {
         resize(m_priSize.width()+deltaX,m_priSize.height());
    }
        break;

    }
    return QWidget::mouseMoveEvent(e);
}

void CustomFloatingWidget::mouseReleaseEvent(QMouseEvent *e)
{
    m_isMoving = false;
    m_boundType = NoBound;
    setCursor(Qt::ArrowCursor);
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
