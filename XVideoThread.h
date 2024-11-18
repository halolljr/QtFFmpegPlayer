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
/// 解码和显示视频
/// </summary>
class XVideoThread:public XDecodeThread
{
public:
	XVideoThread();
	virtual ~XVideoThread();
public:
	
	/// <summary>
	/// 解码pts,如果接收到的解码数据pts >= seekpts则return true;并且显示画面
	/// </summary>
	/// <param name="pkt">解码的包</param>
	/// <param name="seekPos">位置</param>
	/// <returns>成功与否</returns>
	virtual bool RepaintPts(AVPacket* pkt, long long seekPos);

	/// <summary>
	/// 初始化内部解码器{每一次Open都会Clear()}
	/// </summary>
	/// <param name="para">XDemux的CopyVPara()</param>
	/// <returns>bool 成功与否</returns>
	virtual bool Open(AVCodecParameters* para,IVideoCall* call,int width,int height);

	void SetPause(bool isPause);

	void run();
	virtual void Exit();
protected:
	/*调用openGL组件*/
	IVideoCall* call = nullptr;

	bool isPause = false;

	std::mutex mtx_;
	
public:
	/*同步时间，由外部传入*/
	long long synpts = 0;
};

