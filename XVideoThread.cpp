#include "XVideoThread.h"

XVideoThread::XVideoThread()
{

}

XVideoThread::~XVideoThread()
{
}

bool XVideoThread::RepaintPts(AVPacket* pkt, long long seekPos)
{
	mtx_.lock();
	bool ret = decode->Send(pkt);
	if (!ret) {
		mtx_.unlock();
		/*表示解码结束*/
		return true;
	}
	AVFrame* frame = decode->Recv();
	if (!frame) {
		mtx_.unlock();
		return false;
	}
	if (decode->pts == seekPos) {
		/*等于关键帧，需要显示出去*/
		if (call) {
			call->Repaint(frame);
		}
		mtx_.unlock();
		return true;
	}
	av_frame_free(&frame);
	mtx_.unlock();
	return false;
}

void XVideoThread::run()
{
	while (!isExit) {
		mtx_.lock();
		if (this->isPause) {
			mtx_.unlock();
			msleep(5);
			continue;
		}
		// 视频快了，就需要等  , 没有音频不需要等待
		if (synpts > 0 && synpts < decode->pts) {
			mtx_.unlock();
			msleep(1);
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
			if (call) {
				call->Repaint(frame);
			}
		}
		mtx_.unlock();
	}
	return;
}

void XVideoThread::Exit()
{

}

bool XVideoThread::Open(AVCodecParameters* para,IVideoCall* call,int width,int height)
{
	if (!para) {
		return false;
	}
	Clear();
	mtx_.lock();
	synpts = 0;
	/*初始化显示窗口*/
	this->call = call;
	if (call) {
		this->call->Init(width, height);
	}
	mtx_.unlock();
	if (!decode->Open(para)) {
		std::cerr << __FUNCTION__ << " : XDecode::Open() Failed..." << std::endl;
		return false;
	}
	return true;
}

void XVideoThread::SetPause(bool isPause)
{
	mtx_.lock();
	this->isPause = isPause;
	mtx_.unlock();
}

