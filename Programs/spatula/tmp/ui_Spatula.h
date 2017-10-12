/********************************************************************************
** Form generated from reading ui file 'Spatula.ui'
**
** Created: Sun Jan 30 19:54:44 2011
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_SPATULA_H
#define UI_SPATULA_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Spatula
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout_3;
    QTabWidget *tabWidget;
    QWidget *tab;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox_3;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout;
    QLabel *label_3;
    QLineEdit *dirLineEdit;
    QPushButton *dirButton;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *xyzButton;
    QPushButton *goButton;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_2;
    QPlainTextEdit *progressPlainTextEdit;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout;
    QLabel *currentLabel;
    QProgressBar *actionProgressBar;
    QLabel *label;
    QProgressBar *fileProgressBar;
    QLabel *label_2;
    QProgressBar *totalProgressBar;
    QWidget *tab_2;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_4;
    QComboBox *comboBox;
    QSpacerItem *horizontalSpacer;
    QGroupBox *groupBox_4;
    QGridLayout *gridLayout_4;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_5;
    QDoubleSpinBox *alphaDoubleSpinBox;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *verticalSpacer;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *Spatula)
    {
        if (Spatula->objectName().isEmpty())
            Spatula->setObjectName(QString::fromUtf8("Spatula"));
        Spatula->resize(610, 597);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Spatula->sizePolicy().hasHeightForWidth());
        Spatula->setSizePolicy(sizePolicy);
        Spatula->setMinimumSize(QSize(0, 0));
        centralwidget = new QWidget(Spatula);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout_3 = new QGridLayout(centralwidget);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        verticalLayout = new QVBoxLayout(tab);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        groupBox_3 = new QGroupBox(tab);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        verticalLayout_3 = new QVBoxLayout(groupBox_3);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label_3 = new QLabel(groupBox_3);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout->addWidget(label_3);

        dirLineEdit = new QLineEdit(groupBox_3);
        dirLineEdit->setObjectName(QString::fromUtf8("dirLineEdit"));

        horizontalLayout->addWidget(dirLineEdit);

        dirButton = new QPushButton(groupBox_3);
        dirButton->setObjectName(QString::fromUtf8("dirButton"));

        horizontalLayout->addWidget(dirButton);


        verticalLayout_3->addLayout(horizontalLayout);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_3);

        xyzButton = new QPushButton(groupBox_3);
        xyzButton->setObjectName(QString::fromUtf8("xyzButton"));

        horizontalLayout_4->addWidget(xyzButton);

        goButton = new QPushButton(groupBox_3);
        goButton->setObjectName(QString::fromUtf8("goButton"));

        horizontalLayout_4->addWidget(goButton);


        verticalLayout_3->addLayout(horizontalLayout_4);


        verticalLayout->addWidget(groupBox_3);

        groupBox = new QGroupBox(tab);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout_2 = new QGridLayout(groupBox);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        progressPlainTextEdit = new QPlainTextEdit(groupBox);
        progressPlainTextEdit->setObjectName(QString::fromUtf8("progressPlainTextEdit"));
        progressPlainTextEdit->setUndoRedoEnabled(false);
        progressPlainTextEdit->setReadOnly(true);

        gridLayout_2->addWidget(progressPlainTextEdit, 1, 0, 1, 1);


        verticalLayout->addWidget(groupBox);

        groupBox_2 = new QGroupBox(tab);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        gridLayout = new QGridLayout(groupBox_2);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        currentLabel = new QLabel(groupBox_2);
        currentLabel->setObjectName(QString::fromUtf8("currentLabel"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(currentLabel->sizePolicy().hasHeightForWidth());
        currentLabel->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(currentLabel, 0, 0, 1, 1);

        actionProgressBar = new QProgressBar(groupBox_2);
        actionProgressBar->setObjectName(QString::fromUtf8("actionProgressBar"));
        QSizePolicy sizePolicy2(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(actionProgressBar->sizePolicy().hasHeightForWidth());
        actionProgressBar->setSizePolicy(sizePolicy2);
        actionProgressBar->setValue(24);

        gridLayout->addWidget(actionProgressBar, 0, 1, 1, 1);

        label = new QLabel(groupBox_2);
        label->setObjectName(QString::fromUtf8("label"));
        sizePolicy1.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(label, 1, 0, 1, 1);

        fileProgressBar = new QProgressBar(groupBox_2);
        fileProgressBar->setObjectName(QString::fromUtf8("fileProgressBar"));
        sizePolicy2.setHeightForWidth(fileProgressBar->sizePolicy().hasHeightForWidth());
        fileProgressBar->setSizePolicy(sizePolicy2);
        fileProgressBar->setValue(24);

        gridLayout->addWidget(fileProgressBar, 1, 1, 1, 1);

        label_2 = new QLabel(groupBox_2);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        sizePolicy1.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(label_2, 2, 0, 1, 1);

        totalProgressBar = new QProgressBar(groupBox_2);
        totalProgressBar->setObjectName(QString::fromUtf8("totalProgressBar"));
        sizePolicy2.setHeightForWidth(totalProgressBar->sizePolicy().hasHeightForWidth());
        totalProgressBar->setSizePolicy(sizePolicy2);
        totalProgressBar->setValue(24);

        gridLayout->addWidget(totalProgressBar, 2, 1, 1, 1);


        verticalLayout->addWidget(groupBox_2);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        verticalLayout_2 = new QVBoxLayout(tab_2);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_4 = new QLabel(tab_2);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        horizontalLayout_3->addWidget(label_4);

        comboBox = new QComboBox(tab_2);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));

        horizontalLayout_3->addWidget(comboBox);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);


        verticalLayout_2->addLayout(horizontalLayout_3);

        groupBox_4 = new QGroupBox(tab_2);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        gridLayout_4 = new QGridLayout(groupBox_4);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_5 = new QLabel(groupBox_4);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        horizontalLayout_2->addWidget(label_5);

        alphaDoubleSpinBox = new QDoubleSpinBox(groupBox_4);
        alphaDoubleSpinBox->setObjectName(QString::fromUtf8("alphaDoubleSpinBox"));
        alphaDoubleSpinBox->setDecimals(4);
        alphaDoubleSpinBox->setMaximum(1000);
        alphaDoubleSpinBox->setSingleStep(0.1);
        alphaDoubleSpinBox->setValue(0.2);

        horizontalLayout_2->addWidget(alphaDoubleSpinBox);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);


        gridLayout_4->addLayout(horizontalLayout_2, 0, 0, 1, 1);


        verticalLayout_2->addWidget(groupBox_4);

        verticalSpacer = new QSpacerItem(20, 339, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);

        tabWidget->addTab(tab_2, QString());

        gridLayout_3->addWidget(tabWidget, 0, 0, 1, 1);

        Spatula->setCentralWidget(centralwidget);
        menubar = new QMenuBar(Spatula);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 610, 22));
        Spatula->setMenuBar(menubar);
        statusbar = new QStatusBar(Spatula);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        Spatula->setStatusBar(statusbar);

        retranslateUi(Spatula);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(Spatula);
    } // setupUi

    void retranslateUi(QMainWindow *Spatula)
    {
        Spatula->setWindowTitle(QApplication::translate("Spatula", "Spatula", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QApplication::translate("Spatula", "Select Working Directory", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("Spatula", "Directory", 0, QApplication::UnicodeUTF8));
        dirButton->setText(QApplication::translate("Spatula", "Select", 0, QApplication::UnicodeUTF8));
        xyzButton->setText(QApplication::translate("Spatula", "Get XYZ", 0, QApplication::UnicodeUTF8));
        goButton->setText(QApplication::translate("Spatula", "Get Colors", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("Spatula", "Log", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("Spatula", "Progress", 0, QApplication::UnicodeUTF8));
        currentLabel->setText(QApplication::translate("Spatula", "Action", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("Spatula", "File", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("Spatula", "Total", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("Spatula", "Files", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("Spatula", "Surface algorithm", 0, QApplication::UnicodeUTF8));
        comboBox->clear();
        comboBox->insertItems(0, QStringList()
         << QApplication::translate("Spatula", "Alpha shape", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Spatula", "Marching cube", 0, QApplication::UnicodeUTF8)
        );
        groupBox_4->setTitle(QApplication::translate("Spatula", "Alpha shape controls", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("Spatula", "Alpha", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("Spatula", "Surface", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(Spatula);
    } // retranslateUi

};

namespace Ui {
    class Spatula: public Ui_Spatula {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SPATULA_H
