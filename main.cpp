﻿#include "QtFFmpegPlayer.h"
#include <windows.h>
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    QtFFmpegPlayer w;
    w.show();
    return a.exec();
}

