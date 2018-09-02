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
    this->camTh = new Camera(D_CAMERA_POLLING_MSEC, D_CAMEARA_CAPTURE_WIDTH, D_CAMEARA_CAPTURE_HEIGHT);
    connect(this->camTh, SIGNAL(captureImg()), this, SLOT(streamImg()));
    this->camTh->start();



    // init nework
    this->netTh = new Network(D_NETWORK_SLEEP_MSEC);
    connect(this->netTh, SIGNAL(updateRawImg()), this, SLOT(getRawImg()));
    connect(this->netTh, SIGNAL(updateRawImgFin()), this, SLOT(sendRawImgData()));


    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    qDebug() << "delete ui";
    delete ui;
}

void MainWindow::getRawImg()
{
    byte * rawImg = this->camTh->getCapturedRawImg();
    this->netTh->setRawImgData(rawImg);
    emit updateRawImgFin();
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
#define MAX_CAPURES  5
    static int cnt = 1;
    static int size[][2] =
    {
        {D_CAMERA_DISPLAYED_WIDTH*4/7, D_CAMERA_DISPLAYED_HEIGHT*4/7},
        {D_CAMERA_DISPLAYED_WIDTH*3/11, D_CAMERA_DISPLAYED_HEIGHT*3/11},
        {D_CAMERA_DISPLAYED_WIDTH*3/11, D_CAMERA_DISPLAYED_HEIGHT*3/11},
        {D_CAMERA_DISPLAYED_WIDTH*3/11, D_CAMERA_DISPLAYED_HEIGHT*3/11},
        {D_CAMERA_DISPLAYED_WIDTH*3/11, D_CAMERA_DISPLAYED_HEIGHT*3/11},
    };

    static Mat img[MAX_CAPURES];
    static QLabel * lb[MAX_CAPURES] =
    {
        ui->capturedImg1, ui->capturedImg2, ui->capturedImg3, ui->capturedImg4, ui->capturedImg5,
    };

    img[0] = this->camTh->getCapturedImg();

    for(int i = 0 ; i < cnt ; i++)
    {
        cv::resize(img[i], img[i], Size(size[i][0], size[i][1]), 0, 0, CV_INTER_LINEAR);
        lb[i]->resize(img[i].cols, img[i].rows);
        lb[i]->setPixmap(QPixmap::fromImage(QImage(img[i].data, img[i].cols, img[i].rows, img[i].step, QImage::Format_RGB888)));
    }

    for(int i = MAX_CAPURES - 1; i > 0 ;i--)
    {
        img[i] = img[i - 1].clone();
    }

    if(++cnt > (MAX_CAPURES-1)) cnt = MAX_CAPURES;

}
