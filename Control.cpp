#include "Control.h"
#include "ui_Control.h"

Control::Control(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Control)
    , playList()
{
    ui->setupUi(this);
    mediaPlayer = new QMediaPlayer(this);
    index = 0;

    timer = new QTimer(this);
    timer->start(10);
    connect(timer, &QTimer::timeout, this, &Control::on_timer_timeout);

    ui->volumeSlider->setValue(mediaPlayer->volume());

    setPauseImage();
}

Control::~Control()
{
    delete ui;
    delete timer;
}

void Control::on_musicList_clicked(bool checked)
{
     //打开对话框让用户选择音乐所在目录
        auto path = QFileDialog::getExistingDirectory(this, "选择歌曲所在目录","D:\\柳志超\\音乐MP3");
     //根据这个路径，获取里面所有的mp3文件
        qInfo() << path <<endl;
        QDir dir(path);
        auto musicList = dir.entryList(QStringList()<<"*.mp3");

        if (musicList.length() == 0) return;
     //把音乐路径保存下来
        for(auto file :musicList) {
            songNameList.addSong(file);
            playList.addSong(path + "/" + file);
        }

        playList.unique();
        songNameList.unique();
        qInfo() << playList << endl;
        qInfo() << songNameList;
        //把音乐放在listweidget中展示
        ui->listWidget->clear();
        ui->listWidget->addItems(songNameList.returnPlayList());
        //默认选中第一个音乐
        ui->listWidget->setCurrentRow(0);

        newSong();

        mediaPlayer->pause();
        setPauseImage();
}

void Control::newSong() {
    if (mediaPlayer != nullptr) {
        delete mediaPlayer;
    }
    index = ui->listWidget->currentRow();
    mediaPlayer = new QMediaPlayer();
    mediaPlayer->setMedia(playList[index]);

    // 总时长
    connect(mediaPlayer, &QMediaPlayer::durationChanged, this, [=](qint64 duration){
        ui->horizontalSlider->setRange(0, mediaPlayer->duration() / 1000);
        ui->horizontalSlider->setMaximum(mediaPlayer->duration() / 1000);
        ui->horizontalSlider->setValue(0);

        int minutes = duration / 1000 / 60;
        int seconds = duration / 1000 % 60;
        QString *totalTime = nullptr;
        if (seconds >= 10) {
            totalTime = new QString(QString::number(minutes) + ":" + QString::number(seconds));
        }
        else {
            totalTime = new QString(QString::number(minutes) + ":" + "0" + QString::number(seconds));
        }
        ui->duration->setText(*totalTime);
    });

    mediaPlayer->play();
    setPlayImage();
}


void Control::setPauseImage() {
    ui->play->setStyleSheet(" background-image: url(:/pic2/8.png); ");
}

void Control::setPlayImage() {
    ui->play->setStyleSheet(" background-image: url(:/pic2/4.png); ");
}


void Control::on_pre_clicked(bool checked)
{
    if (mediaPlayer->media().isNull()) return;
    if (index == 0) index = playList.length();
    index = --index % (playList.length());
    ui->listWidget->setCurrentRow(index);

    newSong();
}

void Control::on_timer_timeout() {
    qint64 position = mediaPlayer->position();
    int minutes = position / 1000 / 60;
    int seconds = position / 1000 % 60;
    QString *totalTime = nullptr;
    if (seconds >= 10) {
        totalTime = new QString(QString::number(minutes) + ":" + QString::number(seconds));
    }
    else {
        totalTime = new QString(QString::number(minutes) + ":" + "0" + QString::number(seconds));
    }
    ui->position->setText(*totalTime);

    ui->horizontalSlider->setValue(position / 1000);
}

void Control::on_play_clicked(bool checked)
{
    if (mediaPlayer->media().isNull()) return;
    switch(mediaPlayer->state())
    {
       case QMediaPlayer::StoppedState:{
           newSong();
           break;
       }
       case QMediaPlayer::PlayingState:{
            if (index != ui->listWidget->currentRow()) {
                newSong();
                return;
            }
            mediaPlayer->pause();
            setPauseImage();
            break;
       }
       case QMediaPlayer::PausedState:{
            if (index != ui->listWidget->currentRow()) {
                newSong();
                return;
            }
            mediaPlayer->play();
            setPlayImage();
            break;
       }
    }

}


void Control::on_next_clicked(bool checked)
{
    // qInfo() << playList.length();
    if (mediaPlayer->media().isNull()) return;
    index = ++index % (playList.length());
    ui->listWidget->setCurrentRow(index);
    newSong();
}


void Control::on_volume_clicked(bool checked)
{
    if(mediaPlayer->volume())
            setVolume1image();
    else
            setVolume2image();
}

void Control::on_volumeSlider_valueChanged(int value)
{
    mediaPlayer->setVolume(ui->volumeSlider->value());
}


void Control::on_horizontalSlider_valueChanged(int value)
{
    qint64 progress = ui->horizontalSlider->value() * 1000;
    if (qAbs(progress - mediaPlayer->position()) > 100)
        mediaPlayer->setPosition(ui->horizontalSlider->value() * 1000);
}

void Control::setVolume1image()
{
        mediaPlayer->setVolume(0);
        ui->volume->setStyleSheet("  background-image: url(:/pic2/9.png);");
        ui->volumeSlider->hide();
}

void Control::setVolume2image()
{
        ui->volume->setStyleSheet("  background-image: url(:/pic2/5.png);");
        ui->volumeSlider->show();
        mediaPlayer->setVolume(50);
}
