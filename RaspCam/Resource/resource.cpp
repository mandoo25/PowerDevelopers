#include "resource.h"



Resource::Resource()
{

}

Resource::~Resource()
{
    this->clear();
}

void Resource::clear()
{
    this->mutex.lock();


    //qDeleteAll(this->imgs.begin(), this->imgs.end());
    this->imgs.clear();

    //qDeleteAll(this->indexs.begin(), this->indexs.end());
    this->indexs.clear();

    this->mutex.unlock();
}

void Resource::pushData(char * data, int size, int index)
{
    //std::vector<uchar> jpgbytes; // from your db
    //std::vector<char> data(buffer, buffer + size);
    //std::vector<uchar> jpgbytes(data, data + size); // from your db

    //cv::Mat img = cv::imdecode(jpgbytes);
    cv::Mat img = cv::imdecode(cv::Mat(1, size, CV_8UC1, data), CV_LOAD_IMAGE_UNCHANGED);


    this->mutex.lock();

    this->imgs.append(img);
    this->indexs.append(index);

    this->mutex.unlock();
}

int Resource::getSize()
{
    int size;
    this->mutex.lock();

    size = this->imgs.size();
    this->mutex.unlock();

    return size;

}
void Resource::updateImg(int idx,cv::Mat img)
{
    this->mutex.lock();

    this->imgs.removeAt(idx);
    this->imgs.insert(idx,img);

    this->mutex.unlock();
}

cv::Mat Resource::getData(int idx, int * index)
{
    cv::Mat img;

    this->mutex.lock();

    img = this->imgs.at(idx);
    *index = this->indexs.at(idx);

    this->mutex.unlock();

    return img;

}

int Resource::getimgIdx(int idx)
{
    int index;

    this->mutex.lock();


    index = this->indexs.at(idx);

    this->mutex.unlock();

    return index;
}
