#include <QtGui/QApplication>
#include "ui/startwindow.h"

// Обработка запуска приложения.
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // Инициализация стартого окна.
    StartWindow sw;
    sw.show();
    return a.exec();
}
