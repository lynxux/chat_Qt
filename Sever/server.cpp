#include "server.h"
#include "ui_server.h"

Server::Server(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Server)
{
    ui->setupUi(this);
    ip = getIP();
    ui->ServeripLineEdit->append(ip);
    ui->ServeripLineEdit->setReadOnly(true);
    ui->lineEdit->setText("8000");
    tcpServer = new QTcpServer(this);
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(newConnectSlot()));
}

Server::~Server()
{
    delete ui;
}

void Server::newConnectSlot()
{
    clientConnection = tcpServer->nextPendingConnection();
    userList << clientConnection;
    connect(clientConnection, SIGNAL(readyRead()), this, SLOT(readClient()));

    QByteArray tmp;
    //QByteArray str;
    //QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
    //str.append(clientConnection->readAll());//读取tcp发过来的消息
    //QString t(str);
    //QStringList te = t.split("%");
    //QString IPP;
    //.append(te.at(0));
    //QString Name;
    //Name.append(te.at(1));
    //qDebug()<<clientConnection->readAll();
    //QByteArray str = clientConnection ->readAll();
    //QString t(str);
    //QStringList te = t.split('%');
    QString IPP = clientConnection->peerAddress().toString();
    //QByteArray tt = clientConnection->readAll();
    //QString Name(tt);
    //tmp.append(clientConnection->peerAddress().toString());
    //qDebug()<<IPP<<Name;
    tmp.append(IPP);
    tmp.append("上线了！");
    ui->textEdit->append(tmp);

    connect(clientConnection,SIGNAL(disconnected()),this,SLOT(removeUserFormList())); //下线删除列表选项
}

void Server::readClient()
{
    QByteArray str;// = clientConnection->readAll();
    QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
    str.append(socket->readAll());//读取tcp发过来的消息

    if(str[0]=='%')
    {
        QString tmptmp(str); //复制字符串
        ipname = tmptmp.split('%');  //以%分割
        //ips.append(ipname.at(0)); //第一项为ip地址
        //ips.append('^');  //多个ip之间以^分割
        names.append(ipname.at(1)); //第二项为用户名
        names.append('^');  //多个用户名之间也以^分割

        //ipss = ips.split('^');  //去除^，获得所有ip地址
        namess = names.split('^'); //去除^ ，获得所有用户名

        ipss.append(socket->peerAddress().toString());
        //namess.append(socket->peerName());

        list();  //调用函数，发送给客户端消息，形成在线列表
    }
    else if(str[0]=='&')
    {
        QString tmptmp(str); //复制字符串
        ipname = tmptmp.split("&&");  //以&&分割
        p_his_ip.append(ipname.at(1));
        p_my_ip.append(ipname.at(3));

        for(int i = 0; i < userList.count(); i++)
        {
            //userList.at(i)->write(str);
            //if(userList.at(i)->peerAddress().toString()==p_his_ip||userList.at(i)->peerAddress().toString()==p_my_ip)
            //{
                userList.at(i)->write(str);
            //}
        }

    }
    else
    {
        for(int i = 0; i < userList.count(); i++)
        {
            userList.at(i)->write(str);
        }
    }

}

void Server::on_connectButton_clicked()
{

    if(tcpServer->listen(QHostAddress::Any, 8000)){
        ui->textEdit->append("listen OK!");
    }else{
        ui->textEdit->append("listen error!");
    }
}

QString Server::getIP()
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

void Server::removeUserFormList() //更新在线列表
{
    QString tmp;

    QTcpSocket* socket = static_cast<QTcpSocket*>(sender());

    socket->peerAddress().toString();

    if(userList.removeOne(socket)){
        tmp.append(socket->peerAddress().toString());
        tmp.append("下线了！");\
        qDebug()<<tmp;
        ui->textEdit->append(tmp);
        list();
    }
}

void Server::list()
{
    QByteArray msg;
    QString aa;
    for(int i = 0; i < userList.count() && userList.at(i)->peerAddress().toString() != aa; i++){
        msg.append( userList.at(i)->peerAddress().toString());
        //qDebug()<<"msg "+msg;
         for(int k = 0; k <ipss.count(); k++){
             if( userList.at(i)->peerAddress().toString()== ipss.at(k)){
                 msg.append("\n");
                 msg.append(namess.at(k));
                 //qDebug()<<"msg "+msg;
                 break;
             }
         }
        msg.append("   @");  //以@为标志，使客户端认识此消息
        aa =  userList.at(i)->peerAddress().toString();
    }
    for(int j = 0; j <userList.count(); j++){ //发送给所有用户
        userList.at(j)->write(msg);
        //qDebug()<<msg;
    }
    msg.clear();
}

