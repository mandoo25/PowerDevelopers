#include "network.h"


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

        // sleep msec
        msleep(this->pollingPeriod);
    }

}

void Network::setRawImgData(byte *data)
{
    this->rawDataImg = data;
}


void Network::sendRawImgData()
{
    // plz fill out code

    // must free allocated data after sending
    delete this->rawDataImg;
}




