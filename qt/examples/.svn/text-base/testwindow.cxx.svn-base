#include <qapplication.h>
#include "QtViewWidgetWindow.h"

using namespace hippodraw;

int main( int argc, char ** argv )
{
    QApplication a( argc, argv );
    QtViewWidgetWindow w;
    w.show();
    a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );

    return a.exec();
}
