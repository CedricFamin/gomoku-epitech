/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Tue 12. Jun 16:04:40 2012
**      by: Qt User Interface Compiler version 4.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QSpacerItem>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionExit;
    QAction *actionEndgame_catch_rule;
    QAction *actionDouble_three_rule;
    QAction *actionAbout_Qt;
    QAction *actionNew_game;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_2;
    QLabel *label_9;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_3;
    QLabel *label_8;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_4;
    QLabel *label_7;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_5;
    QLabel *label_6;
    QSpacerItem *verticalSpacer_2;
    QVBoxLayout *verticalLayout_2;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout;
    QLabel *label_10;
    QLabel *label_11;
    QVBoxLayout *verticalLayout_4;
    QLabel *label_12;
    QLabel *label_13;
    QLabel *label_14;
    QVBoxLayout *verticalLayout_3;
    QSpacerItem *verticalSpacer_3;
    QLabel *label_17;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_16;
    QLabel *label_20;
    QHBoxLayout *horizontalLayout_9;
    QLabel *label_18;
    QLabel *label_19;
    QMenuBar *menuBar;
    QMenu *menuFichier;
    QMenu *menuRules;
    QMenu *menuHelp;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1063, 648);
        MainWindow->setAutoFillBackground(false);
        MainWindow->setStyleSheet(QString::fromUtf8(""));
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        actionEndgame_catch_rule = new QAction(MainWindow);
        actionEndgame_catch_rule->setObjectName(QString::fromUtf8("actionEndgame_catch_rule"));
        actionDouble_three_rule = new QAction(MainWindow);
        actionDouble_three_rule->setObjectName(QString::fromUtf8("actionDouble_three_rule"));
        actionAbout_Qt = new QAction(MainWindow);
        actionAbout_Qt->setObjectName(QString::fromUtf8("actionAbout_Qt"));
        actionNew_game = new QAction(MainWindow);
        actionNew_game->setObjectName(QString::fromUtf8("actionNew_game"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        centralWidget->setStyleSheet(QString::fromUtf8(""));
        horizontalLayout_2 = new QHBoxLayout(centralWidget);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        label = new QLabel(centralWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setStyleSheet(QString::fromUtf8("background-color : white"));

        horizontalLayout_8->addWidget(label);


        verticalLayout->addLayout(horizontalLayout_8);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setStyleSheet(QString::fromUtf8("background-color : white"));

        horizontalLayout_7->addWidget(label_2);

        label_9 = new QLabel(centralWidget);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setStyleSheet(QString::fromUtf8("background-color : white"));

        horizontalLayout_7->addWidget(label_9);


        verticalLayout->addLayout(horizontalLayout_7);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setStyleSheet(QString::fromUtf8("background-color : white"));

        horizontalLayout_6->addWidget(label_3);

        label_8 = new QLabel(centralWidget);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setStyleSheet(QString::fromUtf8("background-color : white"));

        horizontalLayout_6->addWidget(label_8);


        verticalLayout->addLayout(horizontalLayout_6);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setStyleSheet(QString::fromUtf8("background-color : white"));

        horizontalLayout_5->addWidget(label_4);

        label_7 = new QLabel(centralWidget);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setStyleSheet(QString::fromUtf8("background-color : white"));

        horizontalLayout_5->addWidget(label_7);


        verticalLayout->addLayout(horizontalLayout_5);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_5 = new QLabel(centralWidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setStyleSheet(QString::fromUtf8("background-color : white"));

        horizontalLayout_4->addWidget(label_5);

        label_6 = new QLabel(centralWidget);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setStyleSheet(QString::fromUtf8("background-color : white"));

        horizontalLayout_4->addWidget(label_6);


        verticalLayout->addLayout(horizontalLayout_4);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);


        horizontalLayout_2->addLayout(verticalLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label_10 = new QLabel(centralWidget);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setStyleSheet(QString::fromUtf8("background-color : white"));

        horizontalLayout->addWidget(label_10);

        label_11 = new QLabel(centralWidget);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setStyleSheet(QString::fromUtf8("background-color : white"));

        horizontalLayout->addWidget(label_11);


        verticalLayout_2->addLayout(horizontalLayout);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        label_12 = new QLabel(centralWidget);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setStyleSheet(QString::fromUtf8("background-color : white"));

        verticalLayout_4->addWidget(label_12);

        label_13 = new QLabel(centralWidget);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setStyleSheet(QString::fromUtf8("background-color : white"));

        verticalLayout_4->addWidget(label_13);

        label_14 = new QLabel(centralWidget);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setStyleSheet(QString::fromUtf8("background-color : white"));

        verticalLayout_4->addWidget(label_14);


        verticalLayout_2->addLayout(verticalLayout_4);


        horizontalLayout_2->addLayout(verticalLayout_2);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer_3);

        label_17 = new QLabel(centralWidget);
        label_17->setObjectName(QString::fromUtf8("label_17"));
        label_17->setStyleSheet(QString::fromUtf8("background-color : white"));

        verticalLayout_3->addWidget(label_17);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_16 = new QLabel(centralWidget);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        label_16->setStyleSheet(QString::fromUtf8("background-color : white"));

        horizontalLayout_3->addWidget(label_16);

        label_20 = new QLabel(centralWidget);
        label_20->setObjectName(QString::fromUtf8("label_20"));
        label_20->setStyleSheet(QString::fromUtf8("background-color : white"));

        horizontalLayout_3->addWidget(label_20);


        verticalLayout_3->addLayout(horizontalLayout_3);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setSpacing(6);
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        label_18 = new QLabel(centralWidget);
        label_18->setObjectName(QString::fromUtf8("label_18"));
        label_18->setStyleSheet(QString::fromUtf8("background-color : white"));

        horizontalLayout_9->addWidget(label_18);

        label_19 = new QLabel(centralWidget);
        label_19->setObjectName(QString::fromUtf8("label_19"));
        label_19->setStyleSheet(QString::fromUtf8("background-color : white"));

        horizontalLayout_9->addWidget(label_19);


        verticalLayout_3->addLayout(horizontalLayout_9);


        horizontalLayout_2->addLayout(verticalLayout_3);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1063, 23));
        menuFichier = new QMenu(menuBar);
        menuFichier->setObjectName(QString::fromUtf8("menuFichier"));
        menuRules = new QMenu(menuBar);
        menuRules->setObjectName(QString::fromUtf8("menuRules"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuFichier->menuAction());
        menuBar->addAction(menuRules->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFichier->addAction(actionNew_game);
        menuFichier->addAction(actionExit);
        menuRules->addAction(actionEndgame_catch_rule);
        menuRules->addAction(actionDouble_three_rule);
        menuHelp->addAction(actionAbout_Qt);

        retranslateUi(MainWindow);
        QObject::connect(actionExit, SIGNAL(triggered()), MainWindow, SLOT(update()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        actionExit->setText(QApplication::translate("MainWindow", "Exit", 0, QApplication::UnicodeUTF8));
        actionExit->setShortcut(QApplication::translate("MainWindow", "Ctrl+Q", 0, QApplication::UnicodeUTF8));
        actionEndgame_catch_rule->setText(QApplication::translate("MainWindow", "Endgame catch rule", 0, QApplication::UnicodeUTF8));
        actionEndgame_catch_rule->setShortcut(QApplication::translate("MainWindow", "Ctrl+R", 0, QApplication::UnicodeUTF8));
        actionDouble_three_rule->setText(QApplication::translate("MainWindow", "Double-three rule", 0, QApplication::UnicodeUTF8));
        actionDouble_three_rule->setShortcut(QApplication::translate("MainWindow", "Ctrl+C", 0, QApplication::UnicodeUTF8));
        actionAbout_Qt->setText(QApplication::translate("MainWindow", "About Qt", 0, QApplication::UnicodeUTF8));
        actionAbout_Qt->setShortcut(QApplication::translate("MainWindow", "Ctrl+H", 0, QApplication::UnicodeUTF8));
        actionNew_game->setText(QApplication::translate("MainWindow", "New game", 0, QApplication::UnicodeUTF8));
        actionNew_game->setShortcut(QApplication::translate("MainWindow", "F2", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindow", "Selected Intersection informations :", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MainWindow", "X :", 0, QApplication::UnicodeUTF8));
        label_9->setText(QString());
        label_3->setText(QApplication::translate("MainWindow", "Y :", 0, QApplication::UnicodeUTF8));
        label_8->setText(QString());
        label_4->setText(QApplication::translate("MainWindow", "Belongs to :", 0, QApplication::UnicodeUTF8));
        label_7->setText(QString());
        label_5->setText(QApplication::translate("MainWindow", "Is playable :", 0, QApplication::UnicodeUTF8));
        label_6->setText(QString());
        label_10->setText(QApplication::translate("MainWindow", "Player Turn :", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("MainWindow", "Player 1", 0, QApplication::UnicodeUTF8));
        label_12->setText(QApplication::translate("MainWindow", "Activated Rules :", 0, QApplication::UnicodeUTF8));
        label_13->setText(QApplication::translate("MainWindow", "-Endgame catch rule", 0, QApplication::UnicodeUTF8));
        label_14->setText(QApplication::translate("MainWindow", "-Double-three rule", 0, QApplication::UnicodeUTF8));
        label_17->setText(QApplication::translate("MainWindow", "Captured stones :", 0, QApplication::UnicodeUTF8));
        label_16->setText(QApplication::translate("MainWindow", "Player 1 :", 0, QApplication::UnicodeUTF8));
        label_20->setText(QString());
        label_18->setText(QApplication::translate("MainWindow", "Player 2 :", 0, QApplication::UnicodeUTF8));
        label_19->setText(QString());
#ifndef QT_NO_TOOLTIP
        menuFichier->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        menuFichier->setTitle(QApplication::translate("MainWindow", "File", 0, QApplication::UnicodeUTF8));
        menuRules->setTitle(QApplication::translate("MainWindow", "Rules", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        menuHelp->setAccessibleName(QString());
#endif // QT_NO_ACCESSIBILITY
        menuHelp->setTitle(QApplication::translate("MainWindow", "Help", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
