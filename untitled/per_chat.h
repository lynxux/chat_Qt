#ifndef PER_CHAT_H
#define PER_CHAT_H

#include <QDialog>
#include <QNetworkInterface>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QMessageBox>


namespace Ui {
class per_chat;
}

class per_chat : public QDialog
{
    Q_OBJECT

public:
    explicit per_chat(QWidget *parent = 0);
    ~per_chat();
    void TransIP(QString ,QString );
    QString TransName(QString );
    QString getIP();
    void chuanMessage(QString );

private slots:
    void on_sendButton_clicked();

private:
    Ui::per_chat *ui;
    QTcpSocket *p_tcpsocket;
    QString ip,ip_rec,server_ip;
    QStringList tm;

};

#endif // PER_CHAT_H
