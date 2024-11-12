#include "QtFFmpegPlayer.h"
//#include "XDecode.h"
//#include "XDemux.h"
//#include "XResample.h"
//#include "XVideoWidget.h"
//#include "XAudioPlay.h"
//#include "SDLRender.h"
//#include "XAudioThread.h"
//#include "XVideoThread.h"
//#include <QThread>
#include <windows.h>
#include <QtWidgets/QApplication>
#include "XDemuexThread.h"
//class TestThread :public QThread
//{
//public:
//	TestThread() {}
//	~TestThread() {}
//	void init() {
//		const char* url = "test2.mp4";
//		xDemux.Open(url);
//		///*打开解码器*/
//		//if (x_vDecode.Open(xDemux.CopyVPara())) {
//		//	std::cout << "[Video]" << std::endl;
//		//}
//		//if (x_aDecode.Open(xDemux.CopyAPara())) {
//		//	std::cout << "[Audio]" << std::endl;
//		//}
//		///*打开重采样器*/
//		//if (xResample.Open(x_aDecode.GetAVCodecContext())) {
//		//	std::cout << "[RESAMPLE]" << std::endl;
//		//}
//		///*打开音频播放器*/
//		//if (xAudioPlay.Open(xDemux.sample_rate_, xDemux.ch_layout_.nb_channels)) {
//		//	std::cout << "[AUDIOPLAYER]" << std::endl;
//		//}
//		if (audioThread.Open(xDemux.CopyAPara())) {
//			std::cout << "[AUDIOTHREAD]" << std::endl;
//			audioThread.start();
//		}
//		if (videoThread.Open(xDemux.CopyVPara())) {
//			std::cout << "[VIDEOTHREAD]" << std::endl;
//			videoThread.start();
//		}
//	}
//	void run() {
//		/*解码*/
//		while (!is_Exit) {
//			AVPacket* pkt = xDemux.Read();
//			if (!pkt)
//				break;
//			if (xDemux.IsAudio(pkt)) {
//				/*if (!x_aDecode.Send(pkt)) {*/
//				//	continue;
//				//}
//				//AVFrame* frame = x_aDecode.Recv();
//				//if (!frame) {
//				//	continue;
//				//}
//				///*pcm存在内存泄漏问题*/
//				//int len = xResample.Resample(frame, pcm);
//				//if (len	< 0 ){
//				//	continue;
//				//}
//				//while (xAudioPlay.GetFree() >= len) {
//				//	xAudioPlay.Write(pcm, len);
//				//	//msleep(1);
//				//}
//				audioThread.Push(pkt);
//			}
//			else {
//				/*if (!sdlRender.video_decodec(x_vDecode.GetAVCodecContext(), pkt)) {
//					continue;
//				}*/
//				/*x_vDecode.Send(pkt);
//				AVFrame* frame = x_vDecode.Recv();
//				videoWidget->Repaint(frame);
//				if (frame)
//					av_frame_free(&frame);
//				*/
//				//msleep(1);
//				videoThread.Push(pkt);
//			}
//		}
//		//对于视频而言,将AVPacket置为NULL，清空AVFrame缓冲区
//		/*sdlRender.video_decodec(x_vDecode.GetAVCodecContext(), nullptr);*/
//	}
//	void Exit() {
//		audioThread.Exit();
//		videoThread.Exit();
//		is_Exit = true;
//	}
//public:
//	XDemux xDemux;
//	XAudioThread audioThread;
//	XVideoThread videoThread;
//	bool is_Exit = false;
//	//XDecode x_vDecode;
//	//XDecode x_aDecode;
//	//XResample xResample;
//	//SDLRender sdlRender;
//	//XAudioPlay& xAudioPlay = XAudioPlay::instance();
//	//unsigned char* pcm = new unsigned char[1024 * 1024];
//	//XVideoWidget* videoWidget = nullptr;
//};
int main(int argc, char *argv[])
{
	// 设置控制台输出编码为 UTF-8
	//SetConsoleOutputCP(CP_UTF8);
	/*TestThread tt;
	tt.init();*/

    QApplication a(argc, argv);
    QtFFmpegPlayer w;
    w.show();
	/*tt.start();*/
	XDemuexThread demuxThread;
	if (demuxThread.Open("test2.mp4")) {
		demuxThread.Start();
	}
    return a.exec();
}

