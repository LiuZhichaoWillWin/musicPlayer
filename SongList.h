#ifndef SONGLIST_H
#define SONGLIST_H
#pragma once

#include <QUrl>
#include <QList>
#include <vector>
#include <set>

class SongList: public QList<QUrl> {
    friend class Control;
private:

public:
    SongList();
    void addSong(QString filePath);
    void unique();
    QUrl& getItem(int index);
    QStringList returnPlayList();
    bool addToMyLove();
};
#endif // SONGLIST_H
