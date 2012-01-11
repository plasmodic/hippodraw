/** @file

Example program.  Has .cc suffix to avoid Doxygen conflict with other
main program.
*/

#include "qt/QtViewWidget.h"

#include "controllers/DisplayController.h"
#include "datasrcs/NTupleController.h"
#include "datasrcs/NTuple.h"
#include "plotters/PlotterBase.h"

#include <qapplication.h>

#include <string>
#include <vector>

using std::string;
using std::vector;


/** Example program.  The program is an example and test C++ program
    for the QtViewWidget.  A similiar program written in Python is the
    file `sip/testwidget.py.in'.   

    @bug The path to the file is hardwired.  
*/

using namespace hippodraw;

int main ( int argc, char **argv )
{
    QApplication a( argc, argv );

    const string filename ( "../../../hippodraw/examples/aptuple.tnt" );
    NTupleController * nt_controller = NTupleController::instance ();
    DataSource * nt 
      = nt_controller->createNTuple ( filename  );

    const string histo ( "Histogram" );
    vector < string > bindings;
    bindings.push_back ( "Cost" );

    DisplayController * dc_controller = DisplayController::instance ();
    PlotterBase * plotter 
      = dc_controller->createDisplay ( histo, *nt, bindings );

    QtViewWidget * view = new QtViewWidget ( );
    view->setPlotter ( plotter );

    view->resize ( 200, 200 );
    a.setMainWidget( view );
    view->setCaption ( "Qt HippoDraw - View widget" );
    view->show();

    int result = a.exec();

    delete view;
    delete nt;

    return result;
}
