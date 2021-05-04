#include "fontselect.h"
#include "ui_fontselect.h"
#include "mainwindow.h"

FontSelect::FontSelect(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FontSelect)
{
    ui->setupUi(this);

    this->setFixedSize(255,229);

    QFont font;
    QFont BitstreamCharter("Sans", ui_main->scrol);
    font = BitstreamCharter;

}

FontSelect::~FontSelect()
{
    delete ui;
}

void FontSelect::on_fontComboBox_currentFontChanged(const QFont &f)
{
    QFont fn=f;
    fn.setPointSize(ui_main->scrol);
    ui_main->font=fn;

    for(int i=0; i<ui_main->texts.size(); i++){

        ui_main->texts.at(i)->selectAll();
        ui_main->texts.at(i)->setFont(fn);

        QTextCursor cursor = ui_main->texts.at(i)->textCursor();
        cursor.movePosition( QTextCursor::End );
        ui_main->texts.at(i)->setTextCursor( cursor );
    }


}

