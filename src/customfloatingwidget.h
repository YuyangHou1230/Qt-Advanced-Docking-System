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

private slots:
    void updateTitle(QString text);

private:
    void init();

    TableTitleBar* titleBar;
};


}



#endif // CUSTOMFLOATINGWIDGET_H
