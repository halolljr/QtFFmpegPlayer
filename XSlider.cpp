#include "XSlider.h"

XSlider::XSlider(QWidget* parent /*= nullptr*/):QSlider(parent)
{

}

XSlider::~XSlider()
{

}

void XSlider::mousePressEvent(QMouseEvent* ev)
{
	double pos = (double)ev->pos().x() / (double)width();
	setValue(pos * this->maximum());
	/*ԭ���¼�����*/
	QSlider::mousePressEvent(ev);// ������ܹ����������������صĵ�������ǵ�������沿��û�з�Ӧ������û����
	//QSlider::sliderReleased();	// �����ֻ�ܴ������¼�������κ�λ�ö��У����ǲ��ܴ������¼���
}
