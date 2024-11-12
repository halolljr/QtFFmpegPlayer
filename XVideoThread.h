#pragma once
#include <iostream>
#include <mutex>
#include <list>
#include <QThread>
#include "XDecode.h"
#include "SDLRender.h"
/// <summary>
/// 解码和显示视频
/// </summary>
class XVideoThread:public QThread
{
public:
	XVideoThread();
	virtual ~XVideoThread();
public:
	void run();
	
	/// <summary>
	/// 初始化内部解码器
	/// </summary>
	/// <param name="para">XDemux的CopyVPara()</param>
	/// <returns>bool 成功与否</returns>
	virtual bool Open(AVCodecParameters* para);
	
	/// <summary>
	/// 尾插AVPacket*数据入内部队列
	/// </summary>
	/// <param name="pkt">AVPacket* 数据包</param>
	/// <returns>bool 成功与否</returns>
	virtual bool Push(AVPacket* pkt);
	
	virtual void Exit();
protected:

	/*解码器上下文结构体*/
	XDecode* decodec_ = nullptr;

	/*视频渲染器*/
	SDLRender sdlRender_;

	/*AVPacket*包队列*/
	std::list<AVPacket*> pkt_list_;
	
	/*AVPacket*包队列最大个数*/
	int MAX_LIST_SIZE = 100;
	
	std::atomic_bool is_Exit;
	std::mutex mtx_;
};

