#ifndef SERVER_H
#define SERVER_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>

#include <QNetworkInterface>


namespace Ui {
class Server;
}

class Server : public QWidget
{
    Q_OBJECT

public:
    explicit Server(QWidget *parent = 0);
    ~Server();
    QString getIP(); // 获得ip地址
    void readMessage();
    void list(); //在线列表函数
private slots:
    void on_connectButton_clicked(); //连接监听
    void newConnectSlot();
    void readClient();
    void removeUserFormList(); //用户下线

private:
    Ui::Server *ui;
    QTcpServer *tcpServer;
    QString ip;
    QString ips,names; //存储单个ip地址和用户名
    QStringList ipname,ipss,namess;  //存储多个ip地址和用户名，发给客户端形成在线列表
    QList < QTcpSocket *> userList;
    QTcpSocket *clientConnection;

};

#endif // SERVER_H
