#include "chat_own.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    chat_own w;
    w.show();

    return a.exec();
}
