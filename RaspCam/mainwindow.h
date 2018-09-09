#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

    void on_captureButton_clicked();

    void streamImg();

    void getRawImg();

    void on_matchRateSlider_sliderMoved(int position);
	
	void updateIPResult();
    void updateResource();

    void on_externalButton_pressed();

    void on_ResetButton_clicked();

    void on_leftButton_clicked();

    void on_rightButton_clicked();

private:
    Ui::MainWindow *ui;
    Camera * camTh;
    Network * netTh;
    Buzzer * buzzerTh;
    Key * keyTh;

    void drawImg(int idx,int x, int y, bool result, bool shift);

    void updateLowerUI();

    QPoint m_down;
    QPoint m_up;

    Resource * res;


    int curIdx;
    int maxIdx;
    int viewIdx;

    Mat img[MAX_CAPURES];
    int index[MAX_CAPURES];

    bool resourceFin;





signals :
    void updateRawImgFin();
};

#endif // MAINWINDOW_H
