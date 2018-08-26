#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "Camera/camera.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();



private slots:
    void on_exitButton_clicked();

    void on_captureButton_clicked();

    void streamImg();

private:
    Ui::MainWindow *ui;
    Camera * camTh;
};

#endif // MAINWINDOW_H
