#include "XDemuexThread.h"

XDemuexThread::XDemuexThread()
{

}

XDemuexThread::~XDemuexThread()
{
	is_Exit_ = true;
	wait();
}

bool XDemuexThread::Open(const char* url,IVideoCall* videoWidget)
{
	if (!url || url[0] == '\n') {
		std::cout << __FUNCTION__ << " : URL Is Null..." << std::endl;
		return false;
	}
	//Close();
	std::lock_guard<std::mutex> lck(mtx_);
	if (!demux_) {
		demux_ = new XDemux();
	}
	if (!videoThread_) {
		videoThread_ = new XVideoThread();
	}
	if (!audioThread_) {
		audioThread_ = new XAudioThread();
	}
	pts_ = 0;
	totalMs = 0;
	isPause = false;
	is_Exit_ = false;
	/*打开解封装*/
	if (!demux_->Open(url)) {
		std::cerr << __FUNCTION__ << " : XDemux::Open() Failed..." << std::endl;
		return false;
	}
	///*打开视频解码器和处理线程*/
	if (!videoThread_->Open(demux_->CopyVPara(), videoWidget, demux_->width_, demux_->height_)) {
		std::cerr << __FUNCTION__ << " : XVideoThread::Open() Failed..." << std::endl;
		return false;
	}
	/*打开音频解码器和处理线程*/
	if (!audioThread_->Open(demux_->CopyAPara(),demux_->sample_rate_,demux_->ch_layout_.nb_channels)) {
		std::cerr << __FUNCTION__ << " : XAudioThread::Open() Failed..." << std::endl;
		return false;
	}
	totalMs = demux_->totalMs_;
	return true;
}

void XDemuexThread::Start()
{
	mtx_.lock();
	/*改①======*/
	/*启动当前线程*/
	/*if (!demux_) {
		demux_ = new XDemux();
	}
	if (!videoThread_) {
		videoThread_ = new XVideoThread();
	}
	if (!audioThread_) {
		audioThread_ = new XAudioThread();
	}*/
	QThread::start();
	if (videoThread_) {
		videoThread_->start();
	}
	if (audioThread_) {
		audioThread_->start();
	}
	mtx_.unlock();
	return;
}

void XDemuexThread::Clear()
{
	std::lock_guard<std::mutex> lck(mtx_);
	if (demux_) {
		demux_->Clear();
	}
	if (videoThread_) {
		videoThread_->Clear();
	}
	if (audioThread_) {
		audioThread_->Clear();
	}
}

void XDemuexThread::Seek(double pos)
{
	Clear();
	mtx_.lock();
	bool status = this->isPause;
	mtx_.unlock();
	/*暂停*/
	SetPause(true);
	mtx_.lock();
	if (demux_) {
		/*这里需要清空缓冲*/
		demux_->Seek(pos);
	}
	long long seekPos = pos * demux_->totalMs_;
	while (!is_Exit_) {
		AVPacket* pkt = demux_->ReadVideo();
		if (!pkt) {
			break;
		}
		if (videoThread_->RepaintPts(pkt, seekPos)) {
			break;
		}
	}
	mtx_.unlock();
	if (!status) {
		SetPause(false);
	}
}

void XDemuexThread::SetPause(bool isPause)
{
	std::lock_guard<std::mutex> lck(mtx_);
	this->isPause = isPause;
	if (audioThread_) {
		audioThread_->SetPause(isPause);
	}
	if (videoThread_) {
		videoThread_->SetPause(isPause);
	}
}

void XDemuexThread::run()
{
	while (!is_Exit_) {
		mtx_.lock();
		if (isPause) {
			mtx_.unlock();
			msleep(5);
			continue;
		}
		if (!demux_) {
			mtx_.unlock();
			msleep(1);
			continue;
		}
		/*音视频同步*/
		/*你也可以考虑只有音频或者只有视频的时候*/
		if (audioThread_ && videoThread_) {
			pts_ = audioThread_->pts_;
			videoThread_->synpts = audioThread_->pts_;
		}
		AVPacket* pkt = demux_->Read();
		if (!pkt) {
			mtx_.unlock();
			msleep(5);
			continue;
		}
		/*判断数据是音频*/
		if (demux_->IsAudio(pkt)) {
			if(audioThread_)
				audioThread_->Push(pkt);
		}
		else {
			if(videoThread_)
				videoThread_->Push(pkt);
		}
		mtx_.unlock();
		//读取速度太快，导致不成功 ==========
		msleep(1);
	}
	return;
}

void XDemuexThread::Close()
{
	is_Exit_ = true;
	wait();
	if (demux_) {
		demux_->Close();
	}
	if (audioThread_) {
		audioThread_->Close();
	}
	if (videoThread_) {
		videoThread_->Exit();
	}
	mtx_.lock();
	delete audioThread_;
	audioThread_ = nullptr;
	delete videoThread_;
	videoThread_ = nullptr;
	mtx_.unlock();
	return;
}
