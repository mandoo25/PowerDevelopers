#include <QDebug>
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "Camera/camera.h"
#include "config.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // set titleless window
    this->setWindowFlags(Qt::Popup);

    // init camera
    this->camTh = new Camera(D_CAMERA_POLLING_MSEC, D_CAMERA_DISPLAYED_WIDTH, D_CAMERA_DISPLAYED_HEIGHT);
    connect(this->camTh, SIGNAL(captureImg()), this, SLOT(streamImg()));
    this->camTh->start();

    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    qDebug() << "delete ui";
    delete ui;
}

void MainWindow::streamImg()
{
    Mat img = this->camTh->getCapturedImg();
    ui->realtimeImg->resize(img.cols, img.rows);
    ui->realtimeImg->setPixmap(QPixmap::fromImage(QImage(img.data, img.cols, img.rows, img.step, QImage::Format_RGB888)));
}

void MainWindow::on_exitButton_clicked()
{
    this->camTh->exit();
    this->close();
}

void MainWindow::on_captureButton_clicked()
{
    Mat img = this->camTh->getCapturedImg();
    cv::resize(img, img, Size(D_CAMERA_DISPLAYED_WIDTH*3/5, D_CAMERA_DISPLAYED_HEIGHT*3/5), 0, 0, CV_INTER_LINEAR);
    ui->capturedImg1->resize(img.cols, img.rows);
    ui->capturedImg1->setPixmap(QPixmap::fromImage(QImage(img.data, img.cols, img.rows, img.step, QImage::Format_RGB888)));

    cv::resize(img, img, Size(D_CAMERA_DISPLAYED_WIDTH*2/5, D_CAMERA_DISPLAYED_HEIGHT*2/5), 0, 0, CV_INTER_LINEAR);
    ui->capturedImg2->resize(img.cols, img.rows);
    ui->capturedImg2->setPixmap(QPixmap::fromImage(QImage(img.data, img.cols, img.rows, img.step, QImage::Format_RGB888)));

    cv::resize(img, img, Size(D_CAMERA_DISPLAYED_WIDTH*1/5, D_CAMERA_DISPLAYED_HEIGHT*1/5), 0, 0, CV_INTER_LINEAR);
    ui->capturedImg3->resize(img.cols, img.rows);
    ui->capturedImg3->setPixmap(QPixmap::fromImage(QImage(img.data, img.cols, img.rows, img.step, QImage::Format_RGB888)));

}
