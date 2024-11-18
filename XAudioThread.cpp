#include "XAudioThread.h"

XAudioThread::XAudioThread():resample_(new XResample())
{
}

XAudioThread::~XAudioThread()
{
	isExit = true;
	wait();
}

void XAudioThread::run()
{
	/*这里没有运行*/
	unsigned char* pcm = new unsigned char[1024 * 1024 * 10];
	while (!isExit) {
		mtx_.lock();
		if (isPause_) {
			mtx_.unlock();
			msleep(5);
			continue;
		}
		AVPacket* pkt = Pop();
		bool ret = decode->Send(pkt);
		if (!ret) {
			mtx_.unlock();
			msleep(1);
			continue;
		}
		while (!isExit) {
			AVFrame* frame = decode->Recv();
			if (!frame) {
				break;
			}
			pts_ = decode->pts - audioplayer_.GetYetNotMS();
			int size = resample_->Resample(frame, pcm);
			while (!isExit) {
				if (size <= 0) {
					break;
				}
				if (audioplayer_.GetFree() < size || isPause_) {
					msleep(1);
					continue;
				}
				audioplayer_.Write(pcm, size);
				break;
			}
		}
		mtx_.unlock();
	}
	delete[] pcm;
	return;
}

bool XAudioThread::Open(AVCodecParameters* para,int sampleRate,int nb_channels)
{
	if (!para) {
		return false;
	}
	Clear();
	std::lock_guard<std::mutex> lck(mtx_);
	pts_ = 0;
	if (!resample_->Open(para,false)) {
		std::cerr << __FUNCTION__ << " : XResample::Open() Failed..." << std::endl;
		return false;
	}
	if (!audioplayer_.Open(sampleRate,nb_channels)) {
		
		resample_->Close();
		delete resample_;
		resample_ = nullptr;

		std::cerr << __FUNCTION__ << " : XAudioPlay::Open() Failed..." << std::endl;
		return false;
	}
	if (!decode->Open(para)) {
		
		resample_->Close();
		delete resample_;
		resample_ = nullptr;

		audioplayer_.Close();

		std::cerr << __FUNCTION__ << " : XDecodeThread::Open() Failed..." << std::endl;
		return false;
	}
	return true;
}


void XAudioThread::Clear()
{
	XDecodeThread::Clear();
	mtx_.lock();
	audioplayer_.Clear();
	mtx_.unlock();
}

void XAudioThread::Close()
{
	XDecodeThread::Close();
	std::lock_guard<std::mutex> lck(mtx_);
	if (resample_) {
		resample_->Close();
		delete resample_;
		resample_ = nullptr;
	}
	audioplayer_.Close();
	isPause_ = false;
	this->pts_ = 0;
}

void XAudioThread::SetPause(bool isPause)
{
	this->isPause_ = isPause;
	audioplayer_.SetPause(isPause);
}

