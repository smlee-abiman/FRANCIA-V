#include <QCoreApplication>
#include <QTimer>
#include "appmain.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    AppMain appMain(nullptr, &a);
    QTimer::singleShot(0, &appMain, &AppMain::run);

    return a.exec();
}
