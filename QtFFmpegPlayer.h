#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtFFmpegPlayer.h"
#include <QAudioFormat>
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
    Ui::QtFFmpegPlayerClass *ui;
};
