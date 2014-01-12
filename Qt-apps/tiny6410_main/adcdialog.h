#ifndef ADCDIALOG_H
#define ADCDIALOG_H

#include <QDialog>

namespace Ui {
class adcDialog;
}

class adcDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit adcDialog(QWidget *parent = 0);
    ~adcDialog();

private slots:
    void changeAdcValue(void);
    
private:
    Ui::adcDialog *ui;
};

#endif // ADCDIALOG_H
