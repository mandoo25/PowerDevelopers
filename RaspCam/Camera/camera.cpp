#include <QDebug>

#include "camera.h"
#include "../config.h"

Camera::Camera(unsigned int msecPollingPeriod, unsigned int width, unsigned int height)
{
    this->_exit = false;
    this->pollingPeriod = msecPollingPeriod;
    // this->capturedImg = NULL;

    initCamera(width, height);

    qDebug() << "init Camera Thread Compelete : " << this->isEnable();

}

void Camera::setCameraSize(unsigned int width, unsigned int height)
{
    this->cam.set ( CV_CAP_PROP_FORMAT, CV_8UC3 );
    this->cam.set ( CV_CAP_PROP_FRAME_WIDTH, width);
    this->cam.set ( CV_CAP_PROP_FRAME_HEIGHT, height);
}

bool Camera::initCamera(unsigned int width, unsigned int height)
{
    // do not need to init
    if(this->cam.isOpened()){
        return true;
    }

    this->setCameraSize(width, height);
    if( !this->cam.open())
    {
        return false;
    }
    return true;
}

void Camera::close()
{
    if(this->cam.isOpened())
    {
        this->cam.release();
    }
}

bool Camera::isEnable()
{
    return this->cam.isOpened();
}

void Camera::setPollingPeriod(unsigned int msec)
{
    this->pollingPeriod = msec;
}

void Camera::updateImg(Mat img)
{
    captuerImgMutex.lock();
    this->capturedImg = img;
    captuerImgMutex.unlock();
}

cv::Mat Camera::getCapturedImg()
{
    Mat img;
    // oImg.copyTo(cImg5);
    captuerImgMutex.lock();
    img = this->capturedImg.clone();
    captuerImgMutex.unlock();

    return img;
}



std::vector<byte> Camera::getCapturedRawImg()
{
    Mat image;
/*
    captuerImgMutex.lock();
    image = this->capturedImg;
    int size = image.rows*image.cols;
    byte bytes[size];
    std::memcpy(bytes,image.data,size * sizeof(byte));
    captuerImgMutex.unlock();
*/
    captuerImgMutex.lock();
    image = this->capturedImg;

    std::vector<uchar> buff;//buffer for coding
    std::vector<int> param(2);
    param[0] = cv::IMWRITE_JPEG_QUALITY;
    param[1] = 80;//default(95) 0-100
    cv::imencode(".jpg", image, buff, param);

    captuerImgMutex.unlock();

    return buff;
}


void Camera::run()
{
    cv::Mat img;

    while(!this->_exit)
    {
        // qDebug() << "Cam Th";

        cam.grab();
        cam.retrieve(img);

        // cv::resize(img, img, Size(D_CAMERA_DISPLAYED_WIDTH, D_CAMERA_DISPLAYED_HEIGHT), 0, 0, CV_INTER_LINEAR);

        // image update
        this->updateImg(img);
        emit captureImg();   // stream img

        msleep(this->pollingPeriod);
    }

    qDebug() << "release camera resource..";

    this->close();
}

void Camera::exit()
{
    this->_exit = true;
}
