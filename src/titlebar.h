/**********************************************************************
* @name:   TableTitleBar
* @brief:   对话框的自定义标题栏,提供
* @param:   无
* @return:  无
* @author:  lcc
* @date:    2022/08/22
* @notes:   该控件仅提供默认的背景色(255,255,255),对于继承QWidget的子控件，QT官方文档中存在以下描述：
* Supports only the background, background-clip and background-origin properties.
* 因此在这个文件中设置了背景图片样式表之后，也不会在使用的地方生效。该控件的背景图片需要在使用该控件的UI界面中通过以下语句单独添加：
* setStyleSheet("QWidget#TableTitleBar{border-image: url(:/image/titleBarIcon2.png);}")
* ---没有把样式表一次性安排到位非常抱歉QAQ---
***********************************************************************/
#ifndef TableTitleBar_H
#define TableTitleBar_H

#include <QWidget>

namespace Ui {
class TableTitleBar;
}

class TableTitleBar : public QWidget
{
    Q_OBJECT

public:
    explicit TableTitleBar(QWidget *parent = nullptr);
    void SetDialogText(const QString & text);
    void setCloseButtonVisiable(bool);
    ~TableTitleBar();
protected:
    bool eventFilter(QObject *obj, QEvent *event);
    void paintEvent(QPaintEvent*);


private slots:
    void on_btnClose_clicked();
signals:
    void mouseLeftPress();
    void mouseLeftRelease();
    void mouseMoving();
private:
    Ui::TableTitleBar *ui;
    QWidget *mp_ParentWidget;//标题栏所属的窗口对象
    QPoint refPoint = QPoint(0,0);//鼠标点击自定义标题栏时的坐标
    bool mousePressed = false;//鼠标是否处于按下的状态
};

#endif // TableTitleBar_H
