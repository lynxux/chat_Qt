#ifndef CHAT_OWN_H
#define CHAT_OWN_H

#include <QWidget>
#include <QTcpSocket>
#include <QNetworkInterface>
#include <QMessageBox>

namespace Ui {
class chat_own;
}

class chat_own : public QWidget
{
    Q_OBJECT

public:
    explicit chat_own(QWidget *parent = 0);
    ~chat_own();
    QString getIP(); //获得IP地址
    void connectToServer(QString ); //连接服务器

private slots:
    void on_sendButton_clicked(); //按下发送按钮
    void readMessage(); //读取消息

    void on_connectButton_clicked();

    void on_disconnectButton_clicked();
    void connectedSlot();

    void on_userListWidget_doubleClicked(const QModelIndex &index);

private:
    Ui::chat_own *ui;
    QTcpSocket *tcpsocket;
    QString ip;
    QStringList tm;
    QByteArray ipname;



};

#endif // CHAT_OWN_H
