#ifndef CUSTOMFLOATINGWIDGET_H
#define CUSTOMFLOATINGWIDGET_H

#include "FloatingDockContainer.h"

class TableTitleBar;

namespace ads {

class ADS_EXPORT CustomFloatingWidget : public CFloatingDockContainer
{
    Q_OBJECT
public:
    /**
     * Create empty floating widget - required for restore state
     */
    CustomFloatingWidget(CDockManager* DockManager);

    /**
     * Create floating widget with the given dock area
     */
    CustomFloatingWidget(CDockAreaWidget* DockArea);

    /**
     * Create floating widget with the given dock widget
     */
    CustomFloatingWidget(CDockWidget* DockWidget);

    virtual void reflectWindowsTitle(QString text) override;

    //改变鼠标样式
    void setCursorIcon(QMouseEvent *e);

private slots:
    void updateTitle(QString text);

private:
    void init();
    //鼠标相对窗体的位置
    QPoint m_pos;
    //仅记录第一次按下坐标(move也会触发press信号)
    bool m_isMoving;
    //上一次的宽度
    QRect m_priSize;

    TableTitleBar* titleBar;

protected:
     virtual bool nativeEvent(const QByteArray &eventType, void *message, long *result) override;

     void mousePressEvent(QMouseEvent* e) override;
     void mouseMoveEvent(QMouseEvent* e) override;
     void mouseReleaseEvent(QMouseEvent* e) override;

     void enterEvent(QEvent *event) override;
     void leaveEvent(QEvent *event) override;
};


}



#endif // CUSTOMFLOATINGWIDGET_H
