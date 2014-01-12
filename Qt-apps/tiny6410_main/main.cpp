#include "tiny6410dialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    tiny6410Dialog w;
    w.show();
    
    return a.exec();
}
