#include "mainwindow.h"

// kod tamamlama
void CodeKeyReleaseEvent(QKeyEvent *event, QTabWidget *widget_tab){


    // parantez
    if(event->key()==Qt::Key_ParenLeft){

        ui_main->texts.at(widget_tab->currentIndex())->insertPlainText(")");

        QTextCursor cursor = ui_main->texts.at(widget_tab->currentIndex())->textCursor();
        cursor.setPosition(cursor.position()-1);
        ui_main->texts.at(widget_tab->currentIndex())->setTextCursor( cursor );

    }

    // >
    else if(event->key()==Qt::Key_Less){

        int cr = ui_main->texts.at(widget_tab->currentIndex())->textCursor().position();
        QString str = ui_main->texts.at(widget_tab->currentIndex())->toPlainText();
        int flag=-1;

        int temp=cr-10;

        if(temp<=0){
            temp=0;
        }

        for(int i=temp; i<cr; i++){
            flag = str.indexOf("#include",i);

            if(flag!=-1){
                break;
            }
        }

        if(flag!=-1){

            ui_main->texts.at(widget_tab->currentIndex())->insertPlainText(">");

            QTextCursor cursor = ui_main->texts.at(widget_tab->currentIndex())->textCursor();
            cursor.setPosition(cursor.position()-1);
            ui_main->texts.at(widget_tab->currentIndex())->setTextCursor( cursor );

        }

    }

    // süslü parantez
    else if(event->key()==Qt::Key_BraceLeft){

        ui_main->texts.at(widget_tab->currentIndex())->insertPlainText("\n");

        QString str = ui_main->texts.at(widget_tab->currentIndex())->toPlainText();

        int j=-1;
        int open=0;
        int close=0;
        int tmp=0;

        while(j<str.size()){

            j++;

            if(str.indexOf("{",j)==j){
                open++;
            }

            else if(str.indexOf("}",j)==j){
                close++;
            }

            if(j==ui_main->texts.at(widget_tab->currentIndex())->textCursor().position()){

                tmp = open-close;
                tmp-=1;


                for(int i=0; i<tmp; i++){

                    ui_main->texts.at(widget_tab->currentIndex())->insertPlainText("	");
                }

                break;

            }

        }

        ui_main->texts.at(widget_tab->currentIndex())->insertPlainText("}");

        QTextCursor cursor = ui_main->texts.at(widget_tab->currentIndex())->textCursor();
        cursor.setPosition(cursor.position()-tmp-2);
        ui_main->texts.at(widget_tab->currentIndex())->setTextCursor( cursor );

    }

    // koseli parantez
    else if(event->key()==Qt::Key_BracketLeft){

        ui_main->texts.at(widget_tab->currentIndex())->insertPlainText("]");

        QTextCursor cursor = ui_main->texts.at(widget_tab->currentIndex())->textCursor();
        cursor.setPosition(cursor.position()-1);
        ui_main->texts.at(widget_tab->currentIndex())->setTextCursor( cursor );

    }

    // tirnak
    else if(event->key()==Qt::Key_QuoteDbl){

        ui_main->texts.at(widget_tab->currentIndex())->insertPlainText("\"");

        QTextCursor cursor = ui_main->texts.at(widget_tab->currentIndex())->textCursor();
        cursor.setPosition(cursor.position()-1);
        ui_main->texts.at(widget_tab->currentIndex())->setTextCursor( cursor );

    }

    // tek tirnak
    else if(event->key()==Qt::Key_Apostrophe){

        ui_main->texts.at(widget_tab->currentIndex())->insertPlainText("'");

        QTextCursor cursor = ui_main->texts.at(widget_tab->currentIndex())->textCursor();
        cursor.setPosition(cursor.position()-1);
        ui_main->texts.at(widget_tab->currentIndex())->setTextCursor( cursor );

    }

    // tab bosluk (enter)
    if(event->key()==Qt::Key_Enter || event->key()==Qt::Key_Return){

        QString str = ui_main->texts.at(widget_tab->currentIndex())->toPlainText();

        int j=-1;
        int open=0;
        int close=0;

        while(j<str.size()){

            j++;

            if(str.indexOf("{",j)==j){
                open++;
            }

            else if(str.indexOf("}",j)==j){
                close++;
            }

            if(j==ui_main->texts.at(widget_tab->currentIndex())->textCursor().position()){

                int tmp = open-close;

                for(int i=0; i<tmp; i++){

                    ui_main->texts.at(widget_tab->currentIndex())->insertPlainText("	");

                }

                break;

            }

        }

        // otomatik noktali vurgul kontrolu
        if(ui_main->cpp!=1){
            return;
        }

        str = ui_main->texts.at(widget_tab->currentIndex())->toPlainText();
        QStringList list = str.split("\n");
        int i=-1;
        int size=0;

        while(true){

            i++;

            if(i+1==list.size()){
                break;
            }


            QString sn = list.at(i);

            int cn = sn.size()-1;
            size+=sn.size();


            QString sn0 = list.at(i+1);
            int cn0 = sn0.size()-1;

            if(cn<=0){
                continue;
            }


            if(!(cn0<=0)){
                if(sn0[cn0]=="{"){
                    continue;
                }
            }

            if(sn[cn]=="{"){
                continue;
            }

            else if(sn[cn]==";"){
                continue;
            }

            else if(sn[cn]=="	"){
                continue;
            }

            else if(sn[cn]==">"){
                continue;
            }

            else if(sn[cn]=="}"){
                continue;
            }

            else{

                QTextCursor cr = ui_main->texts.at(widget_tab->currentIndex())->textCursor();
                int main_cr=cr.position();

                cr.setPosition(size+i);
                ui_main->texts.at(widget_tab->currentIndex())->setTextCursor(cr);
                ui_main->texts.at(widget_tab->currentIndex())->insertPlainText(";");

                cr.setPosition(main_cr+1);
                ui_main->texts.at(widget_tab->currentIndex())->setTextCursor(cr);

                break;

            }

        }

    }

}
