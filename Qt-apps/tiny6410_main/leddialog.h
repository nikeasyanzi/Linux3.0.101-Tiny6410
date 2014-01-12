#ifndef LEDDIALOG_H
#define LEDDIALOG_H

#include <QDialog>

namespace Ui {
class ledDialog;
}

class ledDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ledDialog(QWidget *parent = 0);
    ~ledDialog();

private slots:
    int tiny6410LedSlot();

private:
    Ui::ledDialog *ui;
};

#endif // LEDDIALOG_H
