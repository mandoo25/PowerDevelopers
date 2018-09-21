#ifndef RESOURCE_CPP
#define RESOURCE_CPP

#include <QMutex>
#include <QThread>
#include <QObject>


#include <raspicam/raspicam_cv.h>

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>

class Resource
{
    // Q_OBJECT

public:
    Resource();
    ~Resource();

    void clear();


    void pushData(char * data, int size, int index);

    cv::Mat getImgAndIdx(int idx, int * index);
    void setImg(int idx, cv::Mat img);

    int getImgIdx(int idx);
    void updateImg(int idx,cv::Mat img);

    int getSize();
    int getIndexOf(int idx);


    cv::Mat getClearImg();


private:


    QList<int> indexs;
    QList<cv::Mat> imgs;


    QMutex mutex;


    cv::Mat blank;
};



#endif // RESOURCE_CPP
