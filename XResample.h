#pragma once
#include <iostream>
#include <mutex>
extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswresample/swresample.h>
}
class XResample
{
public:
	XResample();
	virtual ~XResample();
public:
	
	/// <summary>
	/// 重采样器初始化，除了采样格式，输出为S16，其余保持一致
	/// </summary>
	/// <param name="para">AVCodecContext*</param>
	/// <returns>bool值，成功与否</returns>
	virtual bool Open(AVCodecParameters* para,bool isClearPara);

	/// <summary>
	/// 音频重采样,不管成功与否都释放AVFrame*
	/// </summary>
	/// <param name="indatas">重采样前的数据，AVFrame*</param>
	/// <param name="outdatas">重采样后的数据，unsigned char*</param>
	/// <returns> <0 if 失败;不然正常返回>0 </returns>
	virtual int Resample(AVFrame* indatas, unsigned char* outdatas);

	/// <summary>
	/// 关闭并释放重采样器资源
	/// </summary>
	virtual void Close();
private:
	/*互斥锁*/
	std::mutex Gmtx_;
	
	/*音频重采样器（不要写成SwsContext*）*/
	SwrContext* swsCtx_ = nullptr;
	
	/*重采样后的格式*/
	AVSampleFormat outFormat = AV_SAMPLE_FMT_S16;
	
};

