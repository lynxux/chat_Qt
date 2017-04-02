#include "chat_own.h"
#include "ui_chat_own.h"
#include "per_chat.h"

chat_own::chat_own(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::chat_own)
{
    ui->setupUi(this);
    tcpsocket = new QTcpSocket(this);
    ip = getIP();
    connect(tcpsocket,SIGNAL(connected()),this, SLOT(connectedSlot()));
}


void chat_own::connectedSlot()
{
    qDebug()<<"connect OK!";
    QMessageBox::warning(this,tr("恭喜"),tr("连接成功！"),QMessageBox::Yes);
}

chat_own::~chat_own()
{
    delete ui;
}

void chat_own::on_sendButton_clicked() //发送消息
{
    QByteArray temp,name;
    //QString name;
    //QByteArray temp;
    name.append(ui->userNameLineEdit->text());
    //name.append(": ");
    temp.append(name+": "+ui->contentLineEdit->text());
    //qDebug()<<temp;
    //QHostAddress ipp(ip);
    //tcpsocket->setPeerAddress(ipp);
    //tcpsocket->setPeerName(ui->userNameLineEdit->text());
    tcpsocket->write(  temp + '\n' );

    tcpsocket->flush();
    ui->contentLineEdit->clear();
    ui->contentLineEdit->setFocus();
}

void chat_own::readMessage() //读取消息
{
    QString tmp;
    tmp = tcpsocket->readAll();

    if(tmp.contains("   @") ){
        ui->userListWidget->clear();
        tm = tmp.split("   @");
        for(int i = 0; i < tm.count(); i ++){
                ui->userListWidget->addItem(tm.at(i));
        }
    }
    else if(tmp.startsWith("&&"))
    {
        sm = tmp.split("&&");
        if(sm.at(3) == ip ){
            person_chat->chuanMessage( "<font color=blue>［自已］</font>" + sm.at(3) + ":<br>" + sm.at(2));
            if(sm.at(1) == tcpsocket->peerAddress().toString()){
                person_chat->chuanMessage("<font color=red>［对方］</font>" + sm.at(3) + ":<br>" + sm.at(2));
            }
        }
        else{
            //(sm.at(3) != ip){
//            person_chat = new per_chat;
            if(flag == 0) {
                //person_chat = new per_chat;
                person_chat = new per_chat;
                person_chat->show();
                flag ++;
                qDebug()<<"readMessage over!!!!XXXXXXX";
            }
            person_chat->TransIP(sm.at(3),ui->serverIPLineEdit->text());
            person_chat->chuanMessage("<font color=red>［对方］</font>" + sm.at(3) +":<br>" + sm.at(2));
                //person_chat = new per_chat;

                qDebug()<<"readMessage over!!!!";
            //}
        }
        qDebug()<<"readMessage over!";



    }
    else ui->displayTextEdit->append(tmp);
}

void chat_own::connectToServer(QString serverIP)
{
    tcpsocket->connectToHost(serverIP,8000); //连接到服务器的8000端口
}

QString chat_own::getIP() //获取ip地址
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

void chat_own::on_connectButton_clicked() //连接服务器
{
    QString serverIP = ui->serverIPLineEdit->text();

    ipname.append('%');
    ipname.append(ui->userNameLineEdit->text());
    qDebug()<<ipname;
    connectToServer(serverIP);
    if(!ui->userNameLineEdit->text().isEmpty()){
        tcpsocket->write(ipname);//连接服务器后发送各服务器的消息，包括ip地址和用户名
    }
    else QMessageBox::warning(this,tr("错误"),tr("无用户名！请先填写用户名！"),QMessageBox::Yes);
    ipname.clear();
    connect(tcpsocket,SIGNAL(readyRead()),this, SLOT(readMessage()));
}

void chat_own::on_disconnectButton_clicked() //断开服务器
{
    tcpsocket->disconnectFromHost();
    ui->connectButton->setDisabled(false);
    ui->serverIPLineEdit->clear();
}

void chat_own::on_userListWidget_doubleClicked(const QModelIndex &index)
{

    QString sip;
    sip.append(tm.at(index.row()));
    QStringList S;
    S = sip.split("\n");
    //person_chat->show();
    person_chat = new per_chat;
    person_chat->TransIP(S.at(0),ui->serverIPLineEdit->text());
    person_chat->TransName(S.at(1));
    person_chat->show();

}
