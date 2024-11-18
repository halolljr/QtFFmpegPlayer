#pragma once
#include <QThread>
#include "XDecodeThread.h"
#include "XAudioPlay.h"
#include "XResample.h"
/// <summary>
/// ������Ƶ�Ͳ�����Ƶ
/// </summary>
class XAudioThread: public XDecodeThread
{
public:
	XAudioThread();
	virtual ~XAudioThread();
	void run();
public:	
	/// <summary>
	/// ���ܳɹ���񶼻�����AVCodecParameters(ÿһ�δ򿪶������Clear)
	/// </summary>
	/// <param name="para">AVCodecParameters</param>
	/// <returns>bool �ɹ����</returns>
	virtual bool Open(AVCodecParameters* para,int sampleRate,int nb_channels);

	/// <summary>
	/// ���ڲ�list�������
	/// </summary>
	/// <param name="pkt">���ݰ�</param>
	/// <returns>bool �ɹ����</returns>
	/*virtual bool Push(AVPacket* pkt);*/

	/// <summary>
	/// ������
	/// </summary>
	virtual void Clear();

	/// <summary>
	/// ֹͣ�̣߳������Դ
	/// </summary>
	virtual void Close();

	/// <summary>
	/// ������ͣ���߲���
	/// </summary>
	/// <param name="isPause">��ͣ����</param>
	void SetPause(bool isPause);
protected:
	
	XAudioPlay& audioplayer_ = XAudioPlay::instance();
	
	XResample* resample_ = nullptr;

	std::mutex mtx_;
	
public:
	/*��ǰ��Ƶ���ŵ�pts*/
	long long pts_ = 0;
	/*����ͣ���߲���*/
	bool isPause_ = false;
};

