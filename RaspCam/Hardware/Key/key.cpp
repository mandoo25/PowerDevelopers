#include "key.h"


Key::Key()
{
    this->_enabled = true;
    this->_exit = false;
    this->pollingPeriod = 1;

    pinMode(KeyPin, INPUT);

}




void Key::run()
{
    int count = 0;
    int port;
    bool longKey = false;

    while(!this->_exit)
    {
        port = digitalRead(KeyPin);

        if(port == 1)
        {
            count++;
            if(count > 150)
            {
                emit keyPressed();
                longKey = true;
            }

            if(longKey == true)
            {
                count = 0;
            }
        }
        else
        {
            count = 0;
            longKey = false;
        }

        msleep(this->pollingPeriod);
    }
}
