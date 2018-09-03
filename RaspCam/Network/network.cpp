#include "network.h"
#include <QDebug>

#include "parserInfo.h"
#include "tcp_sock.h"


extern int transfer_data_main(void);

Network::Network(unsigned int msecPollingPeriod)
{
    this->pollingPeriod = msecPollingPeriod;
    this->_exit = false;

    // plz write somthing want to initialize
}


void Network::exit()
{
    this->_exit = true;
}

void Network::run()
{


    while(!this->_exit)
    {

        // interrupt : updateRawImg
        // emit updateRawImg();
        qDebug() << "enter my func" << endl;
        transfer_data_main();

        //emit imgProcessFin();
        // sleep msec
        msleep(this->pollingPeriod);
    }

}

void Network::setRawImgData(std::vector<byte> data)
{
    this->rawDataImg = data;
}


void Network::sendRawImgData()
{
    // plz fill out code
    //packinfo

   //data*
   qDebug() << this->rawDataImg.size() << endl;

   packInfo_tx pack;
   memset(&pack, 0, sizeof(packInfo_tx));

   pack.cmd_type = CMD_TYPE_REQUEST;
   pack.action_type = ACT_BARCODE1D;
   pack.item_id = WORK_ORDER;
   pack.cell_num = 1;
   pack.process_num = 1;
   pack.accuracy = 100;
   pack.image_size = this->rawDataImg.size();
   //pack.image_data = (char *)this->rawDataImg;
   pack.image_data = reinterpret_cast<char*>(this->rawDataImg.data());
   qDebug() << "data: "<< pack.image_data <<endl;

   buildPacket(&pack);


}



