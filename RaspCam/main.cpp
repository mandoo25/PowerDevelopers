#include "mainwindow.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    w.setAttribute(Qt::WA_DeleteOnClose);

    w.show();

    // qDebug() << "end???";

    return a.exec();
}
