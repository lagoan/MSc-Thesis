/********************************************************************************
** Form generated from reading ui file 'MainWindow.ui'
**
** Created: Tue Oct 26 16:17:47 2010
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDockWidget>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QFrame>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QScrollArea>
#include <QtGui/QSlider>
#include <QtGui/QSpacerItem>
#include <QtGui/QTableWidget>
#include <QtGui/QTextEdit>
#include <QtGui/QToolBar>
#include <QtGui/QTreeWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "ViewPort.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionOpen;
    QAction *actionSave;
    QAction *actionSaveAs;
    QAction *actionQuit;
    QAction *actionHelp;
    QAction *actionAbout;
    QAction *actionAnimation_Controls;
    QAction *actionFrame_Controls;
    QAction *actionObjectInspector;
    QAction *actionPropertyEditor;
    QAction *actionPlay;
    QAction *actionFirst;
    QAction *actionLast;
    QAction *actionRecord;
    QAction *actionParticles;
    QAction *actionSurface;
    QAction *actionSaveBubbles;
    QAction *actionLiveSave;
    QAction *actionBubbleParticles;
    QAction *actionPosition;
    QAction *actionSize;
    QAction *actionTemperature;
    QAction *actionCO2;
    QAction *actionOn_surface;
    QAction *actionIn_collision;
    QAction *actionTrue_surface;
    QAction *actionCut_outs;
    QAction *actionVelocity;
    QAction *actionGradient;
    QWidget *centralwidget;
    QHBoxLayout *hboxLayout;
    ViewPort *viewPortWidget;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuHelp;
    QMenu *menuTools;
    QMenu *menuView;
    QMenu *menu_Particle_properties;
    QDockWidget *dockWidgetObjectInspector;
    QWidget *dockWidgetContentsObjectInspector;
    QVBoxLayout *vboxLayout;
    QFrame *frameObjectInspector;
    QVBoxLayout *vboxLayout1;
    QTreeWidget *treeWidgetObjectInspector;
    QDockWidget *dockWidgetPropertyEditor;
    QWidget *dockWidgetContentsPropertyEditor;
    QVBoxLayout *vboxLayout2;
    QFrame *framePropertyEditor;
    QVBoxLayout *vboxLayout3;
    QTableWidget *tableWidgetPropertyEditor;
    QToolBar *toolBarAnimationControls;
    QDockWidget *dockWidgetConsole;
    QWidget *dockWidgetContentsConsole;
    QHBoxLayout *hboxLayout1;
    QFrame *frameConsole;
    QHBoxLayout *hboxLayout2;
    QTextEdit *textEditConsole;
    QDockWidget *dockWidgetRenderingOptions;
    QWidget *dockWidgetContents;
    QVBoxLayout *verticalLayout_3;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_2;
    QLabel *labelSplatRadius;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout;
    QSlider *horizontalSliderSplatRadius;
    QDoubleSpinBox *doubleSpinBoxSplatRadius;
    QHBoxLayout *horizontalLayout_3;
    QLabel *labelCameraPosition;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_3;
    QDoubleSpinBox *doubleSpinBoxPosX;
    QLabel *label_2;
    QDoubleSpinBox *doubleSpinBoxPosY;
    QLabel *label;
    QDoubleSpinBox *doubleSpinBoxPosZ;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_4;
    QSpacerItem *horizontalSpacer_6;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_8;
    QDoubleSpinBox *doubleSpinBoxRot_0_0;
    QDoubleSpinBox *doubleSpinBoxRot_0_1;
    QDoubleSpinBox *doubleSpinBoxRot_0_2;
    QHBoxLayout *horizontalLayout_9;
    QDoubleSpinBox *doubleSpinBoxRot_1_0;
    QDoubleSpinBox *doubleSpinBoxRot_1_1;
    QDoubleSpinBox *doubleSpinBoxRot_1_2;
    QHBoxLayout *horizontalLayout_10;
    QDoubleSpinBox *doubleSpinBoxRot_2_0;
    QDoubleSpinBox *doubleSpinBoxRot_2_1;
    QDoubleSpinBox *doubleSpinBoxRot_2_2;
    QSpacerItem *verticalSpacer;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(794, 841);
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
        actionSave = new QAction(MainWindow);
        actionSave->setObjectName(QString::fromUtf8("actionSave"));
        actionSaveAs = new QAction(MainWindow);
        actionSaveAs->setObjectName(QString::fromUtf8("actionSaveAs"));
        actionQuit = new QAction(MainWindow);
        actionQuit->setObjectName(QString::fromUtf8("actionQuit"));
        actionHelp = new QAction(MainWindow);
        actionHelp->setObjectName(QString::fromUtf8("actionHelp"));
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
        actionAnimation_Controls = new QAction(MainWindow);
        actionAnimation_Controls->setObjectName(QString::fromUtf8("actionAnimation_Controls"));
        actionAnimation_Controls->setCheckable(true);
        actionAnimation_Controls->setChecked(true);
        actionFrame_Controls = new QAction(MainWindow);
        actionFrame_Controls->setObjectName(QString::fromUtf8("actionFrame_Controls"));
        actionFrame_Controls->setCheckable(true);
        actionFrame_Controls->setChecked(true);
        actionObjectInspector = new QAction(MainWindow);
        actionObjectInspector->setObjectName(QString::fromUtf8("actionObjectInspector"));
        actionObjectInspector->setCheckable(true);
        actionObjectInspector->setChecked(true);
        actionPropertyEditor = new QAction(MainWindow);
        actionPropertyEditor->setObjectName(QString::fromUtf8("actionPropertyEditor"));
        actionPropertyEditor->setCheckable(true);
        actionPropertyEditor->setChecked(true);
        actionPlay = new QAction(MainWindow);
        actionPlay->setObjectName(QString::fromUtf8("actionPlay"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/img/icons/icons/play.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPlay->setIcon(icon);
        actionFirst = new QAction(MainWindow);
        actionFirst->setObjectName(QString::fromUtf8("actionFirst"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/img/icons/icons/first.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionFirst->setIcon(icon1);
        actionLast = new QAction(MainWindow);
        actionLast->setObjectName(QString::fromUtf8("actionLast"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/img/icons/icons/last.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionLast->setIcon(icon2);
        actionRecord = new QAction(MainWindow);
        actionRecord->setObjectName(QString::fromUtf8("actionRecord"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/img/icons/icons/record.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionRecord->setIcon(icon3);
        actionParticles = new QAction(MainWindow);
        actionParticles->setObjectName(QString::fromUtf8("actionParticles"));
        actionParticles->setCheckable(true);
        actionParticles->setChecked(true);
        actionSurface = new QAction(MainWindow);
        actionSurface->setObjectName(QString::fromUtf8("actionSurface"));
        actionSurface->setCheckable(true);
        actionSurface->setChecked(true);
        actionSaveBubbles = new QAction(MainWindow);
        actionSaveBubbles->setObjectName(QString::fromUtf8("actionSaveBubbles"));
        actionLiveSave = new QAction(MainWindow);
        actionLiveSave->setObjectName(QString::fromUtf8("actionLiveSave"));
        actionLiveSave->setCheckable(true);
        actionBubbleParticles = new QAction(MainWindow);
        actionBubbleParticles->setObjectName(QString::fromUtf8("actionBubbleParticles"));
        actionBubbleParticles->setCheckable(true);
        actionBubbleParticles->setChecked(true);
        actionPosition = new QAction(MainWindow);
        actionPosition->setObjectName(QString::fromUtf8("actionPosition"));
        actionPosition->setCheckable(true);
        actionPosition->setChecked(true);
        actionSize = new QAction(MainWindow);
        actionSize->setObjectName(QString::fromUtf8("actionSize"));
        actionSize->setCheckable(true);
        actionTemperature = new QAction(MainWindow);
        actionTemperature->setObjectName(QString::fromUtf8("actionTemperature"));
        actionTemperature->setCheckable(true);
        actionCO2 = new QAction(MainWindow);
        actionCO2->setObjectName(QString::fromUtf8("actionCO2"));
        actionCO2->setCheckable(true);
        actionOn_surface = new QAction(MainWindow);
        actionOn_surface->setObjectName(QString::fromUtf8("actionOn_surface"));
        actionOn_surface->setCheckable(true);
        actionIn_collision = new QAction(MainWindow);
        actionIn_collision->setObjectName(QString::fromUtf8("actionIn_collision"));
        actionIn_collision->setCheckable(true);
        actionTrue_surface = new QAction(MainWindow);
        actionTrue_surface->setObjectName(QString::fromUtf8("actionTrue_surface"));
        actionTrue_surface->setCheckable(true);
        actionCut_outs = new QAction(MainWindow);
        actionCut_outs->setObjectName(QString::fromUtf8("actionCut_outs"));
        actionCut_outs->setCheckable(true);
        actionCut_outs->setChecked(true);
        actionVelocity = new QAction(MainWindow);
        actionVelocity->setObjectName(QString::fromUtf8("actionVelocity"));
        actionVelocity->setCheckable(true);
        actionVelocity->setChecked(true);
        actionGradient = new QAction(MainWindow);
        actionGradient->setObjectName(QString::fromUtf8("actionGradient"));
        actionGradient->setCheckable(true);
        actionGradient->setChecked(true);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        hboxLayout = new QHBoxLayout(centralwidget);
        hboxLayout->setMargin(0);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        viewPortWidget = new ViewPort(centralwidget);
        viewPortWidget->setObjectName(QString::fromUtf8("viewPortWidget"));

        hboxLayout->addWidget(viewPortWidget);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 794, 25));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuHelp = new QMenu(menubar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        menuTools = new QMenu(menubar);
        menuTools->setObjectName(QString::fromUtf8("menuTools"));
        menuView = new QMenu(menubar);
        menuView->setObjectName(QString::fromUtf8("menuView"));
        menu_Particle_properties = new QMenu(menubar);
        menu_Particle_properties->setObjectName(QString::fromUtf8("menu_Particle_properties"));
        MainWindow->setMenuBar(menubar);
        dockWidgetObjectInspector = new QDockWidget(MainWindow);
        dockWidgetObjectInspector->setObjectName(QString::fromUtf8("dockWidgetObjectInspector"));
        dockWidgetObjectInspector->setFloating(false);
        dockWidgetContentsObjectInspector = new QWidget();
        dockWidgetContentsObjectInspector->setObjectName(QString::fromUtf8("dockWidgetContentsObjectInspector"));
        vboxLayout = new QVBoxLayout(dockWidgetContentsObjectInspector);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        frameObjectInspector = new QFrame(dockWidgetContentsObjectInspector);
        frameObjectInspector->setObjectName(QString::fromUtf8("frameObjectInspector"));
        frameObjectInspector->setFrameShape(QFrame::StyledPanel);
        frameObjectInspector->setFrameShadow(QFrame::Raised);
        vboxLayout1 = new QVBoxLayout(frameObjectInspector);
        vboxLayout1->setMargin(0);
        vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
        treeWidgetObjectInspector = new QTreeWidget(frameObjectInspector);
        treeWidgetObjectInspector->setObjectName(QString::fromUtf8("treeWidgetObjectInspector"));
        treeWidgetObjectInspector->setAlternatingRowColors(true);
        treeWidgetObjectInspector->setUniformRowHeights(true);
        treeWidgetObjectInspector->setAnimated(false);

        vboxLayout1->addWidget(treeWidgetObjectInspector);


        vboxLayout->addWidget(frameObjectInspector);

        dockWidgetObjectInspector->setWidget(dockWidgetContentsObjectInspector);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(2), dockWidgetObjectInspector);
        dockWidgetPropertyEditor = new QDockWidget(MainWindow);
        dockWidgetPropertyEditor->setObjectName(QString::fromUtf8("dockWidgetPropertyEditor"));
        dockWidgetContentsPropertyEditor = new QWidget();
        dockWidgetContentsPropertyEditor->setObjectName(QString::fromUtf8("dockWidgetContentsPropertyEditor"));
        vboxLayout2 = new QVBoxLayout(dockWidgetContentsPropertyEditor);
        vboxLayout2->setObjectName(QString::fromUtf8("vboxLayout2"));
        framePropertyEditor = new QFrame(dockWidgetContentsPropertyEditor);
        framePropertyEditor->setObjectName(QString::fromUtf8("framePropertyEditor"));
        framePropertyEditor->setFrameShape(QFrame::StyledPanel);
        framePropertyEditor->setFrameShadow(QFrame::Raised);
        vboxLayout3 = new QVBoxLayout(framePropertyEditor);
        vboxLayout3->setMargin(0);
        vboxLayout3->setObjectName(QString::fromUtf8("vboxLayout3"));
        tableWidgetPropertyEditor = new QTableWidget(framePropertyEditor);
        tableWidgetPropertyEditor->setObjectName(QString::fromUtf8("tableWidgetPropertyEditor"));
        tableWidgetPropertyEditor->setAlternatingRowColors(true);
        tableWidgetPropertyEditor->setSelectionMode(QAbstractItemView::NoSelection);
        tableWidgetPropertyEditor->setShowGrid(false);
        tableWidgetPropertyEditor->setCornerButtonEnabled(false);

        vboxLayout3->addWidget(tableWidgetPropertyEditor);


        vboxLayout2->addWidget(framePropertyEditor);

        dockWidgetPropertyEditor->setWidget(dockWidgetContentsPropertyEditor);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(2), dockWidgetPropertyEditor);
        toolBarAnimationControls = new QToolBar(MainWindow);
        toolBarAnimationControls->setObjectName(QString::fromUtf8("toolBarAnimationControls"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(toolBarAnimationControls->sizePolicy().hasHeightForWidth());
        toolBarAnimationControls->setSizePolicy(sizePolicy);
        toolBarAnimationControls->setMovable(true);
        toolBarAnimationControls->setAllowedAreas(Qt::BottomToolBarArea|Qt::TopToolBarArea);
        toolBarAnimationControls->setIconSize(QSize(32, 32));
        MainWindow->addToolBar(Qt::BottomToolBarArea, toolBarAnimationControls);
        dockWidgetConsole = new QDockWidget(MainWindow);
        dockWidgetConsole->setObjectName(QString::fromUtf8("dockWidgetConsole"));
        dockWidgetContentsConsole = new QWidget();
        dockWidgetContentsConsole->setObjectName(QString::fromUtf8("dockWidgetContentsConsole"));
        hboxLayout1 = new QHBoxLayout(dockWidgetContentsConsole);
        hboxLayout1->setMargin(0);
        hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
        frameConsole = new QFrame(dockWidgetContentsConsole);
        frameConsole->setObjectName(QString::fromUtf8("frameConsole"));
        frameConsole->setFrameShape(QFrame::StyledPanel);
        frameConsole->setFrameShadow(QFrame::Raised);
        hboxLayout2 = new QHBoxLayout(frameConsole);
        hboxLayout2->setMargin(0);
        hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
        textEditConsole = new QTextEdit(frameConsole);
        textEditConsole->setObjectName(QString::fromUtf8("textEditConsole"));
        textEditConsole->setUndoRedoEnabled(false);
        textEditConsole->setReadOnly(true);

        hboxLayout2->addWidget(textEditConsole);


        hboxLayout1->addWidget(frameConsole);

        dockWidgetConsole->setWidget(dockWidgetContentsConsole);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(8), dockWidgetConsole);
        dockWidgetRenderingOptions = new QDockWidget(MainWindow);
        dockWidgetRenderingOptions->setObjectName(QString::fromUtf8("dockWidgetRenderingOptions"));
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
        verticalLayout_3 = new QVBoxLayout(dockWidgetContents);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        scrollArea = new QScrollArea(dockWidgetContents);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, -115, 338, 284));
        verticalLayout_2 = new QVBoxLayout(scrollAreaWidgetContents);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        labelSplatRadius = new QLabel(scrollAreaWidgetContents);
        labelSplatRadius->setObjectName(QString::fromUtf8("labelSplatRadius"));

        horizontalLayout_2->addWidget(labelSplatRadius);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);


        verticalLayout_2->addLayout(horizontalLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSliderSplatRadius = new QSlider(scrollAreaWidgetContents);
        horizontalSliderSplatRadius->setObjectName(QString::fromUtf8("horizontalSliderSplatRadius"));
        horizontalSliderSplatRadius->setMaximum(9999);
        horizontalSliderSplatRadius->setSingleStep(100);
        horizontalSliderSplatRadius->setPageStep(1000);
        horizontalSliderSplatRadius->setValue(0);
        horizontalSliderSplatRadius->setOrientation(Qt::Horizontal);

        horizontalLayout->addWidget(horizontalSliderSplatRadius);

        doubleSpinBoxSplatRadius = new QDoubleSpinBox(scrollAreaWidgetContents);
        doubleSpinBoxSplatRadius->setObjectName(QString::fromUtf8("doubleSpinBoxSplatRadius"));
        doubleSpinBoxSplatRadius->setValue(1);

        horizontalLayout->addWidget(doubleSpinBoxSplatRadius);


        verticalLayout_2->addLayout(horizontalLayout);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        labelCameraPosition = new QLabel(scrollAreaWidgetContents);
        labelCameraPosition->setObjectName(QString::fromUtf8("labelCameraPosition"));

        horizontalLayout_3->addWidget(labelCameraPosition);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_2);


        verticalLayout_2->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_3 = new QLabel(scrollAreaWidgetContents);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_4->addWidget(label_3);

        doubleSpinBoxPosX = new QDoubleSpinBox(scrollAreaWidgetContents);
        doubleSpinBoxPosX->setObjectName(QString::fromUtf8("doubleSpinBoxPosX"));
        QSizePolicy sizePolicy1(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(doubleSpinBoxPosX->sizePolicy().hasHeightForWidth());
        doubleSpinBoxPosX->setSizePolicy(sizePolicy1);
        doubleSpinBoxPosX->setMinimum(-1000);
        doubleSpinBoxPosX->setMaximum(1000);

        horizontalLayout_4->addWidget(doubleSpinBoxPosX);

        label_2 = new QLabel(scrollAreaWidgetContents);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_4->addWidget(label_2);

        doubleSpinBoxPosY = new QDoubleSpinBox(scrollAreaWidgetContents);
        doubleSpinBoxPosY->setObjectName(QString::fromUtf8("doubleSpinBoxPosY"));
        sizePolicy1.setHeightForWidth(doubleSpinBoxPosY->sizePolicy().hasHeightForWidth());
        doubleSpinBoxPosY->setSizePolicy(sizePolicy1);
        doubleSpinBoxPosY->setMinimum(-1000);
        doubleSpinBoxPosY->setMaximum(1000);

        horizontalLayout_4->addWidget(doubleSpinBoxPosY);

        label = new QLabel(scrollAreaWidgetContents);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_4->addWidget(label);

        doubleSpinBoxPosZ = new QDoubleSpinBox(scrollAreaWidgetContents);
        doubleSpinBoxPosZ->setObjectName(QString::fromUtf8("doubleSpinBoxPosZ"));
        sizePolicy1.setHeightForWidth(doubleSpinBoxPosZ->sizePolicy().hasHeightForWidth());
        doubleSpinBoxPosZ->setSizePolicy(sizePolicy1);
        doubleSpinBoxPosZ->setMinimum(-1000);
        doubleSpinBoxPosZ->setMaximum(1000);

        horizontalLayout_4->addWidget(doubleSpinBoxPosZ);


        verticalLayout_2->addLayout(horizontalLayout_4);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        label_4 = new QLabel(scrollAreaWidgetContents);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        horizontalLayout_7->addWidget(label_4);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_6);


        verticalLayout_2->addLayout(horizontalLayout_7);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        doubleSpinBoxRot_0_0 = new QDoubleSpinBox(scrollAreaWidgetContents);
        doubleSpinBoxRot_0_0->setObjectName(QString::fromUtf8("doubleSpinBoxRot_0_0"));
        sizePolicy1.setHeightForWidth(doubleSpinBoxRot_0_0->sizePolicy().hasHeightForWidth());
        doubleSpinBoxRot_0_0->setSizePolicy(sizePolicy1);
        doubleSpinBoxRot_0_0->setDecimals(4);
        doubleSpinBoxRot_0_0->setMinimum(-1);
        doubleSpinBoxRot_0_0->setMaximum(1);

        horizontalLayout_8->addWidget(doubleSpinBoxRot_0_0);

        doubleSpinBoxRot_0_1 = new QDoubleSpinBox(scrollAreaWidgetContents);
        doubleSpinBoxRot_0_1->setObjectName(QString::fromUtf8("doubleSpinBoxRot_0_1"));
        sizePolicy1.setHeightForWidth(doubleSpinBoxRot_0_1->sizePolicy().hasHeightForWidth());
        doubleSpinBoxRot_0_1->setSizePolicy(sizePolicy1);
        doubleSpinBoxRot_0_1->setDecimals(4);
        doubleSpinBoxRot_0_1->setMinimum(-1);
        doubleSpinBoxRot_0_1->setMaximum(1);

        horizontalLayout_8->addWidget(doubleSpinBoxRot_0_1);

        doubleSpinBoxRot_0_2 = new QDoubleSpinBox(scrollAreaWidgetContents);
        doubleSpinBoxRot_0_2->setObjectName(QString::fromUtf8("doubleSpinBoxRot_0_2"));
        sizePolicy1.setHeightForWidth(doubleSpinBoxRot_0_2->sizePolicy().hasHeightForWidth());
        doubleSpinBoxRot_0_2->setSizePolicy(sizePolicy1);
        doubleSpinBoxRot_0_2->setDecimals(4);
        doubleSpinBoxRot_0_2->setMinimum(-1);
        doubleSpinBoxRot_0_2->setMaximum(1);

        horizontalLayout_8->addWidget(doubleSpinBoxRot_0_2);


        verticalLayout->addLayout(horizontalLayout_8);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        doubleSpinBoxRot_1_0 = new QDoubleSpinBox(scrollAreaWidgetContents);
        doubleSpinBoxRot_1_0->setObjectName(QString::fromUtf8("doubleSpinBoxRot_1_0"));
        sizePolicy1.setHeightForWidth(doubleSpinBoxRot_1_0->sizePolicy().hasHeightForWidth());
        doubleSpinBoxRot_1_0->setSizePolicy(sizePolicy1);
        doubleSpinBoxRot_1_0->setDecimals(4);
        doubleSpinBoxRot_1_0->setMinimum(-1);
        doubleSpinBoxRot_1_0->setMaximum(1);

        horizontalLayout_9->addWidget(doubleSpinBoxRot_1_0);

        doubleSpinBoxRot_1_1 = new QDoubleSpinBox(scrollAreaWidgetContents);
        doubleSpinBoxRot_1_1->setObjectName(QString::fromUtf8("doubleSpinBoxRot_1_1"));
        sizePolicy1.setHeightForWidth(doubleSpinBoxRot_1_1->sizePolicy().hasHeightForWidth());
        doubleSpinBoxRot_1_1->setSizePolicy(sizePolicy1);
        doubleSpinBoxRot_1_1->setDecimals(4);
        doubleSpinBoxRot_1_1->setMinimum(-1);
        doubleSpinBoxRot_1_1->setMaximum(1);

        horizontalLayout_9->addWidget(doubleSpinBoxRot_1_1);

        doubleSpinBoxRot_1_2 = new QDoubleSpinBox(scrollAreaWidgetContents);
        doubleSpinBoxRot_1_2->setObjectName(QString::fromUtf8("doubleSpinBoxRot_1_2"));
        sizePolicy1.setHeightForWidth(doubleSpinBoxRot_1_2->sizePolicy().hasHeightForWidth());
        doubleSpinBoxRot_1_2->setSizePolicy(sizePolicy1);
        doubleSpinBoxRot_1_2->setDecimals(4);
        doubleSpinBoxRot_1_2->setMinimum(-1);
        doubleSpinBoxRot_1_2->setMaximum(1);

        horizontalLayout_9->addWidget(doubleSpinBoxRot_1_2);


        verticalLayout->addLayout(horizontalLayout_9);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        doubleSpinBoxRot_2_0 = new QDoubleSpinBox(scrollAreaWidgetContents);
        doubleSpinBoxRot_2_0->setObjectName(QString::fromUtf8("doubleSpinBoxRot_2_0"));
        sizePolicy1.setHeightForWidth(doubleSpinBoxRot_2_0->sizePolicy().hasHeightForWidth());
        doubleSpinBoxRot_2_0->setSizePolicy(sizePolicy1);
        doubleSpinBoxRot_2_0->setDecimals(4);
        doubleSpinBoxRot_2_0->setMinimum(-1);
        doubleSpinBoxRot_2_0->setMaximum(1);

        horizontalLayout_10->addWidget(doubleSpinBoxRot_2_0);

        doubleSpinBoxRot_2_1 = new QDoubleSpinBox(scrollAreaWidgetContents);
        doubleSpinBoxRot_2_1->setObjectName(QString::fromUtf8("doubleSpinBoxRot_2_1"));
        sizePolicy1.setHeightForWidth(doubleSpinBoxRot_2_1->sizePolicy().hasHeightForWidth());
        doubleSpinBoxRot_2_1->setSizePolicy(sizePolicy1);
        doubleSpinBoxRot_2_1->setDecimals(4);
        doubleSpinBoxRot_2_1->setMinimum(-1);
        doubleSpinBoxRot_2_1->setMaximum(1);

        horizontalLayout_10->addWidget(doubleSpinBoxRot_2_1);

        doubleSpinBoxRot_2_2 = new QDoubleSpinBox(scrollAreaWidgetContents);
        doubleSpinBoxRot_2_2->setObjectName(QString::fromUtf8("doubleSpinBoxRot_2_2"));
        sizePolicy1.setHeightForWidth(doubleSpinBoxRot_2_2->sizePolicy().hasHeightForWidth());
        doubleSpinBoxRot_2_2->setSizePolicy(sizePolicy1);
        doubleSpinBoxRot_2_2->setDecimals(4);
        doubleSpinBoxRot_2_2->setMinimum(-1);
        doubleSpinBoxRot_2_2->setMaximum(1);

        horizontalLayout_10->addWidget(doubleSpinBoxRot_2_2);


        verticalLayout->addLayout(horizontalLayout_10);


        verticalLayout_2->addLayout(verticalLayout);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);

        scrollArea->setWidget(scrollAreaWidgetContents);

        verticalLayout_3->addWidget(scrollArea);

        dockWidgetRenderingOptions->setWidget(dockWidgetContents);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(2), dockWidgetRenderingOptions);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuTools->menuAction());
        menubar->addAction(menuView->menuAction());
        menubar->addAction(menu_Particle_properties->menuAction());
        menubar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionSave);
        menuFile->addAction(actionSaveAs);
        menuFile->addAction(actionLiveSave);
        menuFile->addSeparator();
        menuFile->addAction(actionQuit);
        menuHelp->addAction(actionHelp);
        menuHelp->addSeparator();
        menuHelp->addAction(actionAbout);
        menuView->addAction(actionParticles);
        menuView->addAction(actionBubbleParticles);
        menuView->addAction(actionSurface);
        menuView->addAction(actionCut_outs);
        menuView->addAction(actionVelocity);
        menuView->addAction(actionGradient);
        menu_Particle_properties->addAction(actionPosition);
        menu_Particle_properties->addAction(actionSize);
        menu_Particle_properties->addAction(actionTemperature);
        menu_Particle_properties->addAction(actionCO2);
        menu_Particle_properties->addAction(actionOn_surface);
        menu_Particle_properties->addAction(actionIn_collision);
        menu_Particle_properties->addAction(actionTrue_surface);
        toolBarAnimationControls->addAction(actionFirst);
        toolBarAnimationControls->addAction(actionPlay);
        toolBarAnimationControls->addAction(actionLast);
        toolBarAnimationControls->addAction(actionRecord);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "oven", 0, QApplication::UnicodeUTF8));
        actionOpen->setText(QApplication::translate("MainWindow", "&Open...", 0, QApplication::UnicodeUTF8));
        actionSave->setText(QApplication::translate("MainWindow", "&Save", 0, QApplication::UnicodeUTF8));
        actionSaveAs->setText(QApplication::translate("MainWindow", "Save &as...", 0, QApplication::UnicodeUTF8));
        actionQuit->setText(QApplication::translate("MainWindow", "&Quit", 0, QApplication::UnicodeUTF8));
        actionHelp->setText(QApplication::translate("MainWindow", "Help", 0, QApplication::UnicodeUTF8));
        actionAbout->setText(QApplication::translate("MainWindow", "About", 0, QApplication::UnicodeUTF8));
        actionAnimation_Controls->setText(QApplication::translate("MainWindow", "Animation Controls", 0, QApplication::UnicodeUTF8));
        actionFrame_Controls->setText(QApplication::translate("MainWindow", "Frame Controls", 0, QApplication::UnicodeUTF8));
        actionObjectInspector->setText(QApplication::translate("MainWindow", "Object Inspector", 0, QApplication::UnicodeUTF8));
        actionPropertyEditor->setText(QApplication::translate("MainWindow", "Property Editor", 0, QApplication::UnicodeUTF8));
        actionPlay->setText(QApplication::translate("MainWindow", "Play", 0, QApplication::UnicodeUTF8));
        actionFirst->setText(QApplication::translate("MainWindow", "Start", 0, QApplication::UnicodeUTF8));
        actionLast->setText(QApplication::translate("MainWindow", "End", 0, QApplication::UnicodeUTF8));
        actionRecord->setText(QApplication::translate("MainWindow", "Bake", 0, QApplication::UnicodeUTF8));
        actionParticles->setText(QApplication::translate("MainWindow", "Fluid particles", 0, QApplication::UnicodeUTF8));
        actionSurface->setText(QApplication::translate("MainWindow", "Surface", 0, QApplication::UnicodeUTF8));
        actionSaveBubbles->setText(QApplication::translate("MainWindow", "Save bubbles", 0, QApplication::UnicodeUTF8));
        actionLiveSave->setText(QApplication::translate("MainWindow", "Live save", 0, QApplication::UnicodeUTF8));
        actionBubbleParticles->setText(QApplication::translate("MainWindow", "Bubble particles", 0, QApplication::UnicodeUTF8));
        actionPosition->setText(QApplication::translate("MainWindow", "Position", 0, QApplication::UnicodeUTF8));
        actionSize->setText(QApplication::translate("MainWindow", "Size", 0, QApplication::UnicodeUTF8));
        actionTemperature->setText(QApplication::translate("MainWindow", "Temperature", 0, QApplication::UnicodeUTF8));
        actionCO2->setText(QApplication::translate("MainWindow", "CO2", 0, QApplication::UnicodeUTF8));
        actionOn_surface->setText(QApplication::translate("MainWindow", "On surface", 0, QApplication::UnicodeUTF8));
        actionIn_collision->setText(QApplication::translate("MainWindow", "In collision", 0, QApplication::UnicodeUTF8));
        actionTrue_surface->setText(QApplication::translate("MainWindow", "True surface", 0, QApplication::UnicodeUTF8));
        actionCut_outs->setText(QApplication::translate("MainWindow", "Cut outs", 0, QApplication::UnicodeUTF8));
        actionVelocity->setText(QApplication::translate("MainWindow", "Velocity", 0, QApplication::UnicodeUTF8));
        actionGradient->setText(QApplication::translate("MainWindow", "Gradient", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("MainWindow", "&File", 0, QApplication::UnicodeUTF8));
        menuHelp->setTitle(QApplication::translate("MainWindow", "&Help", 0, QApplication::UnicodeUTF8));
        menuTools->setTitle(QApplication::translate("MainWindow", "&Tools", 0, QApplication::UnicodeUTF8));
        menuView->setTitle(QApplication::translate("MainWindow", "&Render", 0, QApplication::UnicodeUTF8));
        menu_Particle_properties->setTitle(QApplication::translate("MainWindow", "&Particle's", 0, QApplication::UnicodeUTF8));
        dockWidgetObjectInspector->setWindowTitle(QApplication::translate("MainWindow", "Object Inspector", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem = treeWidgetObjectInspector->headerItem();
        ___qtreewidgetitem->setText(0, QApplication::translate("MainWindow", "Tree", 0, QApplication::UnicodeUTF8));
        dockWidgetPropertyEditor->setWindowTitle(QApplication::translate("MainWindow", "Property Editor", 0, QApplication::UnicodeUTF8));
        toolBarAnimationControls->setWindowTitle(QApplication::translate("MainWindow", "Animation Controls", 0, QApplication::UnicodeUTF8));
        dockWidgetConsole->setWindowTitle(QApplication::translate("MainWindow", "Console", 0, QApplication::UnicodeUTF8));
        dockWidgetRenderingOptions->setWindowTitle(QApplication::translate("MainWindow", "Rendering Options", 0, QApplication::UnicodeUTF8));
        labelSplatRadius->setText(QApplication::translate("MainWindow", "Point radius:", 0, QApplication::UnicodeUTF8));
        labelCameraPosition->setText(QApplication::translate("MainWindow", "Camera position:", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("MainWindow", "X:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MainWindow", "Y:", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindow", "Z:", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("MainWindow", "Rotation matrix:", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
