#pragma once
#include <QThread>
#include <mutex>
#include <list>
#include "XDecode.h"
#include "XAudioPlay.h"
#include "XResample.h"
/// <summary>
/// ������Ƶ�Ͳ�����Ƶ
/// </summary>
class XAudioThread: public QThread
{
public:
	XAudioThread();
	virtual ~XAudioThread();
	void run();
public:	
	/// <summary>
	/// ���ܳɹ���񶼻�����AVCodecParameters
	/// </summary>
	/// <param name="para">AVCodecParameters</param>
	/// <returns>bool �ɹ����</returns>
	virtual bool Open(AVCodecParameters* para);

	/// <summary>
	/// ���ڲ�list�������
	/// </summary>
	/// <param name="pkt">���ݰ�</param>
	/// <returns>bool �ɹ����</returns>
	virtual bool Push(AVPacket* pkt);

	/// <summary>
	/// �������
	/// </summary>
	virtual void Exit();
protected:
	std::atomic_bool is_Exit;
	XDecode* decode_ = nullptr;
	XAudioPlay& audioplayer_ = XAudioPlay::instance();
	XResample* resample_ = nullptr;
	std::list<AVPacket*> pkt_list_;
	/*������*/
	int MAX_LIST_SIZE = 100;
	std::mutex mtx_;
};

