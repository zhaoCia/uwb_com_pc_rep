#include <QDebug>

#include "comteledialg.h"
#include "ui_comteledialg.h"

Comteledialg::Comteledialg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Comteledialg)
{
    ui->setupUi(this);

    startInit();
    ui->portNameComboBox->addItem("COM0");
    ui->portNameComboBox->addItem("COM1");
    ui->portNameComboBox->addItem("COM2");
    ui->portNameComboBox->addItem("COM3");
    ui->portNameComboBox->addItem("COM4");
    ui->portNameComboBox->addItem("COM5");
    ui->portNameComboBox->addItem("COM6");
    ui->portNameComboBox->addItem("COM7");
    ui->portNameComboBox->addItem("COM8");
    ui->portNameComboBox->addItem("COM9");
    ui->portNameComboBox->addItem("COM10");
    ui->portNameComboBox->addItem("COM11");
    ui->portNameComboBox->addItem("COM12");
    ui->portNameComboBox->addItem("COM13");
    ui->portNameComboBox->addItem("COM14");
    ui->portNameComboBox->addItem("COM15");


}

Comteledialg::~Comteledialg()
{
    delete ui;
}

void Comteledialg::startInit()
{
    ui->actionCloseCom->setEnabled(false);
    ui->sendmsgBtn->setEnabled(false);
    ui->sendMsgLineEdit->setEnabled(false);
    setTeleEnabled(false);
}

void Comteledialg::setComboBoxEnabled(bool status)
{
    ui->portNameComboBox->setEnabled(status);
    ui->baudRateComboBox->setEnabled(status);
    ui->dataBitsComboBox->setEnabled(status);
    ui->parityComboBox->setEnabled(status);
    ui->stopBitsComboBox->setEnabled(status);

}

void Comteledialg::setTeleEnabled(bool status)
{
    ui->backBtn->setEnabled(status);
    ui->back_leftBtn->setEnabled(status);
    ui->back_rightBtn->setEnabled(status);
    ui->leftBtn->setEnabled(status);
    ui->rightBtn->setEnabled(status);
    ui->startBtn->setEnabled(status);
    ui->stopBtn->setEnabled(status);
    ui->upBtn->setEnabled(status);
    ui->up_leftBtn->setEnabled(status);
    ui->up_rightBtn->setEnabled(status);
}

void Comteledialg::on_actionOpenCom_clicked()
{
    QString portName = ui->portNameComboBox->currentText();   //获取串口名
    com_tele = new QextSerialPort(portName);

    connect(com_tele, SIGNAL(readyRead()), this, SLOT(readMyCom()));

    //设置波特率
       com_tele->setBaudRate((BaudRateType)ui->baudRateComboBox->currentText().toInt());

       //设置数据位
       com_tele->setDataBits((DataBitsType)ui->dataBitsComboBox->currentText().toInt());

       //设置校验
       switch(ui->parityComboBox->currentIndex()){
       case 0:
           com_tele->setParity(PAR_NONE);
            break;
       case 1:
           com_tele->setParity(PAR_ODD);
           break;
       case 2:
           com_tele->setParity(PAR_EVEN);
           break;
       default:
           com_tele->setParity(PAR_NONE);
           qDebug("set to default : PAR_NONE");
           break;
       }

       //设置停止位
       switch(ui->stopBitsComboBox->currentIndex()){
       case 0:
           com_tele->setStopBits(STOP_1);
           break;
       case 1:
           com_tele->setStopBits(STOP_1_5);
           break;
       case 2:
           com_tele->setStopBits(STOP_2);
           break;
       default:
           com_tele->setStopBits(STOP_1);
           qDebug("set to default : STOP_1");
           break;
       }

       //设置数据流控制
       com_tele->setFlowControl(FLOW_OFF);
       //设置延时
       com_tele->setTimeout(TIME_OUT);

       if(com_tele->open(QIODevice::ReadWrite))
       {
           QMessageBox::information(this, tr("打开成功"), tr("已成功打开串口") + portName, QMessageBox::Ok);
           //界面控制
           ui->actionOpenCom->setEnabled(false);
           setComboBoxEnabled(false);
           setTeleEnabled(true);

           ui->actionCloseCom->setEnabled(true);
           ui->sendmsgBtn->setEnabled(true);
           ui->sendMsgLineEdit->setEnabled(true);
       }
       else
       {
           QMessageBox::critical(this, tr("打开失败"), tr("未能打开串口 ") + portName + tr("\n该串口设备不存在或已被占用"), QMessageBox::Ok);
           return;
       }

}

void Comteledialg::on_actionCloseCom_clicked()
{
    com_tele->close();
    delete com_tele;
    com_tele = NULL;

    setComboBoxEnabled(true);
    ui->actionOpenCom->setEnabled(true);
    ui->actionCloseCom->setEnabled(false);
    ui->sendmsgBtn->setEnabled(false);
    ui->sendMsgLineEdit->setEnabled(false);
    setTeleEnabled(false);
}

void Comteledialg::sendMsg()
{


}

void Comteledialg::readMyCom()
{
    qDebug() << "read: "<<com_tele->bytesAvailable()<<"bytes";
    QByteArray temp = com_tele->readAll();

    QString buf;

    for(int i = 0; i < temp.count(); i++)
    {
        QString s;
        s.sprintf("0x%02x, ", (unsigned char)temp.at(i));
        buf += s;
    }

    ui->textBrowser->setText(ui->textBrowser->document()->toPlainText() + buf);


}

void Comteledialg::on_sendmsgBtn_clicked()
{
    QByteArray buf;
    QString str;
    bool ok;
    char data;
    QStringList list;
    str = ui->sendMsgLineEdit->text();
    list = str.split(" ");
    for(int i = 0; i < list.count(); i++){
        if(list.at(i) == " ")
            continue;
        if(list.at(i).isEmpty())
            continue;
        data = (char)list.at(i).toInt(&ok, 16);
        if(!ok){
            QMessageBox::information(this, tr("提示消息"), tr("输入的数据格式有错误！"), QMessageBox::Ok);
            ui->sendmsgBtn->setText(tr("发送"));
            ui->sendmsgBtn->setIcon(QIcon(":new/prefix1/src/send.png"));
            return;
        }
        buf.append(data);
   }
    //buf = ui->sendMsgLineEdit->text().toLocal8Bit();//以ASCII码形式将数据写入串口
    com_tele->write(buf);
}

//清空记录
void Comteledialg::on_clearUpBtn_clicked()
{
    ui->textBrowser->clear();
}

void Comteledialg::comdatabuf(unsigned char addr1, unsigned char addr2,
                         unsigned char mode,
                         unsigned char x1, unsigned char x2,
                         unsigned char y1, unsigned char y2,
                         unsigned char z1, unsigned char z2,
                         unsigned char dir)
{

    QByteArray buf;
    unsigned char data[15] = {0xFE,         //帧头
                              0x0C,         //数据长度
                              addr1, addr2, //从机地址
                              0x05, 0x21,   //主机地址0x2105
                              mode,         //mode模式
                              x1,     x2,   //x方向（左右）
                              y1,     y2,   //y方向（前后）
                              z1,     z2,   //z方向（旋转）
                              dir,          //方向控制位
                              0xFF} ;       //帧尾

    for (unsigned char uc_index = 0; uc_index < 15; uc_index++)
    {
        buf.append(data[uc_index]);
    }

    //发送数据
    com_tele->write(buf);
}

void Comteledialg::on_startBtn_clicked()
{
    //发送启动数据
    comdatabuf(0x91, 0x90, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0X00, 0x00);
}

void Comteledialg::on_upBtn_clicked()
{
    //发送前进数据
    comdatabuf(0x91, 0x90, 0x01, 0x00, 0x00, 0x00, 0x10, 0x00, 0X00, 0x00);
}

void Comteledialg::on_backBtn_clicked()
{
    //发送后退数据
    comdatabuf(0x91, 0x90, 0x01, 0x00, 0x00, 0x00, 0x10, 0x00, 0X00, 0x02);
}

void Comteledialg::on_stopBtn_clicked()
{
    //发送停止数据
    comdatabuf(0x91, 0x90, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0X00, 0x00);
}

void Comteledialg::on_rightBtn_clicked()
{
    //发送右进数据
    comdatabuf(0x91, 0x90, 0x01, 0x00, 0x10, 0x00, 0x00, 0x00, 0X00, 0x00);
}

void Comteledialg::on_leftBtn_clicked()
{
    //发送左进数据
    comdatabuf(0x91, 0x90, 0x01, 0x00, 0x10, 0x00, 0x00, 0x00, 0X00, 0x04);
}
