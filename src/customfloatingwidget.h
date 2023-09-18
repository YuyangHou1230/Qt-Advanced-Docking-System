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

    //设置floating高度与宽度范围
    void setMaxWidth(int width);
    void setMinWidth(int width);

    void setMaxHeight(int height);
    void setMinHeight(int height);

    enum BoundType{
        NoBound = 0,
        LeftBound = 1,
        LeftBottomBound = 2,
        BottomBound = 3,
        RightBottomBound = 4,
        RightBound = 5
    };

private slots:
    void updateTitle(QString text);

private:
    void init();
    //窗体所在全局坐标
    QPoint m_widgetPos;
    //鼠标点击全局坐标
    QPoint m_clickPos;
    //仅记录第一次按下坐标(move也会触发press信号)
    bool m_isMoving;
    //上一次的宽度
    QRect m_priSize;

    QRect m_widthRange;//x:min y:max
    QRect m_heightRange;//x:min y:max

    TableTitleBar* titleBar;

    BoundType m_boundType;//按住边线类型
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
