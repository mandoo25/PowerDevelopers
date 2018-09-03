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

    void setRawImgData(std::vector<byte>);

    void exit();
    bool isEnable();
	
	//result data
	struct IpResult
	{
		int x;
		int y;
		bool result;
	}ipResult;

private:
    unsigned int pollingPeriod;
    bool _exit;
    bool _enabled;
    void run();
    std::vector<byte> rawDataImg;
	
	

private slots:
    void sendRawImgData();
	
signals :
    void imgProcessFin();

};





#endif // NETWORK_H
