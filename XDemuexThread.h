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
	/// 初始化XDemux*,XAudioThread*,XVideoThread*
	/// (如果你想仅仅播放音频或者音频进需要注释掉某一项的初始化)
	/// (并且在当前线程的运行中已经对于是否对音频或者视频解码进行判断)
	/// </summary>
	/// <param name="url">多媒体流</param>
	/// <returns>bool 成功与否</returns>
	virtual bool Open(const char* url);

	/// <summary>
	/// 启动所有线程
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

