#pragma once
#include <iostream>
#include <QAudioFormat>
#include <QAudioOutput>
#include <QIODevice>
#include <mutex>
/*
	工厂模式 + 单例模式
	禁止拷贝构造和拷贝赋值
*/
class XAudioPlay
{
public:
	/// <summary>
	/// 这里使用的是Qt播放器，如果你使用其他播放器，就里面修改
	/// C++11以后保证返回局部静态变量线程安全
	/// </summary>
	/// <returns>返回播放器单例实例</returns>
	static XAudioPlay& instance();

	/// <summary>
	/// 初始化播放参数并打开播放器(每一次打开之前都会关闭之前拥有的资源)
	/// </summary>
	/// <param name="sample_rate">采样率</param>
	/// <param name="nb_channels">通道个数</param>
	/// <returns>bool 成功与否</returns>
	virtual bool Open(int sample_rate,int nb_channels) = 0;

	/// <summary>
	/// 设置暂停还是播放
	/// </summary>
	/// <param name="isPause">暂停或者播放</param>
	virtual void SetPause(bool isPause) = 0;

	/// <summary>
	/// 关闭播放器并释放参数
	/// </summary>
	virtual void Close() = 0;

	/// <summary>
	/// 清理缓冲
	/// </summary>
	virtual void Clear() = 0;

	/// <summary>
	/// 写数据到Qt音频播放器（有空间才写）
	/// </summary>
	/// <param name="data">音频数据</param>
	/// <param name="datasize">音频数据大小</param>
	/// <returns>bool 成功与否</returns>
	virtual bool Write(const unsigned char* data, int datasize) = 0;
	
	/// <summary>
	/// 是否有空余空间写入音频数据
	/// </summary>
	/// <returns>剩余空间大小</returns>
	virtual int GetFree() = 0;

	/// <summary>
	/// 返回当前未播放的缓冲区的音频的毫秒
	/// </summary>
	/// <returns>long long</returns>
	virtual long long GetYetNotMS() = 0;
public:
	virtual ~XAudioPlay();
	XAudioPlay(const XAudioPlay& other) = delete;
	XAudioPlay& operator=(const XAudioPlay& other) = delete;
protected:
	/*播放器所需参数，可从Open函数接口传进*/
	
	int sampleRate_ = 44100;	//采样率
	
	int sampelSize_ = 16;	//位数(重采样统一变成S16，可能后续需要修改一下)
	
	int nb_channels_ = 2;	//通道个数
protected:
	XAudioPlay();
};

class QtAudioPlayer : public XAudioPlay {
public:
	QtAudioPlayer();
	virtual ~QtAudioPlayer();
public:
	virtual bool Open(int sample_rate,int nb_channles);
	virtual void SetPause(bool isPause);
	virtual void Close();
	virtual void Clear();
	virtual bool Write(const unsigned char* data, int datasize);
	virtual int GetFree();
	virtual long long GetYetNotMS();
public:
	QAudioOutput* outPut_ = nullptr;
	QIODevice* io_ = nullptr;
	std::mutex Gmtx_;
	bool is_Close = false;
};

