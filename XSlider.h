#pragma once
#include <QObject>
#include <QSlider>
#include <QMouseEvent>
class XSlider:public QSlider
{
	Q_OBJECT
public:
	XSlider(QWidget* parent = nullptr);
	~XSlider();

	void mousePressEvent(QMouseEvent* ev);

};

