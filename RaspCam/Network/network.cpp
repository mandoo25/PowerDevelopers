#include "network.h"
#include <QDebug>

#include "parserInfo.h"
#include "tcp_sock.h"


extern int transfer_data_main(void);
extern void setNetworkHandler(Network* net);

Network::Network(unsigned int msecPollingPeriod, Resource * res)
{
    this->pollingPeriod = msecPollingPeriod;
    this->_exit = false;

    this->res = res;
	
    // plz write somthing want to initialize
    setNetworkHandler(this);

}


void Network::exit()
{
    this->_exit = true;
}

void Network::run()
{


    qDebug() << "enter Network func" << endl;

    while(!this->_exit)
    {

        // interrupt : updateRawImg
        // emit updateRawImg();        
        transfer_data_main();

        //emit imgProcessFin();
        // sleep msec
        msleep(this->pollingPeriod);
    }

}

void Network::setRawImgData(uchar * data, int size, int index)
{
    this->rawDataImg = data;
    this->rawDataImgSize = size;
    this->rawDataIndex = index;
}

uchar * Network::getRawImgData(void)
{
    return this->rawDataImg;
}

void Network::sendRawImgData()
{
    // plz fill out code
    //packinfo

   //data*
   qDebug() << this->rawDataImgSize << endl;

   packInfo_tx pack;
   memset(&pack, 0, sizeof(packInfo_tx));

   pack.cmd_type = CMD_TYPE_REQUEST;
   pack.action_type = ACT_BARCODE1D;
   pack.item_id = WORK_ORDER;
   pack.cell_num = 1;
   pack.process_num = 1;
   pack.accuracy = 100;
   pack.order_size = 0;
   //pack.image_size = this->rawDataImg->size();
   //pack.image_data = (char *)this->rawDataImg;
   //pack.image_data = reinterpret_cast<char*>(this->rawDataImg->data());
   pack.image_size = this->rawDataImgSize;
   pack.image_data = (char*)this->rawDataImg;
   //qDebug() << "image data handler: "<< pack.image_data <<endl;
   //printf("?????: %d\n", this->rawDataImg);
   printf("image size : %d\n", pack.image_size);
   
   printf("%p\n", &pack);
   buildPacket(&pack);


}

void Network::setIpResults(int x, int y, bool res)
{
    printf("network::setIpResults\n");
    this->ipResult.x = x;
    this->ipResult.y = y;
    this->ipResult.result = res;
}



