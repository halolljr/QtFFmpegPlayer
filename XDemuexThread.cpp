#include "XDemuexThread.h"

XDemuexThread::XDemuexThread()
{

}

XDemuexThread::~XDemuexThread()
{
	if (!is_Exit_.load()) {
		Exit();
	}
}

bool XDemuexThread::Open(const char* url)
{
	if (!url || url[0] == '\n') {
		std::cout << __FUNCTION__ << " : URL Is Null..." << std::endl;
		return false;
	}
	std::lock_guard<std::mutex> lck(mtx_);
	if (!demux_) {
		demux_ = new XDemux();
	}
	if (!audioThread_) {
		audioThread_ = new XAudioThread();
	}
	if (!videoThread_) {
		videoThread_ = new XVideoThread();
	}
	if (!demux_ || !audioThread_ || !videoThread_) {
		std::cout << __FUNCTION__ << " : Members Are NULL..." << std::endl;
		return false;
	}
	if (!demux_->Open(url)) {
		std::cout << __FUNCTION__ << " : XDemux::Open() Failed..." << std::endl;
		return false;
	}
	if (!audioThread_->Open(demux_->CopyAPara())) {
		std::cout << __FUNCTION__ << " : XAudioThread::Open() Failed..." << std::endl;
		return false;
	}
	if (!videoThread_->Open(demux_->CopyVPara())) {
		std::cout << __FUNCTION__ << " : XVideoThread::Open() Failed..." << std::endl;
		return false;
	}
	is_Exit_.store(false);
	return true;
}

void XDemuexThread::Start()
{
	mtx_.lock();
	/*启动当前线程*/
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

void XDemuexThread::run()
{
	while (!is_Exit_.load()) {
		mtx_.lock();
		if (!demux_) {
			mtx_.unlock();
			msleep(4);
			continue;
		}
		AVPacket* pkt = demux_->Read();
		if (!pkt) {
			mtx_.unlock();
			msleep(4);
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
	}
}

void XDemuexThread::Exit()
{
	mtx_.lock();
	is_Exit_.store(true);
	if (demux_) {
		delete demux_;
	}
	if (audioThread_) {
		delete audioThread_;
	}
	if (videoThread_) {
		delete videoThread_;
	}
	wait();
	mtx_.unlock();
}
