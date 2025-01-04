#include "campuscard.h"
#include <QApplication>

#include "checkfiledialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CampusCard w;
    w.show();
    return a.exec();
}
