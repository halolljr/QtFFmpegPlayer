#include "SDLRender.h"
#pragma comment (lib, "avcodec.lib")
#pragma comment (lib, "avdevice.lib")
#pragma comment (lib, "avfilter.lib")
#pragma comment (lib, "avformat.lib")
#pragma comment (lib, "avutil.lib")
#pragma comment (lib, "swresample.lib")
#pragma comment (lib, "swscale.lib")
#pragma comment (lib, "SDL2.lib")
#pragma comment (lib, "SDL2main.lib")
#pragma comment (lib, "SDL2test.lib")

SDLRender::SDLRender()
{
	//WINDOW_WIDTH = window_width;
	//WINDOW_HEIGHT = window_height;
	//初始化SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_AUDIO)) {
		std::cerr << "Could not initialize SDL - " << std::string(SDL_GetError()) << std::endl;
	}
	window_ = SDL_CreateWindow(WINDOW_TITLE.c_str(),
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	if (!window_) {
		std::cerr << __FUNCTION__ << " : Failed to create a window..." << std::endl;
	}
	renderer_ = SDL_CreateRenderer(window_, -1, 0);
	if (!renderer_) {
		std::cerr << __FUNCTION__ << " : Failed to create a SDL_Render..." << std::endl;
	}
	frame_ = av_frame_alloc();
	if (!frame_) {
		std::cerr << __FUNCTION__ << " : AVFrame is NULL..." << std::endl;
	}
	/*render_thread_ = new std::thread(&SDLRender::renderVideoLoop, this);
	if (!render_thread_) {
		std::cerr << __FUNCTION__ << " : Thread is NULL..." << std::endl;
	}*/
}

SDLRender::~SDLRender()
{
	if (!is_Close)
		Close();
}


bool SDLRender::video_decodec(AVCodecContext* v_ctx, AVPacket* pkt)
{
	std::lock_guard<std::mutex> lck(mtx_);
	int res_2 = 0;
	res_2 = avcodec_send_packet(v_ctx, pkt);
	if (res_2 < 0) {
		av_packet_free(&pkt);
		throw std::runtime_error("Failed to send packet to decoder.");
	}
	while (res_2 >= 0) {
		/*frame_mutex_.lock();*/
		res_2 = avcodec_receive_frame(v_ctx, frame_);
		if (res_2 == AVERROR(EAGAIN) || res_2 == AVERROR_EOF) {
			av_packet_free(&pkt);
			av_frame_unref(frame_);
			/*frame_mutex_.unlock();*/
			return true;//继续读取数据，不够一帧
		}
		else if (res_2 < 0) {
			av_packet_free(&pkt);
			av_frame_unref(frame_);
			/*frame_mutex_.unlock();*/
			return  false;
		}
		//创建纹理
		if (is_First_C && frame_ != nullptr) {
			//要进行FFmpeg解码器上下文的像素格式到SDL的像素格式的映射
			pixformat_ = ffmpeg_pix_fmt_to_sdl_pix_fmt(v_ctx->pix_fmt);
			if (pixformat_ == -1) {
				/*av_packet_free(&pkt);*/
				std::cout << __FUNCTION__ << " : Unexpected pixefl format" << std::endl;
				av_packet_free(&pkt);
				av_frame_unref(frame_);
				/*frame_mutex_.unlock();*/
				return false;
			}
			texture_ = SDL_CreateTexture(renderer_, pixformat_, SDL_TEXTUREACCESS_STREAMING, v_ctx->width, v_ctx->height);
			if (!texture_) {
				std::cout << __FUNCTION__ << " : SDL_Texture is NULL..." << std::endl;
				av_packet_free(&pkt);
				av_frame_unref(frame_);
				/*frame_mutex_.unlock();*/
				return false;
			}
			is_First_C = false;
			VIDEO_WIDTH_ = v_ctx->width;
			VIDEO_HEIGHT_ = v_ctx->height;
		}
		render();
		/*frame_cv_.notify_all();
		frame_mutex_.unlock();*/
	}
	av_packet_free(&pkt);
	av_frame_unref(frame_);
	return true;
}

//void SDLRender::renderVideoLoop()
//{
//	while (!is_Close) {
//		// 等待解码线程发送新帧
//		std::unique_lock<std::mutex> lock(frame_mutex_);
//		frame_cv_.wait(lock);
//		render();
//	}
//}

void SDLRender::render()
{
	SDL_UpdateYUVTexture(texture_, nullptr,
		frame_->data[0], frame_->linesize[0],
		frame_->data[1], frame_->linesize[1],
		frame_->data[2], frame_->linesize[2]);
	SDL_RenderClear(renderer_);
	SDL_RenderCopy(renderer_, texture_, NULL, NULL);
	SDL_RenderPresent(renderer_);
	return;
}

void SDLRender::Close()
{
	if (window_) {
		SDL_DestroyWindow(window_);
		window_ = nullptr;
	}
	if (renderer_) {
		SDL_DestroyRenderer(renderer_);
		renderer_ = nullptr;
	}
	if (texture_) {
		SDL_DestroyTexture(texture_);
		texture_ = nullptr;
	}
	if (frame_) {
		av_frame_free(&frame_);
	}
	is_First_C = true;
	is_Close = true;
	//// 通知渲染线程退出
	//frame_cv_.notify_all();

	//// 等待渲染线程退出
	//if (render_thread_ && render_thread_->joinable()) {
	//	render_thread_->join();
	//}
	VIDEO_WIDTH_ = 0;
	VIDEO_HEIGHT_ = 0;
}

Uint32 SDLRender::ffmpeg_pix_fmt_to_sdl_pix_fmt(const AVPixelFormat& pixformat)
{
	switch (pixformat)
	{
	case AV_PIX_FMT_YUV420P:
		return SDL_PIXELFORMAT_IYUV;

	case AV_PIX_FMT_YUV422P:
		return SDL_PIXELFORMAT_YV12;

	case AV_PIX_FMT_RGB24:
		return SDL_PIXELFORMAT_RGB24;

	case AV_PIX_FMT_RGBA:
		return SDL_PIXELFORMAT_RGBA32;

	default:
		break;

	}
	return Uint32(-1);
}
