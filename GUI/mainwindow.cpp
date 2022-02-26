/*---------------------------------------------------------------------
 * mainwindow.cpp
 * Graphical User Interface
 * Hang Ji H.Ji@student.tudelft.nl
 * 16-06-2019
 *---------------------------------------------------------------------
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"

/*-------------------------------------------------------------------------------------
 * regard as main function:
 * call functions below the destructor ~MainWindow()
 *
 *-------------------------------------------------------------------------------------
 */

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


/*--------------------------------------------------------------------------------------
 * setting mainwindow format
 * -------------------------------------------------------------------------------------
 */

    this->setWindowTitle(QString("GUI"));
    //set the size of the GUI window
    QDesktopWidget dw;
    int x = dw.width()*0.9;
    int y = dw.height()*1;
    this->setFixedSize(x,y);
    QPalette mainwindow_palette(this->palette());
    mainwindow_palette.setColor(QPalette::Background,Qt::white);
    this->setPalette(mainwindow_palette);


/*--------------------------------------------------------------------------------------
 * setting several widgets
 * -------------------------------------------------------------------------------------
 */

    MainWindow::makePlotSensor1();
    MainWindow::makePlotSensor2();
    MainWindow::makePlotSensor3();
    MainWindow::makePlotFiltered();
    //real time data
    //joystick
    MainWindow::makePicMode();
    MainWindow::makePicBattery();
    MainWindow::makePicRoll();
    MainWindow::makePicPitch();
    MainWindow::makePicYaw();
    MainWindow::makePicLift();
    //motor data
    MainWindow::makePicAE0();
    MainWindow::makePicAE1();
    MainWindow::makePicAE2();
    MainWindow::makePicAE3();



    // refresh the console https://doc.qt.io/qt-5/qtimer.html

    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(updateAllData()));
    connect(timer,SIGNAL(timeout()),this,SLOT(updateJSData()));


    connect(ui->StopBTN,SIGNAL(clicked()),this,SLOT(stoptimer()));
    connect(ui->ConBTN,SIGNAL(clicked()),this,SLOT(continuetimer()));

    connect(ui->StopBTN,SIGNAL(clicked()),this,SLOT(StopBTNTest()));
    connect(ui->ConBTN,SIGNAL(clicked()),this,SLOT(ConBTNTest()));

    timer->start(REFRESH_FREQ);

    //update slot is called every (1000) ms

}
/*------------------------------------------------------------------------------------
 * destructor
 * -----------------------------------------------------------------------------------
 */

MainWindow::~MainWindow()
{
    delete ui;
}








/*------------------------------------------------------------------------------------
 * plotting sensor data sp, sq, sr; no updating values
 * object: SD1
 * -----------------------------------------------------------------------------------
 */
void MainWindow::makePlotSensor1()
{

    ui->SD1->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom); // set user interactions
    ui->SD1->addGraph(); //sp
    ui->SD1->addGraph(); //sq
    ui->SD1->addGraph(); //sr
    ui->SD1->xAxis->setLabel("TimeStamp");
    ui->SD1->yAxis->setLabel("Sensor Data: Angle Velocity");

    ui->SD1->graph(0)->setPen(QColor(Qt::darkBlue));
    ui->SD1->graph(0)->setName("sp");
    ui->SD1->graph(1)->setPen(QColor(Qt::darkCyan));
    ui->SD1->graph(1)->setName("sq");
    ui->SD1->graph(2)->setPen(QColor(Qt::darkYellow));
    ui->SD1->graph(2)->setName("sr");

    ui->SD1->legend->setVisible(true);
    ui->SD1->legend->setFont(QFont("Helvetica",9));
    ui->SD1->legend->setRowSpacing(-3);

}

/*------------------------------------------------------------------------------------
 * plotting sensor data phi, theta, psi; no updating values
 * object: SD2
 * -----------------------------------------------------------------------------------
 */

void MainWindow::makePlotSensor2()
{

    ui->SD2->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom); // set user interactions
    ui->SD2->addGraph();
    ui->SD2->addGraph();
    ui->SD2->addGraph();

    ui->SD2->xAxis->setLabel("TimeStamp");
    ui->SD2->yAxis->setLabel("Sensor Data: Angle");

    ui->SD2->graph(0)->setPen(QColor(Qt::darkBlue));
    ui->SD2->graph(0)->setName("phi");
    ui->SD2->graph(1)->setPen(QColor(Qt::darkCyan));
    ui->SD2->graph(1)->setName("theta");
    ui->SD2->graph(2)->setPen(QColor(Qt::darkYellow));
    ui->SD2->graph(2)->setName("psi");

    ui->SD2->legend->setVisible(true);
    ui->SD2->legend->setFont(QFont("Helvetica",9));
    ui->SD2->legend->setRowSpacing(-3);



}
/*------------------------------------------------------------------------------------
 * plotting sensor data ax, ay, az; no updating values
 * object: SD3
 * -----------------------------------------------------------------------------------
 */

void MainWindow::makePlotSensor3()
{

    ui->SD3->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom); // set user interactions
    ui->SD3->addGraph();
    ui->SD3->addGraph();
    ui->SD3->addGraph();

    ui->SD3->xAxis->setLabel("TimeStamp");
    ui->SD3->yAxis->setLabel("Sensor Data: Acceleration");

    ui->SD3->graph(0)->setPen(QColor(Qt::darkBlue));
    ui->SD3->graph(0)->setName("ax");
    ui->SD3->graph(1)->setPen(QColor(Qt::darkCyan));
    ui->SD3->graph(1)->setName("ay");
    ui->SD3->graph(2)->setPen(QColor(Qt::darkYellow));
    ui->SD3->graph(2)->setName("az");

    ui->SD3->legend->setVisible(true);
    ui->SD3->legend->setFont(QFont("Helvetica",9));
    ui->SD3->legend->setRowSpacing(-3);


}


void MainWindow::makePlotFiltered()
{

    ui->F_SD->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom); // set user interactions
    ui->F_SD->addGraph();
    ui->F_SD->addGraph();
    ui->F_SD->addGraph();

    ui->F_SD->xAxis->setLabel("TimeStamp");
    ui->F_SD->yAxis->setLabel("Sensor Data After Filtered");

    ui->F_SD->graph(0)->setPen(QColor(Qt::darkBlue));
    ui->F_SD->graph(0)->setName("sp_filtered");
    ui->F_SD->graph(1)->setPen(QColor(Qt::darkCyan));
    ui->F_SD->graph(1)->setName("sq_filtered");
    ui->F_SD->graph(2)->setPen(QColor(Qt::darkYellow));
    ui->F_SD->graph(2)->setName("sr_filtered");

    ui->F_SD->legend->setVisible(true);
    ui->F_SD->legend->setFont(QFont("Helvetica",9));
    ui->F_SD->legend->setRowSpacing(-3);


}


/*------------------------------------------------------------------------------------
 * display for mode value
 * object: Mode
 * -----------------------------------------------------------------------------------
 */
void MainWindow::makePicMode()
{

    ui->MODE->setDecMode();
    ui->MODE->setStyleSheet("boder: 1px silver; color:green; background:white");


}

/*------------------------------------------------------------------------------------
 * display for battery value
 * object: Mode
 * -----------------------------------------------------------------------------------
 */
void MainWindow::makePicBattery()
{


    ui->BATTTERY->setMaximum(battery_max);
    ui->BATTTERY->setMinimum(battery_min);
    ui->BATTTERY->setValue(battery_min);
    ui->BATTTERY->setStyleSheet("QProgressBar{boder: 1px silver; color:white; background:white; boder-radius:5px;}"\
                                "QProgressBar::chunk{background-color:green; border-radius:4.5px;}");

}


/*------------------------------------------------------------------------------------
 * display for roll value
 * object: ROLL
 * -----------------------------------------------------------------------------------
 */
void MainWindow::makePicRoll()
{

    ui->ROLL->addBackground(99);
    QcBackgroundItem *bkg1 = ui->ROLL->addBackground(95);
    bkg1->clearrColors();
    bkg1->addColor(0.1,Qt::black);
    bkg1->addColor(1.0,Qt::white);

    QcBackgroundItem *bkg2 = ui->ROLL->addBackground(94);
    bkg2->clearrColors();
    bkg2->addColor(0.1,Qt::gray);
    bkg2->addColor(1.0,Qt::darkGray);

    ui->ROLL->addArc(55);
    ui->ROLL->addDegrees(65)->setValueRange(-100,100);
    ui->ROLL->addColorBand(50);

    ui->ROLL->addValues(80)->setValueRange(-100,100);

    ui->ROLL->addLabel(70)->setText("ROLL");
    QcLabelItem *lab = ui->ROLL->addLabel(40);
    lab->setText("0");

    RollNeedle = ui->ROLL->addNeedle(60);
    RollNeedle->setLabel(lab);
    RollNeedle->setColor(Qt::white);
    RollNeedle->setValueRange(-100,100);
    ui->ROLL->addBackground(7);
    ui->ROLL->addGlass(88);

}


/*------------------------------------------------------------------------------------
 * display for pitch value
 * object: PITCH
 * -----------------------------------------------------------------------------------
 */
void MainWindow::makePicPitch()
{
    ui->PITCH->addBackground(99);
    QcBackgroundItem *bkg1 = ui->PITCH->addBackground(95);
    bkg1->clearrColors();
    bkg1->addColor(0.1,Qt::black);
    bkg1->addColor(1.0,Qt::white);

    QcBackgroundItem *bkg2 = ui->PITCH->addBackground(94);
    bkg2->clearrColors();
    bkg2->addColor(0.1,Qt::gray);
    bkg2->addColor(1.0,Qt::darkGray);

    ui->PITCH->addArc(55);
    ui->PITCH->addDegrees(65)->setValueRange(-100,100);
    ui->PITCH->addColorBand(50);

    ui->PITCH->addValues(80)->setValueRange(-100,100);

    ui->PITCH->addLabel(70)->setText("PITCH");
    QcLabelItem *lab = ui->PITCH->addLabel(40);
    lab->setText("0");

    PitchNeedle = ui->PITCH->addNeedle(60);
    PitchNeedle->setLabel(lab);
    PitchNeedle->setColor(Qt::white);
    PitchNeedle->setValueRange(-100,100);
    ui->PITCH->addBackground(7);
    ui->PITCH->addGlass(88);


}
/*------------------------------------------------------------------------------------
 * display for yaw value
 * object: YAW
 * -----------------------------------------------------------------------------------
 */
void MainWindow::makePicYaw()
{
    ui->YAW->addBackground(99);
    QcBackgroundItem *bkg1 = ui->YAW->addBackground(95);
    bkg1->clearrColors();
    bkg1->addColor(0.1,Qt::black);
    bkg1->addColor(1.0,Qt::white);

    QcBackgroundItem *bkg2 = ui->YAW->addBackground(94);
    bkg2->clearrColors();
    bkg2->addColor(0.1,Qt::gray);
    bkg2->addColor(1.0,Qt::darkGray);

    ui->YAW->addArc(55);
    ui->YAW->addDegrees(65)->setValueRange(-100,100);
    ui->YAW->addColorBand(50);

    ui->YAW->addValues(80)->setValueRange(-100,100);

    ui->YAW->addLabel(70)->setText("YAW");
    QcLabelItem *lab = ui->YAW->addLabel(40);
    lab->setText("0");

    YawNeedle = ui->YAW->addNeedle(60);
    YawNeedle->setLabel(lab);
    YawNeedle->setColor(Qt::white);
    YawNeedle->setValueRange(-100,100);
    ui->YAW->addBackground(7);
    ui->YAW->addGlass(88);


}
/*------------------------------------------------------------------------------------
 * display for lift value
 * object: LIFT
 * -----------------------------------------------------------------------------------
 */
void MainWindow::makePicLift()
{
    ui->LIFT->addBackground(99);
    QcBackgroundItem *bkg1 = ui->LIFT->addBackground(95);
    bkg1->clearrColors();
    bkg1->addColor(0.1,Qt::black);
    bkg1->addColor(1.0,Qt::white);

    QcBackgroundItem *bkg2 = ui->LIFT->addBackground(94);
    bkg2->clearrColors();
    bkg2->addColor(0.1,Qt::gray);
    bkg2->addColor(1.0,Qt::darkGray);

    ui->LIFT->addArc(55);
    ui->LIFT->addDegrees(65)->setValueRange(0,100);
    ui->LIFT->addColorBand(50);

    ui->LIFT->addValues(80)->setValueRange(0,100);

    ui->LIFT->addLabel(70)->setText("LIFT");
    QcLabelItem *lab = ui->LIFT->addLabel(40);
    lab->setText("0");

    LiftNeedle = ui->LIFT->addNeedle(60);
    LiftNeedle->setLabel(lab);
    LiftNeedle->setColor(Qt::white);
    LiftNeedle->setValueRange(0,100);
    ui->LIFT->addBackground(7);
    ui->LIFT->addGlass(88);

}



/*------------------------------------------------------------------------------------
 * display for ae0 value
 * object: AE0
 * -----------------------------------------------------------------------------------
 */
void MainWindow::makePicAE0()
{
    ui->AE0->addBackground(99);
    QcBackgroundItem *bkg1 = ui->AE0->addBackground(95);
    bkg1->clearrColors();
    bkg1->addColor(0.1,Qt::black);
    bkg1->addColor(1.0,Qt::white);

    QcBackgroundItem *bkg2 = ui->AE0->addBackground(94);
    bkg2->clearrColors();
    bkg2->addColor(0.1,Qt::gray);
    bkg2->addColor(1.0,Qt::darkGray);

    ui->AE0->addArc(55);
    ui->AE0->addDegrees(65)->setValueRange(0,100);
    ui->AE0->addColorBand(50);

    QcValuesItem *step = ui->AE0->addValues(80);
    step->setStep(100);
    step->setValueRange(0,1000);

    ui->AE0->addLabel(70)->setText("AE0");
    QcLabelItem *lab = ui->AE0->addLabel(40);
    lab->setText("0");

    Ae0Needle = ui->AE0->addNeedle(60);
    Ae0Needle->setLabel(lab);
    Ae0Needle->setColor(Qt::white);
    Ae0Needle->setValueRange(0,1000);
    ui->AE0->addBackground(7);
    ui->AE0->addGlass(88);

}

/*------------------------------------------------------------------------------------
 * display for AE1 value
 * object: AE1
 * -----------------------------------------------------------------------------------
 */
void MainWindow::makePicAE1()
{
    ui->AE1->addBackground(99);
    QcBackgroundItem *bkg1 = ui->AE1->addBackground(95);
    bkg1->clearrColors();
    bkg1->addColor(0.1,Qt::black);
    bkg1->addColor(1.0,Qt::white);

    QcBackgroundItem *bkg2 = ui->AE1->addBackground(94);
    bkg2->clearrColors();
    bkg2->addColor(0.1,Qt::gray);
    bkg2->addColor(1.0,Qt::darkGray);

    ui->AE1->addArc(55);
    ui->AE1->addDegrees(65)->setValueRange(0,100);
    ui->AE1->addColorBand(50);

    QcValuesItem *step = ui->AE1->addValues(80);
    step->setStep(100);
    step->setValueRange(0,1000);

    ui->AE1->addLabel(70)->setText("AE1");
    QcLabelItem *lab = ui->AE1->addLabel(40);
    lab->setText("0");

    Ae1Needle = ui->AE1->addNeedle(60);
    Ae1Needle->setLabel(lab);
    Ae1Needle->setColor(Qt::white);
    Ae1Needle->setValueRange(0,1000);
    ui->AE1->addBackground(7);
    ui->AE1->addGlass(88);
}

/*------------------------------------------------------------------------------------
 * display for AE2 value
 * object: AE2
 * -----------------------------------------------------------------------------------
 */
void MainWindow::makePicAE2()
{
    ui->AE2->addBackground(99);
    QcBackgroundItem *bkg1 = ui->AE2->addBackground(94);
    bkg1->clearrColors();
    bkg1->addColor(0.1,Qt::black);
    bkg1->addColor(1.0,Qt::white);

    QcBackgroundItem *bkg2 = ui->AE2->addBackground(90);
    bkg2->clearrColors();
    bkg2->addColor(0.1,Qt::gray);
    bkg2->addColor(1.0,Qt::darkGray);

    ui->AE2->addArc(55);
    ui->AE2->addDegrees(65)->setValueRange(-100,100);
    ui->AE2->addColorBand(50);

    QcValuesItem *step = ui->AE2->addValues(80);
    step->setStep(100);
    step->setValueRange(0,1000);

    ui->AE2->addLabel(70)->setText("AE2");
    QcLabelItem *lab = ui->AE2->addLabel(40);
    lab->setText("0");

    Ae2Needle = ui->AE2->addNeedle(60);
    Ae2Needle->setLabel(lab);
    Ae2Needle->setColor(Qt::white);
    Ae2Needle->setValueRange(0,1000);
    ui->AE2->addBackground(7);
    ui->AE2->addGlass(88);
}

/*------------------------------------------------------------------------------------
 * display for AE3 value
 * object: AE3
 * -----------------------------------------------------------------------------------
 */
void MainWindow::makePicAE3()
{
    ui->AE3->addBackground(99);
    QcBackgroundItem *bkg1 = ui->AE3->addBackground(95);
    bkg1->clearrColors();
    bkg1->addColor(0.1,Qt::black);
    bkg1->addColor(1.0,Qt::white);

    QcBackgroundItem *bkg2 = ui->AE3->addBackground(94);
    bkg2->clearrColors();
    bkg2->addColor(0.1,Qt::gray);
    bkg2->addColor(1.0,Qt::darkGray);

    ui->AE3->addArc(55);
    ui->AE3->addDegrees(65)->setValueRange(-100,100);
    ui->AE3->addColorBand(50);

    QcValuesItem *step = ui->AE3->addValues(80);
    step->setStep(100);
    step->setValueRange(0,1000);

    ui->AE3->addLabel(70)->setText("AE3");
    QcLabelItem *lab = ui->AE3->addLabel(40);
    lab->setText("0");

    Ae3Needle = ui->AE3->addNeedle(60);
    Ae3Needle->setLabel(lab);
    Ae3Needle->setColor(Qt::white);
    Ae3Needle->setValueRange(0,1000);
    ui->AE3->addBackground(7);
    ui->AE3->addGlass(88);
}











/*------------------------------------------------------------------------------------
 * update all data except for joystick
 * -----------------------------------------------------------------------------------
 */
void MainWindow::updateAllData()
{
    QFile file("/home/jelger/Desktop/ESL/control_station/controller/data.dat"); // directory of the battery txt file

    if (!file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        std::cout<<"NO FILE FOR ALL DATA EXCEPT FOR JOYSTICK"<<std::endl;
        return;
    }


    QTextStream in(&file);
    QString line;

    int which_line = 0; // indicate which line

    while(!in.atEnd())
    {
        if (which_line==0)
        {
            line = in.readLine();
        }

        which_line = which_line+1;

//        std::cout<<"PROCESSING data.dat LINE "<<which_line<<std::endl;

        line = in.readLine();
        //parsing lines to separate different sensor datas, suppose it should be parsed by space
        //need to be modified. split by tab, important !!!!!!!!!!
        QStringList parseLineList = line.split(" ", QString::SkipEmptyParts);

        int i = 0;

        //decode the .dat file
        for(; i<parseLineList.size(); )
        {
            TimeStamp.append(parseLineList.at(i).toDouble());
            i = i+1;
            mode_num.append(parseLineList.at(i).toDouble());
            //plot the mode type
            i = i+1;
            ae0.append(parseLineList.at(i).toDouble());
            i = i+1;
            ae1.append(parseLineList.at(i).toDouble());
            i = i+1;
            ae2.append(parseLineList.at(i).toDouble());
            i = i+1;
            ae3.append(parseLineList.at(i).toDouble());
            i = i+1;
            phi.append(parseLineList.at(i).toDouble());
            i = i+1;
            theta.append(parseLineList.at(i).toDouble());
            i = i+1;
            psi.append(parseLineList.at(i).toDouble());
            i = i+1;
            sp.append(parseLineList.at(i).toDouble());
            i = i+1;
            sq.append(parseLineList.at(i).toDouble());
            i = i+1;
            sr.append(parseLineList.at(i).toDouble());
            i = i+1;
            ax.append(parseLineList.at(i).toDouble());
            i = i+1;
            ay.append(parseLineList.at(i).toDouble());
            i = i+1;
            az.append(parseLineList.at(i).toDouble());
            i = i+1;
            flt_sp.append(parseLineList.at(i).toDouble());
            i = i+1;
            flt_sq.append(parseLineList.at(i).toDouble());
            i = i+1;
            flt_sr.append(parseLineList.at(i).toDouble());
            i= i+1;

        }
    }


    //find the maximum and the minimum of the timestamp
    qreal TimeStamp_min;
    qreal TimeStamp_max;
    TimeStamp_min = *std::min_element(TimeStamp.begin(),TimeStamp.end());
    TimeStamp_max = *std::max_element(TimeStamp.begin(),TimeStamp.end());

    //find the maximum and the minimum of the sp,sq,sr
    qreal SD1_range;
    qreal sp_range = setRange(sp);
    qreal sq_range = setRange(sq);
    qreal sr_range = setRange(sr);

    SD1_range = findRange(sp_range,sq_range,sr_range);


    //find the maximum and the minimum of the phi,theta,psi
    qreal SD2_range;
    qreal phi_range = setRange(phi);
    qreal theta_range = setRange(theta);
    qreal psi_range = setRange(psi);

    SD2_range = findRange(phi_range,theta_range,psi_range);

    //find the maximum and the minimum of the ax, ay, az
    qreal SD3_range;
    qreal ax_range = setRange(ax);
    qreal ay_range = setRange(ay);
    qreal az_range = setRange(az);

    SD3_range = findRange(ax_range,ay_range,az_range);

    //find the maximum and the minimum of the flt_sp flt_sq flt_sq
    qreal FSD_range;
    qreal flt_sp_range = setRange(flt_sp);
    qreal flt_sq_range = setRange(flt_sq);
    qreal flt_sr_range = setRange(flt_sr);

    FSD_range = findRange(flt_sp_range,flt_sq_range,flt_sr_range);


//    //find the maximum and the minimum of the ae0, ae1, ae2, ae3
//    qreal MotorData_range;

//    qreal ae0_range = setRange(ae0);
//    qreal ae1_range = setRange(ae1);
//    qreal ae2_range = setRange(ae2);
//    qreal ae3_range = setRange(ae3);

//    MotorData_range = findRange_ae(ae0_range,ae1_range,ae2_range,ae3_range);


    //plot sd1: sp, sq, sr
    ui->SD1->graph(0)->setData(TimeStamp,sp);
    ui->SD1->graph(1)->setData(TimeStamp,sq);
    ui->SD1->graph(2)->setData(TimeStamp,sr);
    //rescale sd1
    ui->SD1->xAxis->setRange(TimeStamp_min,TimeStamp_max);
    ui->SD1->yAxis->setRange(-SD1_range,SD1_range);
    ui->SD1->replot();

    //plot sd2: phi, theta, psi
    ui->SD2->graph(0)->setData(TimeStamp,phi);
    ui->SD2->graph(1)->setData(TimeStamp,theta);
    ui->SD2->graph(2)->setData(TimeStamp,psi);
    //rescale SD2
    ui->SD2->xAxis->setRange(TimeStamp_min,TimeStamp_max);
    ui->SD2->yAxis->setRange(-SD2_range,SD2_range);
    ui->SD2->replot();

    //plot sd3: phi, theta, psi
    ui->SD3->graph(0)->setData(TimeStamp,ax);
    ui->SD3->graph(1)->setData(TimeStamp,ay);
    ui->SD3->graph(2)->setData(TimeStamp,az);
    //rescale SD3
    ui->SD3->xAxis->setRange(TimeStamp_min,TimeStamp_max);
    ui->SD3->yAxis->setRange(-SD3_range,SD3_range);
    ui->SD3->replot();

    //plot fsd: flt_sp, flt_sq, flt_sr
    ui->F_SD->graph(0)->setData(TimeStamp,flt_sp);
    ui->F_SD->graph(1)->setData(TimeStamp,flt_sq);
    ui->F_SD->graph(2)->setData(TimeStamp,flt_sr);
    //rescale fsd
    ui->F_SD->xAxis->setRange(TimeStamp_min,TimeStamp_max);
    ui->F_SD->yAxis->setRange(-FSD_range,FSD_range);
    ui->F_SD->replot();



//    //plot MotorDara: ae0, ae1, ae2, ae3
//    ui->MotorData->graph(0)->setData(TimeStamp,ae0);
//    ui->MotorData->graph(1)->setData(TimeStamp,ae1);
//    ui->MotorData->graph(2)->setData(TimeStamp,ae2);
//    ui->MotorData->graph(2)->setData(TimeStamp,ae3);
//    //rescale MotorData
//    ui->MotorData->xAxis->setRange(TimeStamp_min,TimeStamp_max);
//    ui->MotorData->yAxis->setRange(-MotorData_range,MotorData_range);
//    ui->MotorData->replot();


}


/*------------------------------------------------------------------------------------
 * update joystick data
 * -----------------------------------------------------------------------------------
 */
void MainWindow::updateJSData()
{
    QFile file("/home/jelger/Desktop/ESL/control_station/controller/guifile.out"); // directory of the joystick txt file

    if (!file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        std::cout<<"NO FILE FOR DATA FOR JOYSTICK"<<std::endl;
        return;
    }

    QTextStream in(&file);
    QString line;

//    int which_line = 0; // indicate which line

//    while(!in.atEnd())
//    {
//        if (which_line==0)
//        {
//            line = in.readLine();
//        }

//        which_line = which_line+1;

//        std::cout<<"PROCESSING JOYSTICK LINE "<<which_line<<std::endl;

        line = in.readLine();

        //parsing lines to separate different sensor datas, suppose it should be parsed by space
        //need to be modified. split by tab, important !!!!!!!!!!
        QStringList parseLineList = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);

        int i = 0;

        //decode the .out file
        //add some function to include abnormal situation

        std::cout<<parseLineList.size()<<std::endl;

        for(; i<parseLineList.size() ; )
        {
            if ( parseLineList.size() == 10 )
            {
                mode_num.append(parseLineList.at(0).toDouble());
                std::cout<<"mode_num step:"<<parseLineList.at(0).toDouble()<<std::endl;

                ui->MODE->display(parseLineList.at(0).toDouble());
                i=i+1;

                roll.append(parseLineList.at(1).toDouble());
                std::cout<<"ROLL step: "<<parseLineList.at(1).toDouble()<<std::endl;
                roll_value = parseLineList.at(1).toInt()>>8;
                roll_value = checkRange(roll_value);
//                std::cout<<roll_value<<std::endl;
//                // draw roll needle
                RollNeedle->setCurrentValue(roll_value);
                i=i+1;

                pitch.append(parseLineList.at(2).toDouble());
                std::cout<<"PITCH step: "<<parseLineList.at(2).toDouble()<<std::endl;
                pitch_value = parseLineList.at(2).toInt()>>8;
                pitch_value = checkRange(pitch_value);
//                std::cout<<pitch_value<<std::endl;
//                // draw pitch needle
                PitchNeedle->setCurrentValue(pitch_value);
                i=i+1;

                yaw.append(parseLineList.at(3).toDouble());
                std::cout<<"YAW step: "<<parseLineList.at(3).toDouble()<<std::endl;
                yaw_value = parseLineList.at(3).toInt()>>8;
                yaw_value = checkRange(yaw_value);
//                std::cout<<yaw_value<<std::endl;
//                // draw Yaw needle
                YawNeedle->setCurrentValue(yaw_value);
                i=i+1;

                lift.append(parseLineList.at(4).toDouble());
                std::cout<<"LIFT step: "<<parseLineList.at(4).toDouble()<<std::endl;                
                lift_value = (((parseLineList.at(4).toInt())>>1) + 16384)>>8;
                lift_value = checkRange(lift_value);
//                std::cout<<lift_value<<std::endl;
//                // draw Yaw needle
                LiftNeedle->setCurrentValue(lift_value);

                ae0.append(parseLineList.at(5).toDouble());
                std::cout<<"AE0 step: "<<parseLineList.at(5).toDouble()<<std::endl;
                ae0_value = parseLineList.at(5).toInt();
                Ae0Needle->setCurrentValue(ae0_value);

                ae1.append(parseLineList.at(6).toDouble());
                std::cout<<"AE1 step: "<<parseLineList.at(6).toDouble()<<std::endl;
                ae1_value = parseLineList.at(6).toInt();
                Ae1Needle->setCurrentValue(ae1_value);

                ae2.append(parseLineList.at(7).toDouble());
                std::cout<<"AE2 step: "<<parseLineList.at(7).toDouble()<<std::endl;
                ae2_value = parseLineList.at(7).toInt();
                Ae2Needle->setCurrentValue(ae2_value);

                ae3.append(parseLineList.at(8).toDouble());
                std::cout<<"AE3 step: "<<parseLineList.at(8).toDouble()<<std::endl;
                ae3_value = parseLineList.at(8).toInt();
                Ae3Needle->setCurrentValue(ae3_value);

                battery_num.append(parseLineList.at(9).toDouble());
                ui->BATTTERY->setValue(parseLineList.at(9).toDouble());
                i=i+1;
            }
            else
            {
                //skip the line
                i = 6;
                std::cout<<"the length of the line is smaller than 6, skip"<<std::endl;
            }
        }

//        //print value
//        std::cout<<"ROLL BEFORE CHANGE "<<*roll.end()<<std::endl;

//        roll_value = abs(int(*roll.end())>>8);
//        roll_value = checkRange(roll_value);
//        std::cout<<"roll_value: "<<roll_value<<std::endl;
//        // draw roll needle
//        RollNeedle->setValueRange(0,100);
//        RollNeedle->setColor(Qt::white);
//        RollNeedle->setCurrentValue(roll_value);

//        std::cout<<"PTICH BEFORE CHANGE "<<*pitch.end()<<std::endl;
//        pitch_value = abs(int(*pitch.end())>>8);
//        pitch_value = checkRange(pitch_value);
//        std::cout<<"pitch_value: "<<pitch_value<<std::endl;
//        // draw pitch needle
//        PitchNeedle->setValueRange(0,100);
//        PitchNeedle->setColor(Qt::white);
//        PitchNeedle->setCurrentValue(pitch_value);

//        std::cout<<"YAW BEFORE CHANGE "<<*yaw.end()<<std::endl;
//        yaw_value = abs(int(*yaw.end())>>8);
//        yaw_value = checkRange(yaw_value);
//        std::cout<<"yaw_value: "<<yaw_value<<std::endl;
//        // draw yaw needle
//        YawNeedle->setValueRange(0,100);
//        YawNeedle->setColor(Qt::white);
//        YawNeedle->setCurrentValue(yaw_value);

//        std::cout<<"LIFT BEFORE CHANGE "<<*lift.end()<<std::endl;
//        lift_value = abs(int(*lift.end())>>8);
//        lift_value = checkRange(lift_value);
//        std::cout<<"lift_value: "<<lift_value<<std::endl;
//        // draw lift needle
//        LiftNeedle->setValueRange(0,100);
//        LiftNeedle->setColor(Qt::white);
//        LiftNeedle->setCurrentValue(lift_value);

    //}




}


/*------------------------------------------------------------------------------------
 * push start button
 *
 *------------------------------------------------------------------------------------
 */
void MainWindow::continuetimer()
{
    timer->start(REFRESH_FREQ);
}

void MainWindow::stoptimer()
{
    timer->stop();
}

/*------------------------------------------------------------------------------------
 * test whether button is pressed or not
 *------------------------------------------------------------------------------------
 */

void MainWindow::StopBTNTest()
{
    std::cout<<"stop button pressed"<<std::endl;
}

void MainWindow::ConBTNTest()
{
    std::cout<<"continue button pressed"<<std::endl;
}

qreal MainWindow::setRange(const QVector<qreal> &data_vetor)
{
    qreal data_vetor_min;
    qreal data_vetor_max;
    qreal range;

    data_vetor_min = *std::min_element(data_vetor.begin(),data_vetor.end());
    data_vetor_max = *std::max_element(data_vetor.begin(),data_vetor.end());

    if ( abs(data_vetor_min) > abs(data_vetor_max))
    {
        range = 1.5*abs(data_vetor_min);
        return range;
    }
    else
    {
        range = 1.5*abs(data_vetor_max);
        return range;
    }

}

/*------------------------------------------------------------------------------------
 * find maximum number for three numbers
 *------------------------------------------------------------------------------------
 */

qreal MainWindow::findRange(qreal data_1, qreal data_2, qreal data_3)
{
    qreal range;

    if ( data_1 > data_2)
    {
        range = data_1;
    }
    else
    {
        range = data_2;
    }

    if (range < data_3)
    {
        range = data_3;
    }

    return range;
}

/*------------------------------------------------------------------------------------
 * find maximum number for four numbers
 *------------------------------------------------------------------------------------
 */

qreal MainWindow::findRange_ae(qreal data_1, qreal data_2, qreal data_3, qreal data_4)
{

    qreal temp1;
    qreal temp2;
    qreal ae_range;

    if (data_1>data_2)
    {
        temp1 = data_1;
    }
    else
    {
        temp1 = data_2;
    }

    if (data_3>data_4)
    {
        temp2 = data_3;
    }
    else
    {
        temp2 = data_4;
    }

    if (temp1>temp2)
    {
        ae_range = temp1;
    }
    else
    {
        ae_range = temp2;
    }

    return ae_range;
}

/*------------------------------------------------------------------------------------
 * check the value of roll, yaw, pitch, lift
 *------------------------------------------------------------------------------------
 */
int MainWindow::checkRange(qreal value)
{
   if ( value > 100)
   {
       return 100;
   }
   else if ( value < -100)
   {
       return -100;
   }
   else
   {
       return value;
   }
}

