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
    m_isPress = false;
    m_boundType = NoBound;
//    setMinimumSize(1000,1000);
}

bool CustomFloatingWidget::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    QWidget::nativeEvent(eventType, message, result);
    return false;
}

void CustomFloatingWidget::mousePressEvent(QMouseEvent *e)
{
    qDebug()<<"PRESS m_isMoving = " <<m_isMoving;
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
        m_isPress = true;
    }
    setCursorIcon(e);
    return QWidget::mousePressEvent(e);
}

void CustomFloatingWidget::mouseMoveEvent(QMouseEvent *e)
{
    //确认鼠标已经按下后刷新m_isMoveing
    if(m_isPress){
        m_isMoving = true;

        qDebug()<<" MOVE m_isMoving = " <<m_isMoving;
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
    }
    return QWidget::mouseMoveEvent(e);
}

void CustomFloatingWidget::mouseReleaseEvent(QMouseEvent *e)
{
    m_isMoving = false;
    m_isPress = false;
    m_boundType = NoBound;
    setCursor(Qt::ArrowCursor);
}


}
