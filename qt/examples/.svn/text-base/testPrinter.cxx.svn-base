#include <qapplication.h>
#include <qpixmap.h>
#include <qpainter.h>
#include <qprinter.h>

/* Program supplied by Trolltech support to test if printing is work
   with the installed version of Qt on the client operating system.
   For example, for Qt 3.3.4 and Red Hat Enterprise Linux 3, printing
   was broken.
*/

int main(int argc, char **argv)
{
    QApplication a(argc, argv);
    QPixmap pm(40, 40);
    pm.fill(Qt::white);
    {
        QPainter p(&pm);
        p.drawLine(0, 0, 40, 40);
    }
    QPrinter print;
    if (print.setup()) {
        QPainter p(&print);
        p.drawPixmap(QPoint(100, 100), pm);
        p.drawText(QPoint(200, 200), "This is some text" );

    }
    return 0;
}

