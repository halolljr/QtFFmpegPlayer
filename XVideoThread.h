#pragma once
#include <iostream>
#include <mutex>
#include <list>
#include <QThread>
#include "XDecode.h"
#include "SDLRender.h"
/// <summary>
/// �������ʾ��Ƶ
/// </summary>
class XVideoThread:public QThread
{
public:
	XVideoThread();
	virtual ~XVideoThread();
public:
	void run();
	
	/// <summary>
	/// ��ʼ���ڲ�������
	/// </summary>
	/// <param name="para">XDemux��CopyVPara()</param>
	/// <returns>bool �ɹ����</returns>
	virtual bool Open(AVCodecParameters* para);
	
	/// <summary>
	/// β��AVPacket*�������ڲ�����
	/// </summary>
	/// <param name="pkt">AVPacket* ���ݰ�</param>
	/// <returns>bool �ɹ����</returns>
	virtual bool Push(AVPacket* pkt);
	
	virtual void Exit();
protected:

	/*�����������Ľṹ��*/
	XDecode* decodec_ = nullptr;

	/*��Ƶ��Ⱦ��*/
	SDLRender sdlRender_;

	/*AVPacket*������*/
	std::list<AVPacket*> pkt_list_;
	
	/*AVPacket*������������*/
	int MAX_LIST_SIZE = 100;
	
	std::atomic_bool is_Exit;
	std::mutex mtx_;
};

