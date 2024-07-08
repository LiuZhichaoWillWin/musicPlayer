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

bool SongList::addToMyLove(const QString& sourceFilePath, const QString& filename) {
    qDebug() << sourceFilePath << endl;
    QFile sourceFile(sourceFilePath);
    if (!sourceFile.exists()) {
            qDebug() << "Source file does not exist.";
            return false;
        }
    QDir destinationDir("D:\\MusicPlayer\\myLove");
    if (!destinationDir.exists()) {
        qDebug() << "Destination directory does not exist.";
        return false;
    }
     if (sourceFile.copy("D:\\MusicPlayer\\myLove\\" + filename)) {
                qDebug() << "File copied successfully to D:\\MusicPlayer\\myLove" << endl;
                qDebug() << sourceFile.errorString();
                return true;
            } else {
                qDebug() << "Failed to copy file to D:\\MusicPlayer\\myLove" << endl;
                return false;
            }
}

bool SongList::deleteFromMyLove(const QString& filename) {
    QDir destinationDir("D:\\MusicPlayer\\myLove");
    if (!destinationDir.exists()) {
        qDebug() << "Destination directory does not exist.";
        return true;
    }
    destinationDir.remove("D:\\MusicPlayer\\myLove\\" + filename);
    qDebug() << 'deleted!!!';
}
