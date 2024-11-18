#pragma once
#include <iostream>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QGLShaderProgram>
#include <mutex>
#include "IVideoCall.h"
extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#pragma comment (lib, "avcodec.lib")
#pragma comment (lib, "avdevice.lib")
#pragma comment (lib, "avfilter.lib")
#pragma comment (lib, "avformat.lib")
//#pragma comment (lib, "avutil.lib")
//#pragma comment (lib, "swresample.lib")
//#pragma comment (lib, "swscale.lib")
}
class XVideoWidget  : public QOpenGLWidget, protected  QOpenGLFunctions, public IVideoCall
{
	Q_OBJECT

public:
	XVideoWidget(QWidget *parent);
	~XVideoWidget();

	/// <summary>
	/// 初始化窗口宽高
	/// </summary>
	/// <param name="width">yuv数据的宽</param>
	/// <param name="height">yuv数据的高</param>
	virtual void Init(int width, int height);

	/// <summary>
	/// 接受解码后的yuv数据，并释放传入的AVFrame*
	/// </summary>
	/// <param name="frame">解码后的frame</param>
	virtual void Repaint(AVFrame* frame);
protected:

	/// <summary>
	/// 刷新显示
	/// </summary>
	void paintGL();

	/// <summary>
	/// 初始化GL
	/// </summary>
	void initializeGL();

	/// <summary>
	/// 窗口尺寸变换
	/// </summary>
	/// <param name="width">宽</param>
	/// <param name="height">高</param>
	void resizeGL(int width,int height);
private:
	std::mutex Gmtx_;
	/*shader程序*/
	QGLShaderProgram program;
	/*shader中的yuv变量地址*/
	GLuint unis[3] = { 0 };
	/*openGL中的texture地址*/
	GLuint texs[3] = { 0 };
	/*材质空间地址*/
	unsigned char* datas[3] = { 0 };
	/*yuv420P数据的宽高*/
	int width_ = 0;
	int height_ = 0;
	/*FILE* fp;*/
};
