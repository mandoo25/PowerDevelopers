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
    mutex.lock();
    this->capturedImg = img;
    mutex.unlock();
}

cv::Mat Camera::getCapturedImg()
{
    Mat img;
    mutex.lock();
    img = this->capturedImg;
    mutex.unlock();

    return img;
}


void Camera::run()
{
    cv::Mat img;

    while(!this->_exit)
    {
        // qDebug() << "Cam Th";

        cam.grab();
        cam.retrieve(img);

        cv::resize(img, img, Size(D_CAMERA_DISPLAYED_WIDTH, D_CAMERA_DISPLAYED_HEIGHT), 0, 0, CV_INTER_LINEAR);

        // image update
        this->updateImg(img);
        emit captureImg();   // stream img

        msleep(this->pollingPeriod);
    }
}

void Camera::exit()
{
    this->_exit = true;
}
