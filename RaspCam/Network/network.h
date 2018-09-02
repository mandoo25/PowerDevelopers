#ifndef NETWORK_H
#define NETWORK_H

#include <QMutex>
#include <QThread>
#include <QObject>

#include "config.h"

class Network : public QThread
{
    Q_OBJECT

public:
    Network(unsigned int msecPollingPeriod);

    void sendRawImgData();
    void setRawImgData(byte * data);

    void exit();
    bool isEnable();

private:
    unsigned int pollingPeriod;
    bool _exit;
    bool _enabled;
    void run();
    byte * rawDataImg;



signals:
    void updateRawImg();


};





#endif // NETWORK_H
