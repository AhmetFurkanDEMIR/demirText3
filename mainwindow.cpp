#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QKeyEvent>
#include <QScreen>
#include "CodeCheck.cpp"
#include <QToolButton>

// Constructor
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // surukleyerek dosya ekleme
    this->setAcceptDrops(true);

    // cerceveleri sifirlamak
    ui->centralwidget->layout()->setContentsMargins(0, 5, 0, 0);

    // tablerin yeri degisir ise yapilarin yerinin degismesini saglayan signal
    connect(ui->tabWidget->tabBar(), SIGNAL(tabMoved(int, int)), this, SLOT(tabSelected(int, int)));

    // uygulama yeniden calisinca eski konumunda calismasini saglamak
    int w=this->size().width();
    int h=this->size().height();
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int height_res = screenGeometry.height();
    int width_res = screenGeometry.width();
    QSettings settings("demir.ai", "demirText3");
    QVariant value_moveX = settings.value("x",(width_res/2)-(w/2));
    QVariant value_moveY = settings.value("y",(height_res/2)-(h/2));
    this->move(value_moveX.toInt(), value_moveY.toInt());

    // uygulama yeniden calisinca eski boyutlarda calismasini saglamak
    QVariant resizeX = settings.value("resizeX",this->size().width());
    QVariant resizeY = settings.value("resizeY",this->size().height());
    this->resize(resizeX.toInt(), resizeY.toInt());

    // yazi boyutu, kayittan alinir.
    QVariant value = settings.value("size",15);

    // secilen yaziyi buyultmek icin slider
    slider=new QSlider(Qt::Horizontal);
    slider->setMaximumWidth(100);
    slider->setMinimum(10);
    slider->setMaximum(60);
    ui->toolBar->addWidget(slider);
    ui->toolBar->addSeparator();
    slider->setValue(value.toInt());
    // slider deki herhangi bir tetiklemede enlarge fonksiyonuna gidilir
    connect(slider, SIGNAL(actionTriggered(int)), this, SLOT(enlarge()));

    // yazi fontumuz
    QFont BitstreamCharter("Sans", value.toInt());
    font = BitstreamCharter;

    // code yazi alanimiz
    CodeEditor *plain = new CodeEditor();
    plain->setFont(font);
    plain->setTabStopWidth(plain->fontMetrics().width(' ') * 4);
    code_color.append(new Highlighter(plain->document()));

    // QplainTextEdit 'e her islem yapildiginda Text_Write fonksiyonuna ilerlenir
    connect(plain, SIGNAL(modificationChanged(bool)), this, SLOT(Text_Write(bool)));
    connect(plain, SIGNAL(cursorPositionChanged()), this, SLOT(Automatic_save()));

    // tab 'e widget ekleme
    QWidget *mywg = new QWidget();
    QGridLayout *ly = new QGridLayout();
    ly->layout()->setContentsMargins(0, 0, 0, 0);
    ly->addWidget(plain);
    mywg->setLayout(ly);
    ui->tabWidget->insertTab(tab_count,mywg,"None");
    texts.append(plain);

    // tab_count, programdaki acik sekme sayisi yani acik dosya sayisi
    tab_count=0;

    // scrol, yazi boyutu
    scrol=value.toInt();

    // en sonki acik dosyalar okunur
    read_temp();

    // otomatik (;) kontrolu
    cpp=0;

}

MainWindow::~MainWindow()
{
    delete ui;
}

// tablerin yeri degisir ise yapilarin yerinin degismesini saglayan fonksiyon
void MainWindow::tabSelected(int src, int to){

    names.swap(src, to);
    files.swap(src, to);
    texts.swap(src, to);

}

// en sonki pencere boyutlarini kaydetme
void MainWindow::resizeEvent(QResizeEvent* event){

    int w=event->size().width();
    int h=event->size().height();

    QSettings settings("demir.ai", "demirText3");
    settings.setValue("resizeX",w);
    settings.setValue("resizeY",h);

}

// uygulama yeniden calisinca eski konumunda calismasini saglamak
void MainWindow::moveEvent(QMoveEvent * event){

    int w=event->pos().x();
    int h=event->pos().y();

    QSettings settings("demir.ai", "demirText3");
    settings.setValue("x",w);
    settings.setValue("y",h);

}

// uygulama yeniden calisinca eski dosyalari okuma
void MainWindow::read_temp(){

    QDir().mkdir(QString::fromStdString(path)+"/.demirText3");
    QFile temp_fileRead(QString::fromStdString(path)+"/.demirText3/.demirText3.txt");
    temp_fileRead.open(QIODevice::ReadOnly);

    QTextStream in(&temp_fileRead);

    while (!in.atEnd()) {

        QFile file(in.readLine());
        file.open(QIODevice::ReadOnly);
        files.append(file.fileName());
        QTextStream in(&file);

        QString temp = in.readAll();
        file.close();
        tab_add(temp, 1, 99);

    }
}

// tetiklenme, klavye
void MainWindow::keyReleaseEvent(QKeyEvent *event){

    // kodun düzenini sağlamak
    CodeKeyReleaseEvent(event, ui->tabWidget);
}

// dosya islem uzerinde ise dosya adi degistirilir
void MainWindow::Text_Write(bool a){

    if(ui->tabWidget->tabText(0)=="None"){
        return;
    }

    if(a==true){

        // none ise return, cunku kaydedilmesine gerek yok
        if(ui->tabWidget->tabText(ui->tabWidget->currentIndex())=="None"){
            return;
        }

        // dosya uzerinde islem var olarak gosterilir
        else if(Automatic_save_control!=1){
            ui->tabWidget->setTabText(ui->tabWidget->currentIndex(), names.at(ui->tabWidget->currentIndex())+" *");
        }
    }

}

// yeni bir tab ekleme fonksiyonu
void MainWindow::tab_add(QString text, int file_check, int tempRead){

    // eger dosyadan daha once yok ise normal bir sekilde dosya adi yazilir
    // aksi halde karismamasi icin tam yolu yazilir
    QStringList temp = files.at(files.count()-1).split("/");
    QString name_temp = temp.at(temp.size()-1);

    // dosyanin olup olmama kontrolu
    int flag=0;
    foreach (QString tn, names) {

        if(tn==name_temp){
            flag=1;
            break;

        }

    }

    // dosya daha once var ise
    if(flag==1){
        names.append(files.at(files.count()-1));
    }

    // dosya daha once yok ise
    else{
        names.append(name_temp);
    }

    // dosya acilir
    QFile file(files.at(files.count()-1));
    file.open(QIODevice::WriteOnly);

    // ilk tab eklenecekse bu bloga girilir
    if(ui->tabWidget->tabText(0)=="None"){

        tab_count++;

        // ilk tab da herhangi bir text var ise dosyaya yazilir
        if(file_check==0){

            QString temp = texts.at(0)->toPlainText();
            QTextStream in(&file);
            file.write(qPrintable(temp));
            file.close();

        }

        // eger text yok ise bu bloga girilir.
        else{

            QTextStream in(&file);
            texts.at(0)->setPlainText(text);
            file.write(qPrintable(text));
            file.close();

        }

        // baslik guncellenir ve tab_count arttirilir
        ui->tabWidget->setTabText(0, names.at(names.count()-1));

    }

    // ilkden sonraki tum yeni tablar/dosyalar da bu bloga girer
    else{

        tab_count++;
        QWidget *mywg = new QWidget();

        // yazi alanimiz
        CodeEditor *plain = new CodeEditor();
        plain->setPlainText(text);
        plain->setFont(font);
        plain->setTabStopWidth(plain->fontMetrics().width(' ') * 4);

        code_color.append(new Highlighter(plain->document()));
        texts.append(plain);

        // QPlaintext 'e her islem yapildiginda Text_Write fonksiyonuna ilerlenir
        connect(plain, SIGNAL(modificationChanged(bool)), this, SLOT(Text_Write(bool)));
        connect(plain, SIGNAL(cursorPositionChanged()), this, SLOT(Automatic_save()));

        QGridLayout *ly = new QGridLayout();
        ly->addWidget(plain);
        ly->layout()->setContentsMargins(0, 0, 0, 0);
        mywg->setLayout(ly);
        ui->tabWidget->insertTab(tab_count-1,mywg,names.at(names.count()-1));

        file.write(qPrintable(text));
        file.close();

    }

    // mevcut tab 'a ilerlenir, ekranda yansitlir
    ui->tabWidget->setCurrentIndex(tab_count-1);

    if(tempRead!=99){

        QDir().mkdir(QString::fromStdString(path)+"/.demirText3");
        QFile temp_fileWrite(QString::fromStdString(path)+"/.demirText3/.demirText3.txt");
        temp_fileWrite.open(QIODevice::WriteOnly);

        for(int i=0; i<files.count(); i++){
            temp_fileWrite.write(qPrintable(files.at(i)+"\n"));
        }

        temp_fileWrite.close();
    }

}

// tab yani dosya kapatma fonksiyonu
void MainWindow::on_tabWidget_tabCloseRequested(int index)
{

    // eger acikta hicbir tab yok ise text temizlenir ve return edilir
    if(tab_count==0){
        texts.at(0)->setPlainText("");
        return;
    }

    // son tab var ise o tab kapatilir
    if(tab_count==1){

        QFile file(QString::fromStdString(path)+"/.demirText3/__"+names.at(index)+"__"+QString::number(index+1)+".txt");
        file.remove();

        texts.at(0)->setPlainText("");
        ui->tabWidget->setTabText(0, "None");
        files.removeAt(index);
        names.removeAt(index);
        code_color.removeAt(index);

        tab_count--;

    }

    // diger tabler kapatilir ve yapilardan silinir
    else{

        QFile file(QString::fromStdString(path)+"/.demirText3/__"+names.at(index)+"__"+QString::number(index+1)+".txt");
        file.remove();

        ui->tabWidget->removeTab(index);
        files.removeAt(index);
        names.removeAt(index);
        texts.removeAt(index);
        code_color.removeAt(index);

        tab_count--;

    }

    QDir().mkdir(QString::fromStdString(path)+"/.demirText3");
    QFile temp_fileWrite(QString::fromStdString(path)+"/.demirText3/.demirText3.txt");
    temp_fileWrite.open(QIODevice::WriteOnly);

    for(int i=0; i<files.count(); i++){

        for(int i=0; i<files.count(); i++){
            temp_fileWrite.write(qPrintable(files.at(i)+"\n"));
        }

        temp_fileWrite.close();
    }

}

// yeni dosya acma fonksiyonu
void MainWindow::on_actionNew_File_triggered()
{
    // dosya fileDialog yardimi ile secilir ve acilir
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::AnyFile);
    QString strFile = dialog.getSaveFileName(NULL, "Create New File","","");

    if(strFile=="" || strFile==" "){
        return;
    }

    // ayni dosya daha onceden acildi ise tekrar acilmaz
    // sadece o tab 'e ilerlenir
    int flag=0;
    int count = -1;
    foreach(QString file_temp,files){

        count++;

        if(file_temp==strFile){
            ui->tabWidget->setCurrentIndex(count);
            flag=1;
            break;
        }

    }

    // eger daha onceden boyle bir dosya yok ise yenisi olusturulur
    if(flag==0){

        QFile file(strFile);
        file.open(QIODevice::WriteOnly);
        files.append(file.fileName());
        tab_add("", 0, -99);
        file.close();
    }


}

// kayitli bir dosya acma fonksiyonu
void MainWindow::on_actionOpen_triggered()
{

    // dosya fileDialog ile secilir
    QString open_file = QFileDialog::getOpenFileName(this,
            tr("Open File"), "/home");

    if(open_file=="" || open_file==" "){
        return;
    }

    // ayni dosya daha onceden acildi ise tekrar acilmaz
    // sadece o tab 'e ilerlenir
    int flag=0;
    int count = -1;
    foreach(QString file_temp,files){

        count++;

        if(file_temp==open_file){
            flag=1;
            ui->tabWidget->setCurrentIndex(count);
            break;
        }

    }

    // eger daha onceden boyle bir dosya yok ise o dosya cilir ve yeni bir tab olusturulur
    if(flag==0){

        QFile file(open_file);
        file.open(QIODevice::ReadOnly);
        files.append(file.fileName());
        QTextStream in(&file);

        QString temp = in.readAll();
        file.close();
        tab_add(temp, 1, -99);
    }

}

// surukleme
void MainWindow::dragEnterEvent(QDragEnterEvent *e)
{

    if (e->mimeData()->hasUrls()) {
        e->acceptProposedAction();
    }
}

// surukleyerek ekleme (birakma)
void MainWindow::dropEvent(QDropEvent *e)
{
    foreach (const QUrl &url, e->mimeData()->urls()) {
        QString fileName = url.toLocalFile();

        // ayni dosya daha onceden acildi ise tekrar acilmaz
        // sadece o tab 'e ilerlenir
        int flag=0;
        int count = -1;
        foreach(QString file_temp,files){

            count++;

            if(file_temp==fileName){
                flag=1;
                ui->tabWidget->setCurrentIndex(count);
                break;
            }

        }

        // eger daha onceden boyle bir dosya yok ise o dosya cilir ve yeni bir tab olusturulur
        if(flag==0){

            QFile file(fileName);
            file.open(QIODevice::ReadOnly);
            files.append(file.fileName());
            QTextStream in(&file);

            QString temp = in.readAll();
            file.close();
            tab_add(temp, 1, -99);
        }

    }
}

// tum dosyaları gunceleme triggred 'i
void MainWindow::on_actionAll_save_triggered()
{
    if(ui->tabWidget->tabText(ui->tabWidget->currentIndex())=="None"){

        return;
    }

    else{

        // tum dosyalar guncellenir
        for(int i=0; i<tab_count; i++){

            QFile file(files.at(i));
            file.open(QIODevice::WriteOnly);

            QString temp = texts.at(i)->toPlainText();
            file.write(qPrintable(temp));
            file.close();

            QTextCursor cr = texts.at(i)->textCursor();
            int cr0 = texts.at(i)->textCursor().position();

            texts.at(i)->setPlainText(temp);
            cr.setPosition(cr0);
            texts.at(i)->setTextCursor(cr);

            ui->tabWidget->setTabText(i, names.at(i));

        }

        on_actionSave_triggered();

    }
}

// otomatik tum dosyalari guncelleme
void MainWindow::Automatic_save(){

    qDebug("fds");

    if(Automatic_save_control==1 && ui->tabWidget->tabText(0)!="None"){

        QFile file(files.at(ui->tabWidget->currentIndex()));
        file.open(QIODevice::WriteOnly);

        QString temp = texts.at(ui->tabWidget->currentIndex())->toPlainText();
        file.write(qPrintable(temp));
        file.close();
        ui->tabWidget->setTabText(ui->tabWidget->currentIndex(), names.at(ui->tabWidget->currentIndex()));

    }

}

// otomatik tum dosyalari guncelleme
void MainWindow::on_actionAutomatic_save_triggered()
{
    if(Automatic_save_control==0){

        on_actionAll_save_triggered();
        Automatic_save_control=1;

    }

    else{

        for(int i=0; i<tab_count; i++){

            QString temp=texts.at(i)->toPlainText();

            QTextCursor cr = texts.at(i)->textCursor();
            int cr0 = texts.at(i)->textCursor().position();

            texts.at(i)->setPlainText(temp);
            cr.setPosition(cr0);
            texts.at(i)->setTextCursor(cr);
        }

        Automatic_save_control=0;
    }
}


// save islemi
void MainWindow::on_actionSave_triggered()
{

    // dosya yok ise mevcut text ile yeni bir dosya olusturulur
    if(ui->tabWidget->tabText(0)=="None"){

        QFileDialog dialog;
        dialog.setFileMode(QFileDialog::AnyFile);
        QString strFile = dialog.getSaveFileName(NULL, "Save File","","");

        if(strFile=="" || strFile==" "){
            return;
        }

        files.append(strFile);
        QFile file(strFile);
        file.open(QIODevice::WriteOnly);
        file.close();

        tab_add(texts.at(0)->toPlainText(), 0, -99);

        return;

    }

    // dosya var ise uzerine yazilir


    QFile file(files.at(ui->tabWidget->currentIndex()));
    file.open(QIODevice::WriteOnly);

    QString temp = texts.at(ui->tabWidget->currentIndex())->toPlainText();
    file.write(qPrintable(temp));

    QTextCursor cr = texts.at(ui->tabWidget->currentIndex())->textCursor();
    int cr0 = texts.at(ui->tabWidget->currentIndex())->textCursor().position();

    texts.at(ui->tabWidget->currentIndex())->setPlainText(temp);
    cr.setPosition(cr0);
    texts.at(ui->tabWidget->currentIndex())->setTextCursor(cr);

    ui->tabWidget->setTabText(ui->tabWidget->currentIndex(), names.at(ui->tabWidget->currentIndex()));


}


// slider degerine gore secilen text font size guncellenir
void MainWindow::enlarge(){

    QSettings settings("demir.ai", "demirText3");
    settings.clear();
    settings.setValue("size",slider->value());

    for(int i=0; i<texts.size(); i++){

        font.setPointSize(slider->value());
        texts.at(i)->setFont(font);
        scrol=slider->value();

        texts.at(i)->setTabStopWidth(texts.at(i)->fontMetrics().width(' ') * 4);

    }

}

// tum text in font sizei yukseltilir ve update edilir
void MainWindow::plus(){


    if(scrol==60){
        return;
    }

    scrol+=1;

    QSettings settings("demir.ai", "demirText3");
    settings.setValue("size",scrol);
    slider->setValue(scrol);

    for(int i=0; i<texts.size(); i++){

        font.setPointSize(scrol);
        texts.at(i)->setFont(font);
        texts.at(i)->setTabStopWidth(texts.at(i)->fontMetrics().width(' ') * 4);

    }


}

// tum text in font sizei eksiltilir ve update edilir
void MainWindow::minus(){

    if(scrol==10){
        return;
    }

    scrol-=1;

    QSettings settings("demir.ai", "demirText3");
    settings.setValue("size",scrol);
    slider->setValue(scrol);

    for(int i=0; i<texts.size(); i++){

        font.setPointSize(scrol);
        texts.at(i)->setFont(font);
        texts.at(i)->setTabStopWidth(texts.at(i)->fontMetrics().width(' ') * 4);

    }
}

// zoom out
void MainWindow::on_actionZoom_Out_triggered()
{
    minus();
}

// zoom in
void MainWindow::on_actionZoom_In_triggered()
{
    plus();
}

// font secimi, yeni bir window acilir
void MainWindow::on_actionFont_triggered()
{
    FontSelect *window = new FontSelect(this);
    window->exec();

}

// hepsini sec
void MainWindow::on_actionSelect_All_triggered()
{
    texts.at(ui->tabWidget->currentIndex())->selectAll();
}

// secmeyi birak
void MainWindow::on_actionSelect_None_triggered()
{
    QTextCursor cursor = texts.at(ui->tabWidget->currentIndex())->textCursor();
    cursor.movePosition( QTextCursor::End );
    texts.at(ui->tabWidget->currentIndex())->setTextCursor( cursor );

}

// copy
void MainWindow::on_actionCopy_triggered()
{

    texts.at(ui->tabWidget->currentIndex())->copy();
}

// paste
void MainWindow::on_actionPaste_triggered()
{

    texts.at(ui->tabWidget->currentIndex())->paste();
}

// cut
void MainWindow::on_actionCut_triggered()
{
    texts.at(ui->tabWidget->currentIndex())->cut();
}

// noktali virgul kontrolu
void MainWindow::on_actionAutomatic_C_C_control_triggered()
{
    if(cpp==0){
        cpp=1;
    }
    else{
        cpp=0;
    }
}
