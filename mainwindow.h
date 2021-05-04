#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QPlainTextEdit>
#include <thread>
#include <time.h>
#include <unistd.h>
#include <QSlider>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QFont>

#include "QSettings"
#include "fontselect.h"
#include "highlighter.h"
#include "codeeditor.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

extern int Automatic_save_control;
extern std::string path;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // dosya adlari bu liste de tutulur
    QList<QString> files;

    // tab lardaki tum QTextEdit ler bu liste de tutulur
    QList<CodeEditor *> texts;

    // ekranda yazdirilan dosya adlari
    QList<QString> names;

    // tab yani dosya sayisi
    int tab_count;

    // secilen metni buyutmek icin slider
    QSlider *slider;

    // genel font size
    int scrol;

    // text font
    QFont font;

    QList<Highlighter*> code_color;

    int cpp;


protected:

    void dragEnterEvent(QDragEnterEvent *e);
    void dropEvent(QDropEvent *e);
    void keyReleaseEvent(QKeyEvent *);
    void moveEvent(QMoveEvent * event);
    void resizeEvent(QResizeEvent* event);

private slots:
    void on_actionNew_File_triggered();

    void on_actionOpen_triggered();

    void tab_add(QString text, int file_check, int tempRead);

    void on_tabWidget_tabCloseRequested(int index);

    void on_actionSave_triggered();

    void Text_Write(bool a);

    void on_actionAll_save_triggered();

    void on_actionAutomatic_save_triggered();

    void enlarge();

    void plus();

    void minus();

    void on_actionZoom_Out_triggered();

    void on_actionZoom_In_triggered();

    void on_actionFont_triggered();

    void on_actionSelect_All_triggered();

    void on_actionSelect_None_triggered();

    void on_actionCopy_triggered();

    void on_actionPaste_triggered();

    void on_actionCut_triggered();

    void read_temp();

    void Automatic_save();

    void on_actionAutomatic_C_C_control_triggered();

    void tabSelected(int src, int to);

private:
    Ui::MainWindow *ui;
};

extern MainWindow *ui_main;
#endif // MAINWINDOW_H
