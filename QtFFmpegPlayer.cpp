#include "QtFFmpegPlayer.h"

QtFFmpegPlayer::QtFFmpegPlayer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::QtFFmpegPlayerClass())
{
    ui->setupUi(this);
}

QtFFmpegPlayer::~QtFFmpegPlayer()
{
    delete ui;
}
