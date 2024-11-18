#pragma once
#include "IVideoCall.h"
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
	/// <param name="videoWidget">openGL��ʾ����</param>
	/// <returns>bool �ɹ����</returns>
	virtual bool Open(const char* url,IVideoCall* videoWidget);

	/// <summary>
	/// ���������߳�
	/// </summary>
	virtual void Start();

	/// <summary>
	/// ���һЩ����
	/// </summary>
	virtual void Clear();

	/// <summary>
	/// ������Դ���ҹر��߳�
	/// </summary>
	virtual void Close();

	/// <summary>
	/// ��ת
	/// </summary>
	/// <param name="pos"></param>
	virtual void Seek(double pos);
	
	/// <summary>
	/// �Ƿ���ͣ����ֹͣ
	/// </summary>
	/// <param name="isPause">��ͣ����ֹͣ</param>
	virtual void SetPause(bool isPause);

	void run();
public:
	long long pts_ = 0;
	long long totalMs = 0;
	bool isPause = false;
protected:
	XDemux* demux_ = nullptr;
	XAudioThread* audioThread_ = nullptr;
	XVideoThread* videoThread_ = nullptr;
	std::mutex mtx_;
	bool is_Exit_ = false;
};

