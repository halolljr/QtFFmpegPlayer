#include "XResample.h"

#pragma comment (lib, "avcodec.lib")
//#pragma comment (lib, "avdevice.lib")
//#pragma comment (lib, "avfilter.lib")
#pragma comment (lib, "avformat.lib")
//#pragma comment (lib, "avutil.lib")
#pragma comment (lib, "swresample.lib")
//#pragma comment (lib, "swscale.lib")
XResample::XResample()
{

}

XResample::~XResample()
{

}

bool XResample::Open(AVCodecParameters* para,bool isClearPara)
{
	if (!para) {
		return false;
	}
	Close();
	/*上锁*/
	std::lock_guard<std::mutex> lck(Gmtx_);
	if (!swsCtx_) {
		AVChannelLayout in_ch_layout, out_ch_layout;
		av_channel_layout_copy(&in_ch_layout, &para->ch_layout);
		av_channel_layout_copy(&out_ch_layout, &para->ch_layout);
		/*创建SwsContext*/
		int ret = swr_alloc_set_opts2(&swsCtx_,	/*采样器*/
			&out_ch_layout,	/*输出的声道布局*/
			outFormat, /*转换后的采样格式===默认AV_SAMPLE_FMT_S16*/
			para->sample_rate,	/*转换后的采样率*/
			&in_ch_layout,	/*输入的声道布局*/
			(AVSampleFormat)para->format, /*输入的采样格式=====可能有问题*/
			para->sample_rate, /*输入的采样率*/
			0, 0);
		if (isClearPara) {
			avcodec_parameters_free(&para);
		}
		if (ret < 0) {  // 错误检查，确保返回的错误为负数（FFmpeg 的错误码）
			char errbuff[1024];
			av_strerror(ret, errbuff, sizeof(errbuff));
			av_log(nullptr, AV_LOG_ERROR, "%s : %s : %s\n", __FUNCTION__, "swr_alloc_set_opts2() Failed", errbuff);
			return false;
		}
		ret = swr_init(swsCtx_);
		if (ret != 0) {
			char errbuff[1024];
			av_strerror(ret, errbuff, sizeof(errbuff));
			av_log(nullptr, AV_LOG_ERROR, "%s : %s :%s\n", __FUNCTION__, "swr_init() Failed",errbuff);
			return false;
		}
	}
	return true;
}

int XResample::Resample(AVFrame* indatas, unsigned char* outdatas)
{
	if (!indatas) {
		return 0;
	}
	if (!outdatas) {
		av_frame_free(&indatas);
		return 0;
	}
	/*指针数组*/
	uint8_t* data[2] = { 0 };
	data[0] = outdatas;
	int ret = swr_convert(swsCtx_,
		data,indatas->nb_samples ,	/*输出==*/
		(const uint8_t**)indatas->data,indatas->nb_samples	/*输入*/
		);
	if (ret < 0) {
		av_log(nullptr, AV_LOG_ERROR, "%s : %s\n", __FUNCTION__, "Failed to resample audio");
		av_frame_free(&indatas);
		return ret;
	}
	int size = ret * indatas->ch_layout.nb_channels * av_get_bytes_per_sample(outFormat);
	av_frame_free(&indatas);
	return size;
}

void XResample::Close()
{
	std::lock_guard<std::mutex> lck(Gmtx_);
	if (swsCtx_) {
		swr_free(&swsCtx_);
		swsCtx_ = nullptr;
	}
	return;
}
