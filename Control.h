#ifndef CONTROL_H
#define CONTROL_H
#pragma once

#include <QWidget>
#include<QUrl>
#include<QDebug>
#include<QFileDialog>
#include<QDir>
#include<QMediaPlayer>
#include<QAudioOutput>
#include<QTimer>
#include <QString>
#include<QtMath>
#include "SongList.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Control; }
QT_END_NAMESPACE

class Control : public QWidget
{
    Q_OBJECT

public:
    Control(QWidget *parent = nullptr);
    ~Control();

private slots:

    void on_musicList_clicked(bool checked);

    void on_pre_clicked(bool checked);

    void on_play_clicked(bool checked);

    void on_next_clicked(bool checked);

    void on_volume_clicked(bool checked);

    void on_volumeSlider_valueChanged(int value);

    void on_timer_timeout();

    void on_horizontalSlider_valueChanged(int value);

private:
    Ui::Control *ui;
    SongList playList;
    QMediaPlayer *mediaPlayer;
    int index;
    QTimer *timer;
    SongList songNameList;

    void newSong();
    void newTimer();
    void setPauseImage();
    void setPlayImage();
    void noPlayeList();
    void setVolume1image();
    void setVolume2image();

};
#endif // CONTROL_H
