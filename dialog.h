#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QMediaPlayer>
#include <QDebug>
#include <QString>
#include <QSignalMapper>
#include <QApplication>
#include <QPushButton>
#include <QGridLayout>
#include <QDir>
#include <stdio.h>
#include <stdlib.h>
#include <QtMultimedia>
#include <QtWidgets>
#include <QKeyEvent>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

public slots:
    void on_sliderProgress_sliderMoved(int position);

    void on_sliderVolume_sliderMoved(int position);

    void on_pushButton_2_clicked();

    void verif(int state);

    void on_positionChanged(qint64 position);

    void on_durationChanged(qint64 position);

    void appui(const QString &text); //mon petit  slot à moi qui charge le chemin du fichier pour le lire

    void nomFichiers(); //méthode pour recuperer les nom des fichiers

private:
    QSignalMapper *signalMapper;
    Ui::Dialog *ui;
    QMediaPlayer* player;
    QMediaPlaylist * playlist;
    QPushButton *button;
    QLabel *label;
    QGridLayout *layout;
    QHBoxLayout *layout2;
    bool *etat;
    void keyPressEvent(QKeyEvent * event);
    QString strTouche2;
    int increment;
    int incrementBis;
};

#endif // DIALOG_H
