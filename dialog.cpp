#include "dialog.h"
#include "ui_dialog.h"
#include<QScrollArea>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    //Parametrage de la GUI via Dialog.ui
    ui->setupUi(this);
    ui->pushButton_2->setStyleSheet("QPushButton { background-color: red}");
    //ui->sliderVolume->setStyleSheet("QSlider::chunk {color : red}");
    ui->label_3->setStyleSheet("QLabel {color : white}");

    //compteur pour les playlistes des boutons shift et Capslock
    increment = 0;
    incrementBis = 0;
    //initialisation de l'attribue player
    player = new QMediaPlayer(this);
    playlist = new QMediaPlaylist(this);
    etat = new bool(false);

    // connection des curseurs progress et volume au player de musique
    connect(player, &QMediaPlayer::positionChanged, this, &Dialog::on_positionChanged);
    connect(player, &QMediaPlayer::durationChanged, this, &Dialog::on_durationChanged);
    //connection checkbox repeat
    connect(ui->checkBox,SIGNAL(stateChanged(int)),this,SLOT(verif(int)));

    // recuperation répertroire courant
    //QString CurrentDir = QDir::currentPath() + "/RessourcesSoundPad";
    QString CurrentDir = QCoreApplication::applicationDirPath() + "/RessourcesSoundPad";
    QDir repertoire(CurrentDir); // chemin de type QDir


    int nombreColonne(0), numeroLigne(1); // initialisation des compteurs de colonne et de ligne ainsi numero ligne qui represente le debut des boutons
    int tailleLayout(0), tailleLayout2(12);
    //Création de la scrollarea qui englobera layout
    QScrollArea *scrollArea = new QScrollArea;
    QScrollArea *scrollArea2 = new QScrollArea;

    //scrollArea2->setFixedHeight(50);

    // effacer les scrollbars
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea2->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea2->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //lier les scrolls de deux layouts
    connect(scrollArea->horizontalScrollBar(),SIGNAL(valueChanged(int)),scrollArea2->horizontalScrollBar(),SLOT(setValue(int)));

    // Création des layouts
     QVBoxLayout *layoutPrincipal = new QVBoxLayout(); // layout (Titres dossiers + boutons) + scroll
     QVBoxLayout *layoutPrincipal2 = new QVBoxLayout();
     layoutPrincipal->addWidget(scrollArea);
     layoutPrincipal2->addWidget(scrollArea2);

     // Cree le widget de scroll qui va contenir le layout...
     QWidget *scrolledWidget = new QWidget(scrollArea);
     QWidget *scrolledWidget2 = new QWidget(scrollArea2);

     //scrolledWidget2->setFixedHeight(50); VERIFER SI IMPACT

    //  QHBoxLayout *layouthaut = new QHBoxLayout; // titres dossiers
     layout = new QGridLayout(scrolledWidget); // creation de notre layout inclu dans le widget de scroll
     layout2 = new QHBoxLayout(scrolledWidget2);

    // creation du signal mapper
     signalMapper = new QSignalMapper(this);

     int taillePoliceTitre = 15, taillePoliceBouton = 12;
     scrollArea2->setFixedHeight(taillePoliceTitre + 17);


     // traitement des dossiers
     QFileInfoList files = repertoire.entryInfoList(); //Liste des fichiers dans le répertoire courant
     foreach (QFileInfo file, files)
    {
         // dossier police
        if (file.baseName().contains("police") &&file.baseName().contains("_"))
        {
            //récupération nom fichier sans chiffres du début
            QStringList list = file.baseName().split("_");

            //Mots apres le underscore
            QString taillePoliceTString = list.at(1);
            QString taillePoliceBString = list.at(2);
            taillePoliceTitre = taillePoliceTString.toInt();
            taillePoliceBouton = taillePoliceBString.toInt();
            scrollArea2->setFixedHeight(taillePoliceTitre + 17);
        }

        else // pour les autres dossiers
        {

        // on selectionne les dossiers en excluant les repertroires anterieurs, le dossier clavier et les repeertoires des plugins
        if (file.isDir() && file.fileName()!="." && file.fileName()!=".." && file.fileName()!="Clavier" && file.fileName()!="mediaservice" && file.fileName()!="platforms" && file.fileName()!="playlistformats")
            {
                QString repDossier = file.filePath();
                QDir dossier(repDossier);
                QFileInfoList fichierDuDossier = dossier.entryInfoList(); //Liste des fichiers dans le répertoire

                QString str1 = file.baseName();
                QString str2 = "white"; // couleur titre par default

                if(str1.contains("-c-")) //gestion des couleurs
                {
                    QStringList listCoul = str1.split("-c-");
                    str2 = listCoul.at(1); // nom de la couleur
                    str1 = listCoul.at(0); // titre
                }

                if (str1.contains("--"))
                {
                    //récupération nom fichier sans chiffres du début
                    QStringList listNom = str1.split("--");

                    //Mot apres le underscore
                    str1 = listNom.at(1);
                }

                label = new QLabel(str1);

                QFont font1 = label->font();
                font1.setPointSize(taillePoliceTitre);
                label->setFixedHeight(taillePoliceTitre+6);
                label->setFont(QFont("poetsen one",taillePoliceTitre));
                label->setStyleSheet("QLabel { color : " + str2 + " }");
                label->setAlignment(Qt::AlignCenter);

                foreach (QFileInfo fichier, fichierDuDossier)
                {
                    if (fichier.fileName()!="." && fichier.fileName()!=".." &&
                             (fichier.suffix()=="mp3" ||
                             (fichier.suffix()=="wav") ||
                             (fichier.suffix()=="m4a") ))// pour linux et MAC
                    //if (fichier.fileName()!="." && fichier.fileName()!=".." && (fichier.suffix()=="mp3" || (fichier.suffix()=="wav") || (fichier.suffix()=="wma")))// pour windows
                    {
                        QString strNom = fichier.baseName();
                        button = new QPushButton(strNom);


                        if (strNom.contains("-c-"))
                        {
                            //récupération nom fichier sans chiffres du début
                            QStringList listNom = strNom.split("-c-");

                            //Mot apres le underscore
                            strNom = listNom.at(0);
                            QString couleur = listNom.at(1);
                            QString couleurText = ";color : black}";
                            QStringList couleurList;
                            couleurList << "green" << "cyan" << "yellow" << "lightGray";

                            if (couleurList.contains(couleur))
                            {
                                couleurText = ";color : black}";
                            }
                            else
                            {
                                couleurText = ";color : white}";
                            }

                            QString strCouleur = "QPushButton { background-color: " + couleur + couleurText;

                            button->setText(strNom);
                            button->setStyleSheet(strCouleur);

                        }

                        if (strNom.contains("--"))
                        {
                            QStringList listNom2 = strNom.split("--");
                            strNom = listNom2.at(1);
                            button->setText(strNom);

                        }


                        QFont font2 = button->font();
                        font2.setPointSize(taillePoliceBouton);
                        button->setFont(font2);
                        button->setFixedHeight(taillePoliceBouton+6);


                        // On connecte le signal du clic de button au slot map du QSignalMapper
                        QObject::connect(button, SIGNAL(clicked()), signalMapper, SLOT(map()));
                        // On précise au QSignalMapper que button est associé au texte tableau[ligneTotale]
                        signalMapper->setMapping(button, fichier.absoluteFilePath());
                        layout->addWidget(button, numeroLigne, nombreColonne); // positionnement du bouton
                        numeroLigne++;
                    }
                }
                //recuperation taille de chaque colonne
                tailleLayout = layout->totalSizeHint().width()-tailleLayout2;
                tailleLayout2 = tailleLayout2+tailleLayout;
                //taille du label fixer à la taille de la colonne
                label->setFixedWidth(tailleLayout-6);
                //placement des titres des dossiers dans l'horizontal layout
                layout2->addWidget(label);
                numeroLigne = 1; // on revient en haut de la colonne apres la label
                nombreColonne++; // on change de colonne
            }
        }
    }
    // On connecte le QSignalMapper au signal clicked(const QString &)
    QObject::connect(signalMapper, SIGNAL(mapped(const QString &)), this, SLOT(appui(const QString &)));

    scrollArea->setWidget(scrolledWidget);
    scrollArea->setFrameShape(QFrame::NoFrame); // pour enlever la bordure
    scrollArea2->setWidget(scrolledWidget2);
    scrollArea2->setFrameShape(QFrame::NoFrame);

    layoutPrincipal2->addLayout(layout2); //Ajout du layout Titre
    layoutPrincipal->addLayout(layout); // Ajout du layout des boutons

    // bulle d'information sur le bouton satar/stop
    ui->pushButton_2->setToolTip("Vous pouvez également presser la touche CTRL Pause/Start");
    ui->checkBox->setToolTip("Vous pouvez également presser la touche &");
    ui->verticalLayout_2->addLayout(layoutPrincipal2);
    ui->verticalLayout_2->addLayout(layoutPrincipal);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_sliderProgress_sliderMoved(int position)
{
    player->setPosition(position);
}
void Dialog::on_sliderVolume_sliderMoved(int position)
{
    player->setVolume(position);
}
void Dialog::on_pushButton_2_clicked()
{
    if (*etat==false)
    {
        player->pause();
        *etat=true;
    }
    else
    {
        player->play();
        *etat=false;
    }
}

//Methode du mode repeat
void Dialog::verif(int state)
{
    switch(state)
    {
        case Qt::Checked:
        {
        playlist->setPlaybackMode( QMediaPlaylist::Loop ); // mode repeat
            break;
        }
        case Qt::Unchecked:
        {
            playlist->setPlaybackMode(QMediaPlaylist::CurrentItemOnce); // mode lecture simple
           break;
        }
    };
}

void Dialog::on_positionChanged(qint64 position)
{
    ui->sliderProgress->setValue(position);
}

void Dialog::on_durationChanged(qint64 position)
{
    ui->sliderProgress->setMaximum(position);
}

// methode activé lors de l'appui sur un bouton
void Dialog::appui(const QString &text)
{
    *etat=false; // bouton start activé
    playlist->clear(); // effacement du fichier en cours
    playlist->addMedia(QUrl::fromLocalFile(text));    //Load the file
    player->setPlaylist( playlist );
    player->play();
}

//methode pour comparer les touches aux préfixes des noms de fichiers
void Dialog::nomFichiers()
{
    // recuperation répertroire courant
    //QString CurrentDir = QDir::currentPath() + "/RessourcesSoundPad";
    QString CurrentDir = QCoreApplication::applicationDirPath() + "/RessourcesSoundPad";
    QString CurrentDir2 = CurrentDir + "/Clavier";
    QDir repertoire(CurrentDir2); // chemin de type QDir
    if (repertoire.exists())
    {

        QFileInfoList files = repertoire.entryInfoList(); //Liste des fichiers dans le répertoire clavier

        foreach (QFileInfo fichier, files)
       {
           if (fichier.fileName().contains("_") && fichier.fileName()!="." && fichier.fileName()!=".." &&
                    (fichier.suffix()=="mp3" ||
                    (fichier.suffix()=="wav") ||
                    (fichier.suffix()=="m4a") ))// pour linux et MAC
           //if (fichier.fileName().contains("_") && fichier.fileName()!="." && fichier.fileName()!=".." && (fichier.suffix()=="mp3" || (fichier.suffix()=="wav") || (fichier.suffix()=="wma")))// pour windows

               /*Autre formats à tester (fichier.suffix()=="flac") ||
               (fichier.suffix()=="ogg") ||
               (fichier.suffix()=="aac") ||
               (fichier.suffix()=="aiff") ||
               (fichier.suffix()=="opus")
               */
           {
                //récupération nom fichier en separant le nom de la touche
                QStringList list = fichier.baseName().split("_");
                QString touche = list.at(0);
                if (touche==strTouche2 && touche != "&")
                {
                    *etat=false;
                   playlist->clear();
                   playlist->addMedia(QUrl::fromLocalFile(fichier.absoluteFilePath()));
                   player->setPlaylist( playlist );
                   player->play();
                }
           }
        }
   }
}

void Dialog::keyPressEvent(QKeyEvent *event)
{
   if (event->key() == Qt::Key_Control || event->key() == Qt::Key_Meta || event->key() == Qt::Key_F32) // CTRL windows ou META pour MAC
    {
        //gestion du Start/Pause
        if (*etat==false) // En lecture
        {
            player->pause();
            *etat=true;
        }
        else // En pause
        {
            player->play();
            *etat=false;
        }
    }

   else
    {
        *etat = false;
        // recuperation répertroire courant
        //QString CurrentDir = QDir::currentPath()  + "/RessourcesSoundPad";
       QString CurrentDir = QCoreApplication::applicationDirPath() + "/RessourcesSoundPad";
        QString CurrentDir2 = CurrentDir + "/Clavier";
        QDir repertoire; // chemin de type QDir
        QString dossier, fichier("");

        //Touche Shift
        if (event->key() == Qt::Key_Shift && repertoire.exists())
        {
            incrementBis ++;
            dossier = CurrentDir2 + "/Shift";
            repertoire.setPath(dossier); // chemin de type QDir
            if (repertoire.exists())
            {
                QFileInfoList files = repertoire.entryInfoList(); //Liste des fichiers dans le répertoire courant
                int nombreFichierBis(0);
                foreach (QFileInfo file, files)
                {
                    if (     file.suffix()=="mp3" ||
                             (file.suffix()=="wav") ||
                             (file.suffix()=="m4a") )// pour linux et MAC
                    //if (file.suffix()=="mp3" || (file.suffix()=="wav") || (file.suffix()=="wma"))// pour windows
                    {
                        nombreFichierBis ++; // compte du nombre de fichiers dans le répertoire               
                    }
                }
                //QString leFichier[nombreFichierBis];
                if (nombreFichierBis>0)
                {
                    QString *leFichier = new QString[nombreFichierBis];
                    nombreFichierBis = 0;
                    foreach (QFileInfo file, files)
                    {
                        if (     file.suffix()=="mp3" ||
                                 (file.suffix()=="wav") ||
                                 (file.suffix()=="m4a") )// pour linux et MAC
                        //if (file.suffix()=="mp3" || (file.suffix()=="wav") || (file.suffix()=="wma"))// pour windows
                        {
                            leFichier[nombreFichierBis] = file.absoluteFilePath(); // tableau comportant les noms des fichiers du répertoire
                            nombreFichierBis ++;
                        }
                    }
                    if (incrementBis<nombreFichierBis)
                    {
                        fichier = leFichier[incrementBis]; // lecture du fichier numero "increment"
                    }
                    else
                    {
                        fichier = leFichier[0];
                        incrementBis=0;
                    }
                }
            }
        }

        else
        {
            // Touche Capslock
            //if (event->key() == Qt::Key_CapsLock && repertoire.exists())
            if ((event->key() == Qt::Key_At ) && repertoire.exists())
            {
                increment ++;
                dossier = CurrentDir2 + "/@";
                repertoire.setPath(dossier); // chemin de type QDir
                if (repertoire.exists())
                {
                    QFileInfoList files = repertoire.entryInfoList(); //Liste des fichiers dans le répertoire courant
                    int nombreFichier(0);
                    foreach (QFileInfo file, files)
                    {
                        if (     file.suffix()=="mp3" ||
                                 (file.suffix()=="wav") ||
                                 (file.suffix()=="m4a") )// pour linux et MAC
                        //if (file.suffix()=="mp3" || (file.suffix()=="wav") || (file.suffix()=="wma"))// pour windows
                        {
                            nombreFichier ++; // compte du nombre de fichiers dans le répertoire
                        }
                    }
                    //QString leFichier[nombreFichier];
                    if (nombreFichier>0)
                    {
                        QString *leFichier = new QString[nombreFichier];
                        nombreFichier = 0;
                        foreach (QFileInfo file, files)
                        {
                            if (     file.suffix()=="mp3" ||
                                     (file.suffix()=="wav") ||
                                     (file.suffix()=="m4a") )// pour linux et MAC
                            //if (file.suffix()=="mp3" || (file.suffix()=="wav") || (file.suffix()=="wma"))// pour windows
                            {
                                leFichier[nombreFichier] = file.absoluteFilePath(); // tableau comportant les noms des fichiers du répertoire
                                nombreFichier ++;
                            }
                        }
                        if (increment<nombreFichier)
                        {
                            fichier = leFichier[increment]; // lecture du fichier numero "increment"
                        }
                        else
                        {
                        fichier = leFichier[0];
                        increment=0;
                        }
                    }
                }
            }
            else
            {
                //recupếration nom de la touche appuyée
               char touche = (char)event->key();
               QString strTouche(touche);
               strTouche2 = strTouche;

               //mode repeat
               if (strTouche == "&")
               {
                   if (ui->checkBox->isChecked())
                   {
                        ui->checkBox->setCheckState(Qt::Unchecked);
                   }
                   else
                   {
                       ui->checkBox->setCheckState(Qt::Checked);
                   }
               }

               nomFichiers(); // lancement méthode pour comparaison touche avec fichiers
            }
        }
        //lecture du fichier
        if (fichier!="")
           {
               playlist->clear();
               playlist->addMedia(QUrl::fromLocalFile(fichier));
               player->setPlaylist( playlist );
               player->play();
           }
    }
}

