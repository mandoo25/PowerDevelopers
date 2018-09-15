#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>

#include "Camera/camera.h"
#include "Network/network.h"
#include "Hardware/Buzzer/buzzer.h"
#include "Hardware/Key/key.h"
#include "Resource/resource.h"

namespace Ui {
class MainWindow;
}


#define MAX_CAPURES  5

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private slots:
    void on_exitButton_clicked();



    void streamImg();

    void getRawImg();

    void on_matchRateSlider_sliderMoved(int position);
	
	void updateIPResult();

    void updateResource();

    void on_externalButton_pressed();

    void on_ResetButton_clicked();

    void on_leftButton_clicked();

    void on_rightButton_clicked();

    void on_tabWidget_currentChanged(int index);

    void on_factorycb_currentTextChanged(const QString &arg1);

    void on_ipcb1_currentIndexChanged(const QString &arg1);

    void on_ipcb2_currentIndexChanged(const QString &arg1);

    void on_ipcb3_currentIndexChanged(const QString &arg1);

    void on_ipcb4_currentIndexChanged(const QString &arg1);

    void on_portcb_currentIndexChanged(const QString &arg1);

    void on_streamingImg_clicked();

private:
    Ui::MainWindow *ui;
    Camera * camTh = NULL;
    Network * netTh= NULL;
    Buzzer * buzzerTh= NULL;
    Key * keyTh = NULL;

    void drawImg(bool draw, int x, int y, bool result);

    void updateLowerUI(int startIdx);

    QPoint m_down;
    QPoint m_up;

    Resource * res;


    int curIdx;
    int maxIdx;
    int viewIdx;

    QPushButton * capturedImg[D_UI_NUMBER_OF_LOWER_UI_IMGS];
    Mat img[D_UI_NUMBER_OF_LOWER_UI_IMGS];
    int index[D_UI_NUMBER_OF_LOWER_UI_IMGS];

    bool resourceFin;

    void setIpAddress();
    QString IP;
    QByteArray tempQs; //temp data for QString -> char*

    void setProcess();
    QString process;

    void setPort();
    void setImgMatchRate();






signals :
    void updateRawImgFin();
};

#endif // MAINWINDOW_H
