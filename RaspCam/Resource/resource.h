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
    cv::Mat getData(int idx, int * index);
    int getimgIdx(int idx);
    void updateImg(int idx,cv::Mat img);

    int getSize();


private:


    QList<int> indexs;
    QList<cv::Mat> imgs;


    QMutex mutex;

};



#endif // RESOURCE_CPP
