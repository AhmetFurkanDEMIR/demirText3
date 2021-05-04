#include "mainwindow.h"
#include <QStyleFactory>
#include <QApplication>
#include <QMessageBox>
#include <stdio.h>

int Automatic_save_control=0;
MainWindow *ui_main;
std::string path;


int main(int argc, char *argv[])
{

    path=get_current_dir_name();

    QApplication a(argc, argv);

    // dark thema
    qApp->setStyle(QStyleFactory::create("Fusion"));
    QPalette darkPalette;

    darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(40, 40, 40));
    darkPalette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(75, 75, 75));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);

    qApp->setPalette(darkPalette);

    char buffer [10];
    FILE *filea = popen("wget -r -np -A .txt https://demirtext3.s3.amazonaws.com/UpdateCheck/UpdateCheck.txt","r");
    fgets (buffer , 10 , filea);

    QFile file(QString::fromStdString(path)+"/demirtext3.s3.amazonaws.com/UpdateCheck/UpdateCheck.txt");

    int check = file.open(QIODevice::ReadOnly);
    QTextStream in(&file);

    if(!check){
    }

    else if(in.readAll()!="1.0.0.0"){

        QMessageBox messageBox;

        if(QSysInfo::productType()!="windows"){
            messageBox.information(0,"New version detected","Please download the new version to use demirText3 in full performance. Download link: <a href='https://demirtext3.s3.amazonaws.com/Linux/Linux.rar'>demirText3 (aws S3 url)</a>");

        }

        else{
            messageBox.information(0,"New version detected","Please download the new version to use demirText3 in full performance. Download link: <a href='https://demirtext3.s3.amazonaws.com/Windows10/Windows10.rar'>demirText3 (aws S3 url)</a>");

        }

    }

    MainWindow w;
    ui_main = &w;
    w.show();
    return a.exec();
}
