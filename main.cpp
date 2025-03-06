#include "mainwindow.h"

#include <QApplication>
#include <QNetworkConfigurationManager>

int main(int argc, char *argv[])
{
    QNetworkConfigurationManager manager;
        if (!manager.isOnline()) {
            qDebug() << "警告：无法检测网络状态（可能已禁用 bearer 模块）。";
        }

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
