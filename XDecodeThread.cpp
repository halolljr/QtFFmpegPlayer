#include "XDecodeThread.h"

XDecodeThread::XDecodeThread()
{
	if (!decode) {
		decode = new XDecode();
	}
}

XDecodeThread::~XDecodeThread()
{
	isExit = true;
	wait();
}

void XDecodeThread::Push(AVPacket* pkt)
{
	if (!pkt) {
		return;
	}
	while (!isExit) {
		mtx_.lock();
		if (list_pkt_.size() < MAX_LIST_SIZE) {
			list_pkt_.push_back(pkt);
			mtx_.unlock();
			break;
		}
		mtx_.unlock();
		msleep(1);
	}
}

void XDecodeThread::Clear()
{
	std::lock_guard<std::mutex> lck(mtx_);
	decode->Clear();
	while (!list_pkt_.empty()) {
		AVPacket* pkt = list_pkt_.front();
		av_packet_free(&pkt);
		list_pkt_.pop_front();
	}
}

void XDecodeThread::Close()
{
	Clear();
	/*等待线程退出*/
	isExit = true;
	wait();
	decode->Close();
	mtx_.lock();
	delete decode;
	decode = nullptr;
	mtx_.unlock();
}

AVPacket* XDecodeThread::Pop()
{
	std::lock_guard<std::mutex> lck(mtx_);
	if (list_pkt_.empty()) {
		return nullptr;
	}
	AVPacket* pkt = list_pkt_.front();
	list_pkt_.pop_front();
	return pkt;
}
