#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtFFmpegPlayer.h"
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QSlider>
#include "XDemuexThread.h"
QT_BEGIN_NAMESPACE
namespace Ui { class QtFFmpegPlayerClass; };
QT_END_NAMESPACE

class QtFFmpegPlayer : public QMainWindow
{
    Q_OBJECT

public:
    QtFFmpegPlayer(QWidget *parent = nullptr);
    ~QtFFmpegPlayer();
    Ui::QtFFmpegPlayerClass* GetUI() {
        return ui;
    }
private:
    /*打开文件按钮*/
    void do_openFile();

    /*播放按钮*/
    void do_isPlay();

    /*滑动条点击和释放的时候*/
    void do_playPos_Pressed();
    void do_playPos_Released();

private:
    /*定时器、滑动条（显示）重载*/
    void timerEvent(QTimerEvent* event);

    /*窗口尺寸变化*/
    void resizeEvent(QResizeEvent* event);

    /*双击全屏*/
    void mouseDoubleClickEvent(QMouseEvent* event);

    /*是否开始播放*/
    void setPause(bool isPause);

private:
    Ui::QtFFmpegPlayerClass *ui;

    static XDemuexThread dt;

    bool isSliderPress = false;
};
