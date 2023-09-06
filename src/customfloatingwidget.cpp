#include "DockAreaWidget.h"
#include "customfloatingwidget.h"
#include "titlebar.h"
#include <QVBoxLayout>

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

    titleBar->SetDialogText(windowTitle());
}


}

