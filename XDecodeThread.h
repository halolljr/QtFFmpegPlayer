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
	/// ������в������ݣ�ǳ������
	/// </summary>
	/// <param name="pkt">AVPacke* ��</param>
	virtual void Push(AVPacket* pkt);

	/// <summary>
	/// �������
	/// </summary>
	virtual void Clear();

	/// <summary>
	/// ������Դ��ֹͣ�߳�
	/// </summary>
	virtual void Close();

	/// <summary>
	/// ȡ��һ֡���ݣ�����ջ�����û���򷵻�NULL
	/// </summary>
	/// <returns>ȡ�������ݣ�û���򷵻�NULL</returns>
	virtual AVPacket* Pop();

protected:
	std::list<AVPacket*> list_pkt_;
	int MAX_LIST_SIZE = 100;
	bool isExit = false;
	std::mutex mtx_;
	XDecode* decode = nullptr;
};

