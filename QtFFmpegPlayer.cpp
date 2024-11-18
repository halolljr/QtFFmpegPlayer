#include "QtFFmpegPlayer.h"

XDemuexThread QtFFmpegPlayer::dt;

QtFFmpegPlayer::QtFFmpegPlayer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::QtFFmpegPlayerClass())
{
    ui->setupUi(this); 
	/*改②======*/
	//dt.Start();
    connect(ui->openFile, &QPushButton::clicked, this, &QtFFmpegPlayer::do_openFile);
	connect(ui->isPlay, &QPushButton::clicked, this, &QtFFmpegPlayer::do_isPlay);
	connect(ui->playPos, &QSlider::sliderPressed, this, &QtFFmpegPlayer::do_playPos_Pressed);
	connect(ui->playPos, &QSlider::sliderReleased, this, &QtFFmpegPlayer::do_playPos_Released);
	/*一秒25帧，40ms的定时器*/
	startTimer(40);
}

QtFFmpegPlayer::~QtFFmpegPlayer()
{
	dt.Close();
    delete ui;
}

void QtFFmpegPlayer::do_openFile()
{
	// 选择文件
	QString name = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("选择视频"));
	if (name.isEmpty()) return;
	this->setWindowTitle(name);
	if (!dt.Open(name.toLocal8Bit(), ui->openGLWidget))   // 打开失败
	{
		QMessageBox::information(0, "error", "open file failed!");
		return;
	}
	/*改③====*/
	dt.Start();
	setPause(dt.isPause);
}

void QtFFmpegPlayer::do_isPlay()
{
	bool isPause = !dt.isPause;
	setPause(isPause);
	dt.SetPause(isPause);
}

void QtFFmpegPlayer::do_playPos_Pressed()
{
	isSliderPress = true;
}

void QtFFmpegPlayer::do_playPos_Released()
{
	isSliderPress = false;
	double pos = 0.0;
	pos = (double)ui->playPos->value() / (double)ui->playPos->maximum();
	dt.Seek(pos);
}

void QtFFmpegPlayer::timerEvent(QTimerEvent* event)
{
	if (isSliderPress) {
		return; /*按住滑动条，直接返回，不再刷新画面*/
	}
	long long total = dt.totalMs;
	if (total > 0) {
		double pos = (double)dt.pts_ / (double)total;
		int v = ui->playPos->maximum() * pos;
		ui->playPos->setValue(v);
	}
}

void QtFFmpegPlayer::resizeEvent(QResizeEvent* event)
{
	ui->playPos->move(50, this->height() - 100);
	ui->playPos->resize(this->width() - 100, ui->playPos->height());
	ui->openFile->move(100, this->height() - 150);
	ui->openGLWidget->resize(this->size());
	ui->isPlay->move(ui->openFile->x() + ui->openFile->width() + 10, ui->openFile->y());
}

void QtFFmpegPlayer::mouseDoubleClickEvent(QMouseEvent* event)
{
	if (isFullScreen())
		this->showNormal();
	else
	{
		this->showFullScreen();
	}
}

void QtFFmpegPlayer::setPause(bool isPause)
{
	if (isPause)
		ui->isPlay->setText(QString::fromLocal8Bit("播 放"));
	else
		ui->isPlay->setText(QString::fromLocal8Bit("暂 停"));
}
