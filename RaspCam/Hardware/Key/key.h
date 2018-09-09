#ifndef KEY_H
#define KEY_H

#include <QMutex>
#include <QThread>
#include <QObject>

#include <wiringPi.h>


#define KeyPin  2

class Key : public QThread
{
    Q_OBJECT

public:
    Key();

    bool isEnabled();


private:
    unsigned int pollingPeriod;
    bool _exit;
    bool _enabled;
    void run();

    void close();

signals:
    void keyPressed();
};



#endif // KEY_H
