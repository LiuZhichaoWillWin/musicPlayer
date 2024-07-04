#include "SongList.h"

void SongList::addSong(QString filePath) {
    QUrl url(QUrl::fromLocalFile(filePath));
    this->append(filePath);
}

SongList::SongList(): QList<QUrl>() {}

QUrl& SongList::getItem(int index) {
    return this->operator[](index);
}

void SongList::unique() {;
    int len = this->length();
    for (int i = 0; i < len; i++) {
        for (int j = i + 1; j < len; j++) {
            if (this->getItem(i) == this->getItem(j)) {
                this->getItem(j) = "0";
            }
        }
    }
    this->removeAll(QUrl("0"));
}

QStringList SongList::returnPlayList() {
    QStringList list;
    for (auto& fileNameUrl : *this) {
        list.append(fileNameUrl.toString().replace(".mp3", ""));
    }
    return list;
}


