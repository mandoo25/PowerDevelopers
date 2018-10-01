#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include "Resource/resource.h"
#include <cstring>





int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;

    // w.setAttribute(Qt::WA_DeleteOnClose);
    // w.setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowCloseButtonHint | Qt::Popup);
    w.setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowCloseButtonHint);

    if(argc == 2 && strcmp(argv[1],"-d") == 0)
    {
        w.show();
    }
    else
    {
        w.showFullScreen();
    }

    // qDebug() << "end???";

    return a.exec();
}
