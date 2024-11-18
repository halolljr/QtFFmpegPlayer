#pragma once
#include <iostream>
#include <mutex>
#include <list>
#include <thread>
#include <QThread>
#include "XDecodeThread.h"
#include "XDecode.h"
#include "XVideoWidget.h"
//#include "SDLRender.h"
/// <summary>
/// �������ʾ��Ƶ
/// </summary>
class XVideoThread:public XDecodeThread
{
public:
	XVideoThread();
	virtual ~XVideoThread();
public:
	
	/// <summary>
	/// ����pts,������յ��Ľ�������pts >= seekpts��return true;������ʾ����
	/// </summary>
	/// <param name="pkt">����İ�</param>
	/// <param name="seekPos">λ��</param>
	/// <returns>�ɹ����</returns>
	virtual bool RepaintPts(AVPacket* pkt, long long seekPos);

	/// <summary>
	/// ��ʼ���ڲ�������{ÿһ��Open����Clear()}
	/// </summary>
	/// <param name="para">XDemux��CopyVPara()</param>
	/// <returns>bool �ɹ����</returns>
	virtual bool Open(AVCodecParameters* para,IVideoCall* call,int width,int height);

	void SetPause(bool isPause);

	void run();
	virtual void Exit();
protected:
	/*����openGL���*/
	IVideoCall* call = nullptr;

	bool isPause = false;

	std::mutex mtx_;
	
public:
	/*ͬ��ʱ�䣬���ⲿ����*/
	long long synpts = 0;
};

