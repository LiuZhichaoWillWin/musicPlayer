#ifndef SONGLIST_H
#define SONGLIST_H
#pragma once

#include <QUrl>
#include <QList>
#include <vector>
#include <set>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QFile>
#include <QDir>
#include <QString>

class SongList: public QList<QUrl>,public QMediaPlaylist {
    friend class Control;
public:
    SongList();
    void addSong(QString filePath);
    void unique();
    QUrl& getItem(int index);
    QStringList returnPlayList();
    bool addToMyLove(const QString& sourceFilePath, const QString& filename);
    bool deleteFromMyLove(const QString& filename);
};

#endif // SONGLIST_H
