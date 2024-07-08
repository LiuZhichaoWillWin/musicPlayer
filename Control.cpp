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

    ui->volumeSlider->setValue(100);
    connect(mediaPlayer, &QMediaPlayer::stateChanged, this, &Control::changePlayMode);

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
    index = ui->listWidget->currentRow();
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

    QDir myLove("D:\\MusicPlayer\\myLove");
    QStringList myLoveSongs = myLove.entryList();
    isMyLove = false;
    for (auto fileName : myLoveSongs) {
        if (songNameList[index].toString() == fileName) {
            isMyLove = true;
            break;
        }
    }
    qDebug() << isMyLove;
}


void Control::setPauseImage() {
    ui->play->setStyleSheet(" background-image: url(:/pic2/8.png); ");
}

void Control::setPlayImage() {
    ui->play->setStyleSheet(" background-image: url(:/pic2/4.png); ");
}


void Control::on_pre_clicked(bool checked)
{
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
    oldVolume = ui->volumeSlider->value();
    ui->volumeSlider->setValue(0);
//    qDebug() <<  "设置了静音后的音量" << mediaPlayer->volume();
    ui->volume->setStyleSheet("  background-image: url(:/pic2/9.png);");
    ui->volumeSlider->hide();
}

void Control::setVolume2image()
{
    ui->volume->setStyleSheet("  background-image: url(:/pic2/5.png);");
    ui->volumeSlider->show();
    ui->volumeSlider->setValue(oldVolume);
//    qDebug() << ui->volumeSlider->value() << " " << oldVolume;
}

std::vector<size_t> Control::returnUnplayedSongsIndexs() {
    if (randomlyPlayedSongs.size() == playList.length())
        randomlyPlayedSongs.clear();
    std::vector<size_t> unplayedSongsIndexs;
    for (size_t i = 0; i < playList.length(); i++) {
        int isRandomlyPlayed = false;
        for (int j = 0; j < randomlyPlayedSongs.size(); j++) {
            if (i == randomlyPlayedSongs[j]) {
                isRandomlyPlayed = true;
                continue;
            }
        }
        if (!isRandomlyPlayed)
            unplayedSongsIndexs.push_back(i);
    }
    return unplayedSongsIndexs;
}

void Control::changePlayMode()
{
    if (mediaPlayer->media().isNull()) return;
    if (playMode == 0) {
        qDebug() << "顺序" << endl;
        ui->playMode->setStyleSheet(" background-image: url(:/pic2/7.png); ");
        if (mediaPlayer->state() == QMediaPlayer::StoppedState && mediaPlayer->position() == mediaPlayer->duration()) {
            index = ++index % playList.length();
            ui->listWidget->setCurrentRow(index);
            newSong();
        }
    }
    else if (playMode == 1) {
        qDebug() << "随机" << endl;
        ui->playMode->setStyleSheet(" background-image: url(:/pic2/6.png); ");
        std::mt19937 gen(rd()); // 以随机设备作为种子的Mersenne Twister生成器

        if (mediaPlayer->state() == QMediaPlayer::StoppedState && mediaPlayer->position() == mediaPlayer->duration()) {
            // 定义随机数的分布范围
            randomlyPlayedSongs.push_back(index);
            std::vector<size_t> randomlyUnplayedSongsIndexs = returnUnplayedSongsIndexs();
            std::uniform_real_distribution<> distr(0, randomlyUnplayedSongsIndexs.size() - 1);
            int random_number = randomlyUnplayedSongsIndexs[distr(gen)];
            ui->listWidget->setCurrentRow(random_number);
            newSong();
        }
    }
    else {
        qDebug() << "循环" << endl;
        ui->playMode->setStyleSheet(" background-image: url(:/pic2/12.png) ");
        if (playMode == 2) {
            if (mediaPlayer->state() == QMediaPlayer::StoppedState && mediaPlayer->position() == mediaPlayer->duration()) {
                newSong();
            }
        }
    }
}

void Control::on_playMode_clicked()
{
    qDebug() << "mode";
    playMode = ++playMode % 3;
    changePlayMode();
}


void Control::on_loveSong_clicked(bool checked)
{
    if (isMyLove) {
        playList.deleteFromMyLove(songNameList[index].toString());
        isMyLove = false;
    }
    else {
        playList.addToMyLove(playList[index].toString(), songNameList[index].toString());
        isMyLove = true;
    }
}

