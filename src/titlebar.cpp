#include "titlebar.h"
#include "ui_titlebar.h"
#include <QMouseEvent>
#include <QPaintEvent>
#include <QStyleOption>
#include <QPainter>
#include <QDebug>
TableTitleBar::TableTitleBar(QWidget *parent) :QWidget(parent),ui(new Ui::TableTitleBar)
{
    ui->setupUi(this);
    this->installEventFilter(this);

    mp_ParentWidget = this->parentWidget();
}

void TableTitleBar::SetDialogText(const QString &text)
{
    ui->TitleLabel->setText(text);
}

void TableTitleBar::setCloseButtonVisiable(bool b)
{
    ui->btnClose->setVisible(b);
    ui->line->setVisible(b);
}

TableTitleBar::~TableTitleBar()
{
    delete ui;
}

bool TableTitleBar::eventFilter(QObject *obj, QEvent *event)
{
    //判断部件->判断事件类型->执行对应动作
    if (obj == this)
    {

        if(event->type() == QEvent::MouseButtonPress)
        {
            QMouseEvent *ev = static_cast<QMouseEvent*>(event);
            if(ev && ev->button() == Qt::LeftButton){
                refPoint = ev->pos();//将鼠标进入标题栏并且点击时的坐标记录下来,为窗口拖动做准备,如果鼠标长按的话，该事件只会触发一次，刚好起到了记录鼠标按下时坐标的作用
                mousePressed = true;
                emit mouseLeftPress();
            }
        }

        if(event->type() == QEvent::MouseButtonRelease)
        {
            if(mousePressed){
                mousePressed = false;
                emit mouseLeftRelease();
            }

        }

        if(event->type()==QEvent::MouseMove)
        {
            if(mousePressed)//判断鼠标是否处于按下的状态
            {
                QMouseEvent *ev = static_cast<QMouseEvent*>(event);

                QPoint point = ev->pos();//鼠标移动后的坐标
                QPoint deltaPos = point - refPoint;


                QPoint newPos = mp_ParentWidget->pos() + deltaPos;
//                if(mp_ParentWidget->pos().y() > (1080 - 80) && deltaPos.y() > 0){
//                    newPos.setY(1080 - 80);
//                }
//                if(mp_ParentWidget->pos().x() > (1920 - mp_ParentWidget->width()/2) && deltaPos.x() > 0){
//                    newPos.setX(1920 - mp_ParentWidget->width()/2);
//                }



//                if(mp_ParentWidget->pos().y() <= 0 && deltaPos.y() < 0){
//                    newPos.setY(0);
//                }

//                if(mp_ParentWidget->pos().x() <= (0-mp_ParentWidget->width()/2) && deltaPos.x() < 0){
//                    newPos.setX(-mp_ParentWidget->width()/2);
//                }
                mp_ParentWidget->move(newPos);
                emit mouseMoving();
            }
        }
    }
    return false;
}

void TableTitleBar::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void TableTitleBar::on_btnClose_clicked()
{
    if(mp_ParentWidget->isWindow())
        mp_ParentWidget->close();
}
