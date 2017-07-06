#include "dialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //Création de la fenetre
    Dialog w;
    //Recuperation répertroire courant
    //QString CurrentDir = QDir::currentPath() + "/RessourcesSoundPad";
    QString CurrentDir = QCoreApplication::applicationDirPath() + "/RessourcesSoundPad";    
    //Chemin du Logo
    QString icon = CurrentDir + "/Logo_GM.icns";
    w.setWindowIcon(QIcon(icon)); //Parametrage du Logo


    w.showMaximized();//Affichage fentre de la taille de l'écran

    return a.exec();
}

