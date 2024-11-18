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
	/*原有事件处理*/
	QSlider::mousePressEvent(ev);// 用这个能够处理滑动和上面重载的点击，但是点击到后面部分没有反应，滑动没问题
	//QSlider::sliderReleased();	// 用这个只能处理点击事件，点击任何位置都行，但是不能处理滑动事件了
}
