#ifndef COMTELEDIALG_H
#define COMTELEDIALG_H

#include <QWidget>

#include <QMainWindow>
#include <QtCore>
#include <QMessageBox>
#include <QFile>
#include <QInputDialog>
#include <QFileDialog>
#include <QTextStream>

#include "qextserial/qextserialport.h"
#include "comteledialg.h"


//延时，TIME_OUT是串口读写的延时
#define TIME_OUT 10

namespace Ui {
class Comteledialg;
}

class Comteledialg : public QWidget
{
    Q_OBJECT

public:
    explicit Comteledialg(QWidget *parent = 0);
    ~Comteledialg();
    void startInit();
    void setComboBoxEnabled(bool status);
    void setTeleEnabled(bool status);

private slots:
    void on_actionOpenCom_clicked();


    void on_actionCloseCom_clicked();

    void sendMsg();

    void readMyCom();

    void on_sendmsgBtn_clicked();

    void on_clearUpBtn_clicked();

    void comdatabuf(unsigned char addr1, unsigned char addr2,
                             unsigned char mode,
                             unsigned char x1, unsigned char x2,
                             unsigned char y1, unsigned char y2,
                             unsigned char z1, unsigned char z2,
                             unsigned char dir);

    void on_startBtn_clicked();

    void on_upBtn_clicked();

    void on_backBtn_clicked();

    void on_stopBtn_clicked();

    void on_rightBtn_clicked();

    void on_leftBtn_clicked();

private:
    Ui::Comteledialg *ui;
    QextSerialPort *com_tele;
};

#endif // COMTELEDIALG_H
