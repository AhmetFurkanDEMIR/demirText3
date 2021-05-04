#ifndef FONTSELECT_H
#define FONTSELECT_H

#include <QDialog>

namespace Ui {
class FontSelect;
}

class FontSelect : public QDialog
{
    Q_OBJECT

public:
    explicit FontSelect(QWidget *parent = nullptr);
    ~FontSelect();

private slots:

    void on_fontComboBox_currentFontChanged(const QFont &f);


private:
    Ui::FontSelect *ui;
};

#endif // FONTSELECT_H
