#include "XVideoThread.h"

XVideoThread::XVideoThread():sdlRender_()
{

}

XVideoThread::~XVideoThread()
{
	if (!is_Exit.load()) {
		Exit();
		wait();
	}
}

void XVideoThread::run()
{
	while (!is_Exit.load()) {
		mtx_.lock();
		if (pkt_list_.empty() || !decodec_) {
			mtx_.unlock();
			msleep(1);
			continue;
		}
		AVPacket* pkt = pkt_list_.front();
		pkt_list_.pop_front();
		if (!sdlRender_.video_decodec(decodec_->GetAVCodecContext(), pkt)) {
			mtx_.unlock();
			msleep(1);
			continue;
		}
		mtx_.unlock();
	}
	/*强制刷新剩余的视频缓冲*/
	sdlRender_.video_decodec(decodec_->GetAVCodecContext(), nullptr);
}

bool XVideoThread::Open(AVCodecParameters* para)
{
	std::lock_guard<std::mutex> lck(mtx_);
	if (!para) {
		return false;
	}
	if (!decodec_) {
		decodec_ = new XDecode();
	}
	if (!decodec_) {
		return false;
	}
	else {
		if (decodec_->Open(para)) {
			is_Exit.store(false);
			return true;
		}
		else {
			return false;
		}
	}
}

bool XVideoThread::Push(AVPacket* pkt)
{
	if(!pkt) {
		return false;
	}
	while (!is_Exit.load()) {
		mtx_.lock();
		if (pkt_list_.size() < MAX_LIST_SIZE) {
			pkt_list_.push_back(pkt);
			mtx_.unlock();
			break;
		}
		mtx_.unlock();
		msleep(1);
	}
	return true;
}

void XVideoThread::Exit()
{
	std::lock_guard<std::mutex> lck(mtx_);
	decodec_->Close();
	sdlRender_.Close();
	pkt_list_.clear();
	is_Exit.store(true);
	wait();
	return;
}
