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
		/*��ʾ�������*/
		return true;
	}
	AVFrame* frame = decode->Recv();
	if (!frame) {
		mtx_.unlock();
		return false;
	}
	if (decode->pts == seekPos) {
		/*���ڹؼ�֡����Ҫ��ʾ��ȥ*/
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
		// ��Ƶ���ˣ�����Ҫ��  , û����Ƶ����Ҫ�ȴ�
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
	/*��ʼ����ʾ����*/
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

