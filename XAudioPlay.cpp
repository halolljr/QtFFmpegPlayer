#include "XAudioPlay.h"

XAudioPlay& XAudioPlay::instance()
{
	static QtPlayer player;
	return player;
}

XAudioPlay::~XAudioPlay()
{

}

XAudioPlay::XAudioPlay()
{

}

bool QtPlayer::Open(int sample_rate, int nb_channels)
{
	std::lock_guard<std::mutex> lck(Gmtx_);
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
		return false;
	}
	return true;
}

void QtPlayer::Close()
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
}

bool QtPlayer::Write(const unsigned char* data, int datasize)
{
	if (!data || datasize <= 0) {
		return false;
	}
	std::lock_guard<std::mutex> lck(Gmtx_);
	if (!outPut_ || !io_) {
		return false;
	}
	int size = io_->write((char*)data, datasize);
	if (datasize != size) {
		return false;
	}
	return true;
}

int QtPlayer::GetFree()
{
	std::lock_guard<std::mutex> lck(Gmtx_);
	if (!outPut_) {
		return 0;
	}
	int free = outPut_->bytesFree();
	return free;
}
