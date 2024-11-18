#pragma once
#include <iostream>
#include <mutex>
#include <list>
#include <QThread>
#include "XDecode.h"
class XDecodeThread:public QThread
{
public:
	XDecodeThread();
	virtual ~XDecodeThread();
public:
	
	/// <summary>
	/// 向队列中插入数据（浅拷贝）
	/// </summary>
	/// <param name="pkt">AVPacke* 包</param>
	virtual void Push(AVPacket* pkt);

	/// <summary>
	/// 清理队列
	/// </summary>
	virtual void Clear();

	/// <summary>
	/// 清理资源，停止线程
	/// </summary>
	virtual void Close();

	/// <summary>
	/// 取出一帧数据，并出栈，如果没有则返回NULL
	/// </summary>
	/// <returns>取出的数据，没有则返回NULL</returns>
	virtual AVPacket* Pop();

protected:
	std::list<AVPacket*> list_pkt_;
	int MAX_LIST_SIZE = 100;
	bool isExit = false;
	std::mutex mtx_;
	XDecode* decode = nullptr;
};

