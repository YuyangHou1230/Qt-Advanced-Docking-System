#include "../../examples/simple/MainWindow.h"

#include "ui_MainWindow.h"

#include <QLabel>
#include <QTimer>
using namespace ads;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ads::CDockManager::setConfigFlag(ads::CDockManager::UserCustomFloatingTitle);
    // Create the dock manager. Because the parent parameter is a QMainWindow
    // the dock manager registers itself as the central widget.
	m_DockManager = new ads::CDockManager(this);

	// Create example content label - this can be any application specific
	// widget
	QLabel* l = new QLabel();
	l->setWordWrap(true);
	l->setAlignment(Qt::AlignTop | Qt::AlignLeft);
	l->setText("Lorem ipsum dolor sit amet, consectetuer adipiscing elit. ");

	// Create a dock widget with the title Label 1 and set the created label
	// as the dock widget content
	ads::CDockWidget* DockWidget = new ads::CDockWidget("Label 1");
	DockWidget->setWidget(l);
    ui->menuView->addAction(DockWidget->toggleViewAction());
    m_DockManager->addDockWidgetTab(ads::TopDockWidgetArea, DockWidget);

    DockWidget = new ads::CDockWidget("Label 2");
    l = new QLabel();
    l->setWordWrap(true);
    l->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    l->setText("Lorem ipsum dolor sit amet, consectetuer adipiscing elit. ");
    DockWidget->setWidget(l);
    ui->menuView->addAction(DockWidget->toggleViewAction());
    m_DockManager->addDockWidgetTab(ads::TopDockWidgetArea, DockWidget);

    DockWidget = new ads::CDockWidget("Label 3");
    l = new QLabel();
    l->setWordWrap(true);
    l->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    l->setText("Lorem ipsum dolor sit amet, consectetuer adipiscing elit. ");
    DockWidget->setWidget(l);
	// Add the toggleViewAction of the dock widget to the menu to give
	// the user the possibility to show the dock widget if it has been closed
    ui->menuView->addAction(DockWidget->toggleViewAction());
    m_DockManager->addDockWidgetTab(ads::TopDockWidgetArea, DockWidget);
	// Add the dock widget to the top dock widget area


}

MainWindow::~MainWindow()
{
    delete ui;
}

