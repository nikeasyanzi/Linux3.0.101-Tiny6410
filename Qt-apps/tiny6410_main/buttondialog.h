#ifndef BUTTONDIALOG_H
#define BUTTONDIALOG_H

#include <QDialog>

namespace Ui {
class buttonDialog;
}

class buttonDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit buttonDialog(QWidget *parent = 0);
    ~buttonDialog();

private slots:
    int buttonsMsgSlot();
    void emitButtonsMsg();
    
private:
    Ui::buttonDialog *ui;
    void buttonsChangeText(QString labelText[4]);
};

#endif // BUTTONDIALOG_H
