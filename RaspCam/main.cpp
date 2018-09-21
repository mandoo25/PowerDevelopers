#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include "Resource/resource.h"




int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;

    // w.setAttribute(Qt::WA_DeleteOnClose);
    // w.setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowCloseButtonHint | Qt::Popup);
    w.setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowCloseButtonHint);
    w.show();

    //w.showFullScreen();

    // qDebug() << "end???";

    return a.exec();
}
