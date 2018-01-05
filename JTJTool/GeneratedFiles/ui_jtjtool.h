/********************************************************************************
** Form generated from reading UI file 'jtjtool.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_JTJTOOL_H
#define UI_JTJTOOL_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_JTJToolClass
{
public:
    QWidget *centralWidget;
    QPushButton *RunBtn;
    QLineEdit *CmdEdt;
    QTextBrowser *TextB;
    QLabel *ShowImg;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *JTJToolClass)
    {
        if (JTJToolClass->objectName().isEmpty())
            JTJToolClass->setObjectName(QStringLiteral("JTJToolClass"));
        JTJToolClass->resize(902, 832);
        centralWidget = new QWidget(JTJToolClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        RunBtn = new QPushButton(centralWidget);
        RunBtn->setObjectName(QStringLiteral("RunBtn"));
        RunBtn->setGeometry(QRect(350, 731, 93, 30));
        CmdEdt = new QLineEdit(centralWidget);
        CmdEdt->setObjectName(QStringLiteral("CmdEdt"));
        CmdEdt->setGeometry(QRect(10, 730, 331, 30));
        TextB = new QTextBrowser(centralWidget);
        TextB->setObjectName(QStringLiteral("TextB"));
        TextB->setGeometry(QRect(10, 0, 431, 721));
        ShowImg = new QLabel(centralWidget);
        ShowImg->setObjectName(QStringLiteral("ShowImg"));
        ShowImg->setGeometry(QRect(470, 0, 421, 751));
        JTJToolClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(JTJToolClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 902, 26));
        JTJToolClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(JTJToolClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        JTJToolClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(JTJToolClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        JTJToolClass->setStatusBar(statusBar);

        retranslateUi(JTJToolClass);

        QMetaObject::connectSlotsByName(JTJToolClass);
    } // setupUi

    void retranslateUi(QMainWindow *JTJToolClass)
    {
        JTJToolClass->setWindowTitle(QApplication::translate("JTJToolClass", "JTJTool", Q_NULLPTR));
        RunBtn->setText(QApplication::translate("JTJToolClass", "\346\211\247\350\241\214", Q_NULLPTR));
        ShowImg->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class JTJToolClass: public Ui_JTJToolClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_JTJTOOL_H
