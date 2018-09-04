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

    void setRawImgData(uchar * data);
	uchar * getRawImgData(void);
	
	void setRawImgDataSize(int size);

    void exit();
    bool isEnable();
	
	//result data
    struct IpResult
	{
		int x;
		int y;
		bool result;
	}ipResult;    

    static Network *getInstance() { return &net_instance; }

    void setIpResults(int x, int y, bool res);

private:
    unsigned int pollingPeriod;
    bool _exit;
    bool _enabled;
    void run();
    
	uchar * rawDataImg;
	int rawDataImgSize;
    static Network net_instance;
	
	

private slots:
    void sendRawImgData();
	
signals :
    void imgProcessFin();

};





#endif // NETWORK_H
