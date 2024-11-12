#pragma once
#include "XDemux.h"
#include "XAudioThread.h"
#include "XVideoThread.h"
#include <QThread>
class XDemuexThread:public QThread
{
public:
	XDemuexThread();
	virtual ~XDemuexThread();
public:
	/// <summary>
	/// ��ʼ��XDemux*,XAudioThread*,XVideoThread*
	/// (����������������Ƶ������Ƶ����Ҫע�͵�ĳһ��ĳ�ʼ��)
	/// (�����ڵ�ǰ�̵߳��������Ѿ������Ƿ����Ƶ������Ƶ��������ж�)
	/// </summary>
	/// <param name="url">��ý����</param>
	/// <returns>bool �ɹ����</returns>
	virtual bool Open(const char* url);

	/// <summary>
	/// ���������߳�
	/// </summary>
	virtual void Start();

	void run();
	void Exit();
protected:
	XDemux* demux_ = nullptr;
	XAudioThread* audioThread_ = nullptr;
	XVideoThread* videoThread_ = nullptr;
	std::mutex mtx_;
	std::atomic_bool is_Exit_;
};

