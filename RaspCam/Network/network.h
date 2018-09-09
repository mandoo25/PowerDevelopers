#ifndef NETWORK_H
#define NETWORK_H

#include <QMutex>
#include <QThread>
#include <QObject>

#include "config.h"
#include "Resource/resource.h"

class Network : public QThread
{
    Q_OBJECT

public:
    Network(unsigned int msecPollingPeriod, Resource * res);

    void setRawImgData(uchar * data, int size, int index);
	uchar * getRawImgData(void);


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
    int rawDataIndex;

    static Network net_instance;
    Resource * res;
	
	

private slots:
    void sendRawImgData();
	
signals :
    void imgProcessFin();       // image process fininshed
    void resourceUpdateFin();   // update resource finished

};





#endif // NETWORK_H
