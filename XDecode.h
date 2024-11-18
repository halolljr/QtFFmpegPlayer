#pragma once
#include <iostream>
#include <mutex>
extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
}
class XDecode
{
public:
	XDecode();
	virtual~XDecode();
public:
	/// <summary>
	/// 打开解码器并释放AVCodecParameters*的空间（不管成功与否）{每一次Open都会释放上一次的资源}
	/// </summary>
	/// <param name="para">参数AVCodecParameters*,不管成功与否都要释放AVCodecParameters空间</param>
	/// <returns>bool 解码器是否打开成功</returns>
	virtual bool Open(AVCodecParameters* para);

	/// <summary>
	/// 发送到解码线程，不管成功与否都释放AVPacket*空间
	/// </summary>
	/// <param name="pkt">av_read_frame()得到的包</param>
	/// <returns>bool 是否av_send_packet()成功</returns>
	virtual bool Send(AVPacket* pkt);

	/// <summary>
	/// 获取解码数据，一次Send可能需要多次Recv，获取缓冲中的数据Send NULL再Recv多次
	/// </summary>
	/// <returns>返回AVFrame*，内存需要外部释放</returns>
	virtual AVFrame* Recv();

	/// <summary>
	/// 返回AVCodecContext指针
	/// </summary>
	/// <returns>AVCodecContext*</returns>
	AVCodecContext* GetAVCodecContext() {
		return ctx_;
	}

	/// <summary>
	/// 清理解码缓冲=====这个还没有用到，不知道何时去调用
	/// </summary>
	virtual void Clear();

	/// <summary>
	/// 回收资源
	/// </summary>
	virtual void Close();
private:
	std::mutex Gmtx_;
	
	/*解码器*/
	const AVCodec* codec_ = nullptr;
	
	/*解码器上下文*/
	AVCodecContext* ctx_ = nullptr;

public:
	/*当前解码到的pts*/
	long long pts = 0;

	std::atomic<bool> isAudio{ false };
};

