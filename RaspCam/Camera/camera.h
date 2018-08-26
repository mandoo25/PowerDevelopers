#ifndef CAMERA_H
#define CAMERA_H

#include <QMutex>
#include <QThread>
#include <QObject>

#include <raspicam/raspicam_cv.h>

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;


class Camera : public QThread
{
    Q_OBJECT

public:
    Camera(unsigned int msecPollingPeriod, unsigned int width, unsigned int height);
    void setPollingPeriod(unsigned int msec);
    void exit();
    bool isEnable();
    void setCameraSize(unsigned int width, unsigned int height);
    bool initCamera(unsigned int width, unsigned int height);
    cv::Mat getCapturedImg();

private:
    unsigned int pollingPeriod;
    bool _exit;
    bool _enabled;
    void run();
    raspicam::RaspiCam_Cv cam;
    cv::Mat capturedImg;
    QMutex mutex;

    void updateImg(cv::Mat img);


signals:
    void captureImg();


};

#endif // CAMERA_H
