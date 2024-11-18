#include "XAudioPlay.h"

XAudioPlay& XAudioPlay::instance()
{
	static QtAudioPlayer player;
	return player;
}

XAudioPlay::~XAudioPlay()
{
}

XAudioPlay::XAudioPlay()
{

}

QtAudioPlayer::QtAudioPlayer()
{

}

QtAudioPlayer::~QtAudioPlayer()
{
	if (!is_Close) {
		Close();
	}
}

bool QtAudioPlayer::Open(int sample_rate, int nb_channels)
{
	Close();
	std::lock_guard<std::mutex> lck(Gmtx_);
	is_Close = false;
	QAudioFormat fmt;
	this->sampleRate_ = sample_rate;
	this->nb_channels_ = nb_channels;
	fmt.setSampleRate(sampleRate_);
	fmt.setSampleSize(sampelSize_);
	fmt.setChannelCount(nb_channels_);
	fmt.setCodec("audio/pcm");
	fmt.setByteOrder(QAudioFormat::LittleEndian);
	fmt.setSampleType(QAudioFormat::UnSignedInt);
	outPut_ = new QAudioOutput(fmt);
	io_ = outPut_->start();	//开始播放
	if (!io_) {
		outPut_->stop();
		delete outPut_;
		outPut_ = nullptr;
		return false;
	}
	return true;
}

void QtAudioPlayer::SetPause(bool isPause)
{
	std::lock_guard<std::mutex> lck(Gmtx_);
	if (!outPut_) {
		return;
	}
	if (isPause) {
		/*挂起*/
		outPut_->suspend();
	}
	else {
		/*恢复*/
		outPut_->resume();
	}
	return;
}

void QtAudioPlayer::Close()
{
	std::lock_guard<std::mutex> lck(Gmtx_);
	if (io_) {
		io_->close();
		io_ = nullptr;
	}
	
	if (outPut_) {
		outPut_->stop();
		delete outPut_;
		outPut_ = nullptr;
	}
	is_Close = true;
	return;
}

void QtAudioPlayer::Clear()
{
	std::lock_guard<std::mutex> lck(Gmtx_);
	if (io_) {
		io_->reset();
	}
}

bool QtAudioPlayer::Write(const unsigned char* data, int datasize)
{
	if (!data || datasize <= 0) {
		return false;
	}
	Gmtx_.lock();
	if (!outPut_ || !io_) {
		Gmtx_.unlock();
		return false;
	}
	int size = io_->write((char*)data, datasize);
	Gmtx_.unlock();
	if (datasize != size) {
		return false;
	}
	return true;
}

int QtAudioPlayer::GetFree()
{
	Gmtx_.lock();
	if (!outPut_) {
		Gmtx_.unlock();
		return 0;
	}
	int free = outPut_->bytesFree();
	Gmtx_.unlock();
	return free;
}

long long QtAudioPlayer::GetYetNotMS()
{
	Gmtx_.lock();
	if (!outPut_) {
		Gmtx_.unlock();
		return 0;
	}
	long long pts = 0;
	/*还未播放的字节数*/
	double yet_size = outPut_->bufferSize() - outPut_->bytesFree();
	/*一秒音频的字节大小*/
	double per_sec_size = sampleRate_* (sampelSize_ / 8)* nb_channels_;
	if (per_sec_size <= 0) {
		pts = 0;
	}
	else {
		pts = (yet_size / per_sec_size) * 1000;
	}
	Gmtx_.unlock();
	return pts;
}
