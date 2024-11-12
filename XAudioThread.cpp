#include "XAudioThread.h"

XAudioThread::XAudioThread()
{
}

XAudioThread::~XAudioThread()
{
	if (!is_Exit.load()) {
		Exit();
		wait();
	}
}

void XAudioThread::run()
{
	/*�ز���֮��Ĵ������*/
	unsigned char* pcm = new unsigned char[1024 * 1024 * 10];
	while (!is_Exit.load()) {
		mtx_.lock();
		if (pkt_list_.empty()||!decode_||!resample_) {
			mtx_.unlock();
			msleep(1);
			continue;
		}
		/*ȡ��ͷ����ַ�Ŀ�����ɾ��ԭ����ͷ����ַ*/
		AVPacket* pkt = pkt_list_.front();
		pkt_list_.pop_front();
		bool ret = decode_->Send(pkt);
		if (!ret) {
			mtx_.unlock();
			msleep(1);
			continue;
		}
		/*һ��send���ܶ��recv*/
		while (!is_Exit.load()) {
			AVFrame* frame = decode_->Recv();
			if (!frame)
				break;
			/*�ز���*/
			int size = resample_->Resample(frame, pcm);
			/*������Ƶ*/
			while (!is_Exit.load()) {
				if (size <= 0)
					break;
				/*����û�в���*/
				if (audioplayer_.GetFree() < size) {
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
}

bool XAudioThread::Open(AVCodecParameters* para)
{
	if (!para) {
		return false;
	}
	std::lock_guard<std::mutex> lck(mtx_);
	if (!decode_) {
		decode_ = new XDecode();
	}
	if (!resample_) {
		resample_ = new XResample();
	}
	if (!decode_->Open(para)) {
		std::cout << __FUNCTION__ << " : XDecode Open() Failed..." << std::endl;
		return false;
	}
	if (!resample_->Open(decode_->GetAVCodecContext())){
		std::cout << __FUNCTION__ << " : XResample Open() Failed..." << std::endl;
		return false;
	}
	if (!audioplayer_.Open(decode_->GetAVCodecContext()->sample_rate, decode_->GetAVCodecContext()->ch_layout.nb_channels) ){
		std::cout << __FUNCTION__ << " : XAudioPlayer Open() Failed..." << std::endl;
		return false;
	}
	is_Exit.store(false);
	return true;
}

bool XAudioThread::Push(AVPacket* pkt)
{
	if (!pkt) {
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

void XAudioThread::Exit()
{
	std::lock_guard<std::mutex> lck(mtx_);
	decode_->Close();
	audioplayer_.Close();
	resample_->Close();
	pkt_list_.clear();
	is_Exit.store(true);
	wait();
}
