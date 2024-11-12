#pragma once
#include <iostream>
#include <mutex>
#include <thread>
extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <SDL2/SDL.h>
}
class SDLRender
{
public:
	/// <summary>
	/// 内部初始化SDL_Window和SDL_Render，为AVFrame*分配空间
	/// </summary>
	SDLRender();
	~SDLRender();
	SDLRender(const SDLRender& other) = delete;
	SDLRender& operator=(const SDLRender& other) = delete;
public:
	
	/// <summary>
	/// 内部会将视频解码，如果是第一次调用还会创建SDL_Texture；同时内部还会释放AVPacket*；内部while循环不断渲染
	/// </summary>
	/// <param name="v_ctx">解码器上下文 AVCodecContext*</param>
	/// <param name="pkt">视频包 AVPacket*</param>
	/// <returns>成功与否</returns>
	bool video_decodec(AVCodecContext* v_ctx, AVPacket* pkt);
	
	/// <summary>
	/// 内部循环去调用render函数
	/// </summary>
	/*void renderVideoLoop();*/

	/// <summary>
	/// 内部视频渲染
	/// </summary>
	void render();

	/// <summary>
	/// 关闭资源
	/// </summary>
	void Close();

	/// <summary>
	/// 从FFmpeg视频格式转为SDL支持的播放格式
	/// </summary>
	/// <param name="pixformat">AVPixelFormat&</param>
	/// <returns>-1 if 失败</returns>
	Uint32 ffmpeg_pix_fmt_to_sdl_pix_fmt(const AVPixelFormat& pixformat);
private:
	SDL_Window* window_ = nullptr;
	SDL_Renderer* renderer_ = nullptr;
	SDL_Texture* texture_ = nullptr;
	
	/*视频渲染线程*/
	/*std::thread* render_thread_ = nullptr;*/

	/*循环利用的AVFrame*/
	AVFrame* frame_ = nullptr;
	
	/*是否初次创建*/
	bool is_First_C = true;
	
	/*显示的视频材质格式（需要从FFmpeg转化为SDL支持的）*/
	Uint32 pixformat_ = 0;
	
	/*视频yuv数据的宽高*/
	int VIDEO_WIDTH_ = 0;
	int VIDEO_HEIGHT_ = 0;
	
	/*通用锁*/
	std::mutex mtx_;
	
	/*视频帧锁*/
	/*std::mutex frame_mutex_;*/

	/*条件变量*/
	/*std::condition_variable frame_cv_;*/

	bool is_Close = false;
public:
	
	/*显示器的窗口大小===这里默认与外部Qt窗口宽高一致*/
	int WINDOW_WIDTH = 640;
	int WINDOW_HEIGHT = 480;
	
	/*显示器窗口标题*/
	std::string WINDOW_TITLE = "PLAYER";
};

