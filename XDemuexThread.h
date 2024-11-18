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
	/// 初始化XDemux*,XAudioThread*,XVideoThread*
	/// (如果你想仅仅播放音频或者音频进需要注释掉某一项的初始化)
	/// (并且在当前线程的运行中已经对于是否对音频或者视频解码进行判断)
	/// </summary>
	/// <param name="url">多媒体流</param>
	/// <param name="videoWidget">openGL显示窗口</param>
	/// <returns>bool 成功与否</returns>
	virtual bool Open(const char* url,IVideoCall* videoWidget);

	/// <summary>
	/// 启动所有线程
	/// </summary>
	virtual void Start();

	/// <summary>
	/// 清空一些缓存
	/// </summary>
	virtual void Clear();

	/// <summary>
	/// 清理资源并且关闭线程
	/// </summary>
	virtual void Close();

	/// <summary>
	/// 跳转
	/// </summary>
	/// <param name="pos"></param>
	virtual void Seek(double pos);
	
	/// <summary>
	/// 是否暂停或者停止
	/// </summary>
	/// <param name="isPause">暂停或者停止</param>
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

