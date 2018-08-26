/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QPushButton *captureButton;
    QPushButton *exitButton;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QLabel *capturedImg1;
    QLabel *capturedImg2;
    QLabel *capturedImg3;
    QLabel *realtimeImg;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(480, 320);
        QPalette palette;
        QBrush brush(QColor(255, 255, 255, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Base, brush);
        QBrush brush1(QColor(90, 90, 90, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        MainWindow->setPalette(palette);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        captureButton = new QPushButton(centralWidget);
        captureButton->setObjectName(QStringLiteral("captureButton"));
        captureButton->setGeometry(QRect(10, 280, 101, 31));
        exitButton = new QPushButton(centralWidget);
        exitButton->setObjectName(QStringLiteral("exitButton"));
        exitButton->setGeometry(QRect(130, 280, 101, 31));
        verticalLayoutWidget = new QWidget(centralWidget);
        verticalLayoutWidget->setObjectName(QStringLiteral("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(309, 10, 161, 301));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        capturedImg1 = new QLabel(verticalLayoutWidget);
        capturedImg1->setObjectName(QStringLiteral("capturedImg1"));

        verticalLayout->addWidget(capturedImg1);

        capturedImg2 = new QLabel(verticalLayoutWidget);
        capturedImg2->setObjectName(QStringLiteral("capturedImg2"));

        verticalLayout->addWidget(capturedImg2);

        capturedImg3 = new QLabel(verticalLayoutWidget);
        capturedImg3->setObjectName(QStringLiteral("capturedImg3"));

        verticalLayout->addWidget(capturedImg3);

        realtimeImg = new QLabel(centralWidget);
        realtimeImg->setObjectName(QStringLiteral("realtimeImg"));
        realtimeImg->setGeometry(QRect(10, 10, 67, 21));
        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        captureButton->setText(QApplication::translate("MainWindow", "Capure", Q_NULLPTR));
        exitButton->setText(QApplication::translate("MainWindow", "Exit", Q_NULLPTR));
        capturedImg1->setText(QString());
        capturedImg2->setText(QString());
        capturedImg3->setText(QString());
        realtimeImg->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
