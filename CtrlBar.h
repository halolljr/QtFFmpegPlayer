﻿#pragma once

#include <QWidget>
#include "CustomSlider.h"
#include "ui_CtrlBar.h"

QT_BEGIN_NAMESPACE
namespace Ui { class CtrlBarClass; };
QT_END_NAMESPACE

class CtrlBar : public QWidget
{
	Q_OBJECT

public:
    explicit CtrlBar(QWidget* parent = 0);
    ~CtrlBar();
    /**
         * @brief	初始化UI
         *
         * @return	true 成功 false 失败
         * @note
         */
    bool Init();

public:
    void OnVideoTotalSeconds(int nSeconds);
    void OnVideoPlaySeconds(int nSeconds);
    void OnVideopVolume(double dPercent);
    void OnPauseStat(bool bPaused);
    void OnStopFinished();
    void OnSpeed(float speed);
private:
    void OnPlaySliderValueChanged();
    void OnVolumeSliderValueChanged();
private slots:
    void on_PlayOrPauseBtn_clicked();
    void on_VolumeBtn_clicked();
    void on_StopBtn_clicked();
    void on_SettingBtn_clicked();

    /**
    * @brief	连接信号槽
    *
    * @param
    * @return
    * @note
    */
    bool ConnectSignalSlots();
    void on_speedBtn_clicked();

signals:
    void SigShowOrHidePlaylist();	//< 显示或隐藏信号
    void SigPlaySeek(double dPercent); ///< 调整播放进度
    void SigPlayVolume(double dPercent);
    void SigPlayOrPause();
    void SigStop();
    void SigForwardPlay();
    void SigBackwardPlay();
    void SigShowMenu();
    void SigShowSetting();
    void SigSpeed();
private:
	Ui::CtrlBarClass *ui;
	int m_nTotalPlaySeconds;
	double m_dLastVolumePercent;
};