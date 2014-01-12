#ifndef PWMDIALOG_H
#define PWMDIALOG_H

#include <QDialog>

namespace Ui {
class pwmDialog;
}

class pwmDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit pwmDialog(QWidget *parent = 0);
    ~pwmDialog();
    void closeEvent(QCloseEvent *event);

private slots:
    int tiny6410PwmSlot();
    void tiny6410PwmOpenSlot(void);

private:
    Ui::pwmDialog *ui;
    int pwmFd;
    int ioctrlRet;
    int pwmFreq;

    void setBuzzerFreq(int freq);
};

#endif // PWMDIALOG_H
