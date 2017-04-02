#include "per_chat.h"
#include "ui_per_chat.h"

per_chat::per_chat(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::per_chat)
{
    ui->setupUi(this);
    p_tcpsocket = new QTcpSocket(this);

    ip = getIP();
    this->setWindowTitle("私人聊天中...");
    //p_tcpsocket->connectToHost(sever_ip,8000);

}

per_chat::~per_chat()
{
    delete ui;
}

void per_chat::on_sendButton_clicked()
{
    QByteArray tmp;
    tmp.append("&&");
    tmp.append(ip_rec + "&&");
    tmp.append(ui->contentLineEdit->text() + "&&");
    tmp.append(ip);
    p_tcpsocket->write( tmp );

    p_tcpsocket->flush();
    ui->contentLineEdit->clear();
    ui->contentLineEdit->setFocus();
}


QString per_chat::getIP()
{
    QList<QHostAddress> list  = QNetworkInterface::allAddresses();  //获取ipv4和ipv6地址
    foreach(QHostAddress address,list)
    {
        if(address.protocol()==QAbstractSocket::IPv4Protocol)
        {
            return address.toString();
        }
    }
    return 0;
}


void per_chat::TransIP(QString ip,QString serverip)
{
    ip_rec = ip;
    server_ip = serverip;
    ui->recip_lineEdit->setText(ip_rec);
    ui->recip_lineEdit->setReadOnly(true);
    p_tcpsocket->connectToHost(server_ip,8000);
}

QString per_chat::TransName(QString Name)
{
    return Name;
}


void per_chat::chuanMessage(QString a)
{
    ui->displayTextEdit->append(a);
    ui->displayTextEdit->setReadOnly(true);
}
