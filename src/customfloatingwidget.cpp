#include "DockAreaWidget.h"
#include "customfloatingwidget.h"
#include "titlebar.h"
#include <QApplication>
#include <QVBoxLayout>
#include <qapplication.h>
#include <windows.h>
#include <windowsx.h>
const int boundaryWidth = 10;
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
    }else if(e->x()<boundaryWidth&&e->y()>=this->height()-boundaryWidth){
        //leftBottom
        setCursor(Qt::SizeBDiagCursor);
    }else if(e->y()<this->height()&&e->y()>this->height()-boundaryWidth&&e->x()>boundaryWidth&&e->x()<this->width()-boundaryWidth){
        //bottom
        setCursor(Qt::SizeVerCursor);
    }else if(e->x()>this->width()-boundaryWidth&&e->y()>=this->height()-boundaryWidth){
        //rightBottom
        setCursor(Qt::SizeFDiagCursor);
    }else if(e->x()>this->width()-boundaryWidth){
        //right
        setCursor(Qt::SizeHorCursor);
    }else {
        setCursor(Qt::ArrowCursor);
    }
}

void CustomFloatingWidget::setMaxWidth(int width)
{
    m_widthRange.setY(width);
}

void CustomFloatingWidget::setMinWidth(int width)
{
    m_widthRange.setX(width);
}

void CustomFloatingWidget::setMaxHeight(int height)
{
    m_heightRange.setY(height);
}

void CustomFloatingWidget::setMinHeight(int height)
{
    m_heightRange.setX(height);
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
    setMaxWidth(99999);
    setMinWidth(400);
    setMaxHeight(99999);
    setMinHeight(400);
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
    qDebug()<<"m_clickPos = "<<m_clickPos;
    if(!m_isMoving){//避免每次移动时也更新点击坐标与窗口坐标
        m_clickPos = e->globalPos();
        qDebug()<<"m_clickPos = "<<m_clickPos;
        m_widgetPos.setX(this->geometry().x());
        m_widgetPos.setY(this->geometry().y());
        qDebug()<<"m_widgetPos = "<<m_widgetPos;
        //记录当前floatingContainer大小
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
    qDebug()<<"m_clickPos = "<<m_clickPos;
    qDebug()<<"movingMouse = "<<movingMouse;
    int deltaX = movingMouse.rx()-m_clickPos.rx();
    int deltaY = movingMouse.ry()-m_clickPos.ry();
    qDebug()<<"deltaX = "<<deltaX;
    qDebug()<<"deltaY = "<<deltaY;
    setCursorIcon(e);
    if(e->x()<boundaryWidth&&e->y()<this->height()-boundaryWidth){
        //left
        if(deltaX > this->width()||(m_boundType == RightBound && this->width()<=m_widthRange.x())){//width不能为负,且当宽度为最小宽度时不改变
            return QWidget::mouseMoveEvent(e);
        }
        m_boundType = LeftBound;
        setGeometry(m_widgetPos.x()+deltaX,m_widgetPos.y(),m_priSize.width()-deltaX,m_priSize.height());
    }else if(e->x()<boundaryWidth&&e->y()>=this->height()-boundaryWidth){
        //leftBottom
        if(deltaX > this->width()){//width不能为负
            return QWidget::mouseMoveEvent(e);
        }
        m_boundType = LeftBottomBound;
        setGeometry(m_widgetPos.x()+deltaX,m_widgetPos.y(),m_priSize.width()-deltaX,m_priSize.height()+deltaY);
//        move(m_clickPos.rx()+deltaX,this->geometry().y());
//        resize(m_priSize.width()-deltaX,m_priSize.height()+deltaY);
    }else if(e->y()<this->height()&&e->y()>this->height()-boundaryWidth&&e->x()>boundaryWidth&&e->x()<this->width()-boundaryWidth){
        //bottom
        m_boundType = BottomBound;
        resize(m_priSize.width(),m_priSize.height()+deltaY);
    }else if(e->x()>this->width()-boundaryWidth&&e->y()>=this->height()-boundaryWidth){
        //rightBottom
        m_boundType = RightBottomBound;
        resize(m_priSize.width()+deltaX,m_priSize.height()+deltaY);
    }else if(e->x()>this->width()-boundaryWidth){
        //right
//        qDebug()<<"right .... ";
        if(m_boundType == LeftBound && this->width()<=m_widthRange.x()){//width不能为负
            return QWidget::mouseMoveEvent(e);
        }
        m_boundType = RightBound;
        resize(m_priSize.width()+deltaX,m_priSize.height());
    }

    //边界判断
    if(this->geometry().width()<m_widthRange.x()&&this->geometry().height()<m_heightRange.x()){
        resize(m_widthRange.x(),m_heightRange.x());
    }else if (this->geometry().width()<m_widthRange.x()) {
        if(m_boundType == RightBound){
            resize(m_widthRange.x(),this->height());
        }else if(m_boundType == LeftBound){//左边移动时,geometry的横坐标不能动
            setGeometry(m_widgetPos.x(),m_widgetPos.y(),m_widthRange.x(),this->height());
        }
    }else if (this->geometry().width()>m_widthRange.y()) {
        resize(m_widthRange.y(),this->height());
    }else if (this->geometry().height()<m_heightRange.x()) {
        resize(this->width(),m_heightRange.x());
    }else if (this->geometry().height()>m_heightRange.y()) {
        resize(this->width(),m_heightRange.y());
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
