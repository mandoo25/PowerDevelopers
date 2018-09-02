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
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QTabWidget *tabWidget;
    QWidget *CameraTab;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QLabel *capturedImg1;
    QFormLayout *formLayout;
    QLabel *capturedImg2;
    QLabel *capturedImg4;
    QLabel *capturedImg3;
    QLabel *capturedImg5;
    QPushButton *captureButton;
    QPushButton *exitButton;
    QLabel *realtimeImg;
    QLabel *label;
    QLabel *label_2;
    QWidget *ToolsTab;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(487, 320);
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
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setGeometry(QRect(-2, -1, 491, 321));
        QFont font;
        font.setPointSize(10);
        font.setBold(true);
        font.setItalic(true);
        font.setWeight(75);
        tabWidget->setFont(font);
        CameraTab = new QWidget();
        CameraTab->setObjectName(QStringLiteral("CameraTab"));
        QPalette palette1;
        QBrush brush2(QColor(0, 0, 0, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::WindowText, brush2);
        QBrush brush3(QColor(175, 175, 175, 255));
        brush3.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Button, brush3);
        palette1.setBrush(QPalette::Active, QPalette::Light, brush);
        QBrush brush4(QColor(215, 215, 215, 255));
        brush4.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Midlight, brush4);
        QBrush brush5(QColor(87, 87, 87, 255));
        brush5.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Dark, brush5);
        QBrush brush6(QColor(117, 117, 117, 255));
        brush6.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Mid, brush6);
        palette1.setBrush(QPalette::Active, QPalette::Text, brush2);
        palette1.setBrush(QPalette::Active, QPalette::BrightText, brush);
        palette1.setBrush(QPalette::Active, QPalette::ButtonText, brush2);
        palette1.setBrush(QPalette::Active, QPalette::Base, brush);
        palette1.setBrush(QPalette::Active, QPalette::Window, brush3);
        palette1.setBrush(QPalette::Active, QPalette::Shadow, brush2);
        palette1.setBrush(QPalette::Active, QPalette::AlternateBase, brush4);
        QBrush brush7(QColor(255, 255, 220, 255));
        brush7.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::ToolTipBase, brush7);
        palette1.setBrush(QPalette::Active, QPalette::ToolTipText, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::WindowText, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::Button, brush3);
        palette1.setBrush(QPalette::Inactive, QPalette::Light, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::Midlight, brush4);
        palette1.setBrush(QPalette::Inactive, QPalette::Dark, brush5);
        palette1.setBrush(QPalette::Inactive, QPalette::Mid, brush6);
        palette1.setBrush(QPalette::Inactive, QPalette::Text, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::BrightText, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::ButtonText, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::Window, brush3);
        palette1.setBrush(QPalette::Inactive, QPalette::Shadow, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush4);
        palette1.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush7);
        palette1.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::WindowText, brush5);
        palette1.setBrush(QPalette::Disabled, QPalette::Button, brush3);
        palette1.setBrush(QPalette::Disabled, QPalette::Light, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::Midlight, brush4);
        palette1.setBrush(QPalette::Disabled, QPalette::Dark, brush5);
        palette1.setBrush(QPalette::Disabled, QPalette::Mid, brush6);
        palette1.setBrush(QPalette::Disabled, QPalette::Text, brush5);
        palette1.setBrush(QPalette::Disabled, QPalette::BrightText, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::ButtonText, brush5);
        palette1.setBrush(QPalette::Disabled, QPalette::Base, brush3);
        palette1.setBrush(QPalette::Disabled, QPalette::Window, brush3);
        palette1.setBrush(QPalette::Disabled, QPalette::Shadow, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush3);
        palette1.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush7);
        palette1.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush2);
        CameraTab->setPalette(palette1);
        CameraTab->setFont(font);
        verticalLayoutWidget = new QWidget(CameraTab);
        verticalLayoutWidget->setObjectName(QStringLiteral("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(310, 30, 161, 251));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        capturedImg1 = new QLabel(verticalLayoutWidget);
        capturedImg1->setObjectName(QStringLiteral("capturedImg1"));

        verticalLayout->addWidget(capturedImg1);

        formLayout = new QFormLayout();
        formLayout->setSpacing(6);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        formLayout->setHorizontalSpacing(2);
        formLayout->setVerticalSpacing(2);
        capturedImg2 = new QLabel(verticalLayoutWidget);
        capturedImg2->setObjectName(QStringLiteral("capturedImg2"));
        capturedImg2->setText(QStringLiteral(""));

        formLayout->setWidget(0, QFormLayout::LabelRole, capturedImg2);

        capturedImg4 = new QLabel(verticalLayoutWidget);
        capturedImg4->setObjectName(QStringLiteral("capturedImg4"));

        formLayout->setWidget(0, QFormLayout::FieldRole, capturedImg4);

        capturedImg3 = new QLabel(verticalLayoutWidget);
        capturedImg3->setObjectName(QStringLiteral("capturedImg3"));

        formLayout->setWidget(1, QFormLayout::LabelRole, capturedImg3);

        capturedImg5 = new QLabel(verticalLayoutWidget);
        capturedImg5->setObjectName(QStringLiteral("capturedImg5"));

        formLayout->setWidget(1, QFormLayout::FieldRole, capturedImg5);


        verticalLayout->addLayout(formLayout);

        captureButton = new QPushButton(CameraTab);
        captureButton->setObjectName(QStringLiteral("captureButton"));
        captureButton->setGeometry(QRect(10, 240, 101, 31));
        exitButton = new QPushButton(CameraTab);
        exitButton->setObjectName(QStringLiteral("exitButton"));
        exitButton->setGeometry(QRect(140, 240, 101, 31));
        realtimeImg = new QLabel(CameraTab);
        realtimeImg->setObjectName(QStringLiteral("realtimeImg"));
        realtimeImg->setGeometry(QRect(10, 20, 251, 191));
        label = new QLabel(CameraTab);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(80, 210, 111, 21));
        label_2 = new QLabel(CameraTab);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(350, 10, 111, 21));
        tabWidget->addTab(CameraTab, QString());
        ToolsTab = new QWidget();
        ToolsTab->setObjectName(QStringLiteral("ToolsTab"));
        tabWidget->addTab(ToolsTab, QString());
        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        capturedImg1->setText(QString());
        capturedImg4->setText(QString());
        capturedImg3->setText(QString());
        capturedImg5->setText(QString());
        captureButton->setText(QApplication::translate("MainWindow", "Capture", Q_NULLPTR));
        exitButton->setText(QApplication::translate("MainWindow", "Exit", Q_NULLPTR));
        realtimeImg->setText(QString());
        label->setText(QApplication::translate("MainWindow", "Streaming Image", Q_NULLPTR));
        label_2->setText(QApplication::translate("MainWindow", "Captured Images", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(CameraTab), QApplication::translate("MainWindow", "Camera", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(ToolsTab), QApplication::translate("MainWindow", "Tools", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
