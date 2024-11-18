#pragma once
#include <QThread>
#include "XDecodeThread.h"
#include "XAudioPlay.h"
#include "XResample.h"
/// <summary>
/// 解码音频和播放音频
/// </summary>
class XAudioThread: public XDecodeThread
{
public:
	XAudioThread();
	virtual ~XAudioThread();
	void run();
public:	
	/// <summary>
	/// 不管成功与否都会清理AVCodecParameters(每一次打开都会调用Clear)
	/// </summary>
	/// <param name="para">AVCodecParameters</param>
	/// <returns>bool 成功与否</returns>
	virtual bool Open(AVCodecParameters* para,int sampleRate,int nb_channels);

	/// <summary>
	/// 往内部list填充数据
	/// </summary>
	/// <param name="pkt">数据包</param>
	/// <returns>bool 成功与否</returns>
	/*virtual bool Push(AVPacket* pkt);*/

	/// <summary>
	/// 清理缓冲
	/// </summary>
	virtual void Clear();

	/// <summary>
	/// 停止线程，清空资源
	/// </summary>
	virtual void Close();

	/// <summary>
	/// 设置暂停或者播放
	/// </summary>
	/// <param name="isPause">暂停播放</param>
	void SetPause(bool isPause);
protected:
	
	XAudioPlay& audioplayer_ = XAudioPlay::instance();
	
	XResample* resample_ = nullptr;

	std::mutex mtx_;
	
public:
	/*当前音频播放的pts*/
	long long pts_ = 0;
	/*是暂停或者播放*/
	bool isPause_ = false;
};

