/*---------------------------------------------------------------------
 * mainwindow.h
 * Graphical User Interface
 * Hang Ji H.Ji@student.tudelft.nl
 * 16-06-2019
 *---------------------------------------------------------------------
 */





#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QFile>
#include <iostream>
#include <vector>
#include <QTimer>
#include <QDebug>
#include <algorithm>
#include <stdlib.h>
#include <QLCDNumber>
#include <QPen>
#include <QDesktopWidget>


#include "qcustomplot.h"
#include "qcgaugewidget.h"


#define REFRESH_FREQ 40
#define battery_max 1200
#define battery_min 0
#define motor_min 0
#define motor_max 1000





namespace Ui {
class MainWindow;
}


/*----------------------------------------------------------------
 * MainWindow Class:
 * Inside the MainWindow Class:
 * Timer: used to refresh data from the .txt file to the plot
 * Slots: used to defined how to plot the data on the plot
 *
 *
 *----------------------------------------------------------------
 */

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    qreal setRange(const QVector<qreal> &data_vector);
    qreal findRange(qreal data_1, qreal data_2, qreal data_3);
    qreal findRange_ae(qreal data_1, qreal data_2, qreal data_3, qreal data_4);
    int checkRange(qreal value);
private slots:

//    void SetMainLayout(); // set main layout

    void makePlotSensor1(); // plot for sp, sq, sr; object name: SD1
    void makePlotSensor2(); // plot for phi, theta, psi
    void makePlotSensor3(); // plot for ax, ay, az
    void makePlotFiltered(); //plot filtered sp, sq, sr

    void makePicMode(); // display for mode
    void makePicBattery(); //display for battery

    void makePicRoll(); //display roll
    void makePicPitch(); //display pitch
    void makePicYaw(); //display yaw
    void makePicLift(); //display lift


    void makePicAE0(); //display ae0
    void makePicAE1(); //display ae1
    void makePicAE2(); //display ae2
    void makePicAE3(); //display ae3

    void updateAllData(); //update sensor data
    void updateJSData();

    void stoptimer(); //stop timer
    void continuetimer(); //continue timer


    void StopBTNTest();
    void ConBTNTest();




private:
    Ui::MainWindow *ui;

    QTimer *timer; // timer used to refresh the data
//    qreal TimeInterval; // timer interval to refresh the output on the console

    QVector<qreal> TimeStamp;

    QVector<qreal> TimeStampJS;

    QVector<qreal>  sp; // vector to store sp
    QVector<qreal>  sq; // vector to store sq
    QVector<qreal>  sr; // vector to store sr

    QVector<qreal> phi;
    QVector<qreal> theta;
    QVector<qreal> psi;

    QVector<qreal> ax;
    QVector<qreal> ay;
    QVector<qreal> az;



    QVector<qreal> flt_sp; // filtered sp
    QVector<qreal> flt_sq; // filered sq
    QVector<qreal> flt_sr; // filered sq

    QVector<qreal> roll;
    QVector<qreal> pitch;
    QVector<qreal> yaw;
    QVector<qreal> lift;

    QVector<qreal> ae0;
    QVector<qreal> ae1;
    QVector<qreal> ae2;
    QVector<qreal> ae3;

    qreal roll_value;
    qreal pitch_value;
    qreal yaw_value;
    qreal lift_value;
    qreal ae0_value;
    qreal ae1_value;
    qreal ae2_value;
    qreal ae3_value;


    QVector<qreal> mode_num;
    QVector<qreal> battery_num;

    QcNeedleItem *RollNeedle;
    QcNeedleItem *PitchNeedle;
    QcNeedleItem *YawNeedle;
    QcNeedleItem *LiftNeedle;

    QcNeedleItem *Ae0Needle;
    QcNeedleItem *Ae1Needle;
    QcNeedleItem *Ae2Needle;
    QcNeedleItem *Ae3Needle;

};

#endif // MAINWINDOW_H
