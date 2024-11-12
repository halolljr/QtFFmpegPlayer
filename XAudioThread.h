#pragma once
#include <QThread>
#include <mutex>
#include <list>
#include "XDecode.h"
#include "XAudioPlay.h"
#include "XResample.h"
/// <summary>
/// 解码音频和播放音频
/// </summary>
class XAudioThread: public QThread
{
public:
	XAudioThread();
	virtual ~XAudioThread();
	void run();
public:	
	/// <summary>
	/// 不管成功与否都会清理AVCodecParameters
	/// </summary>
	/// <param name="para">AVCodecParameters</param>
	/// <returns>bool 成功与否</returns>
	virtual bool Open(AVCodecParameters* para);

	/// <summary>
	/// 往内部list填充数据
	/// </summary>
	/// <param name="pkt">数据包</param>
	/// <returns>bool 成功与否</returns>
	virtual bool Push(AVPacket* pkt);

	/// <summary>
	/// 清空数据
	/// </summary>
	virtual void Exit();
protected:
	std::atomic_bool is_Exit;
	XDecode* decode_ = nullptr;
	XAudioPlay& audioplayer_ = XAudioPlay::instance();
	XResample* resample_ = nullptr;
	std::list<AVPacket*> pkt_list_;
	/*最大队列*/
	int MAX_LIST_SIZE = 100;
	std::mutex mtx_;
};

