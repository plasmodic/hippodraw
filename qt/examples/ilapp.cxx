#include "QtApp.h"
#include "qt/QtViewWidget.h"
#include "qt/CanvasWindow.h"

#include "controllers/DisplayController.h"
#include "datasrcs/NTupleController.h"
#include "datasrcs/NTuple.h"
#include "plotters/PlotterBase.h"

#include <qapplication.h>

#include <iostream>
using std::cout;
using std::endl;

#include <string>
#include <vector>

using std::string;
using std::vector;

using namespace hippodraw;

int main(int argc, char *argv[])
{
  cout << "main argc = " << argc
       << endl;
  QtApp app(argc, argv );
  cout << "main got app  argc= " << argc
       << endl;

//   app.setFirstWindow();

  //create data
  int const N = 10;
  double tmp_x[N] = {90.74, 91.06, 91.43, 91.50, 92.16, 92.22, 92.96, 89.24, 89.98, 90.35};
  double tmp_y[N] = {29.0, 30.0,  28.40, 28.80, 21.95, 22.90, 13.50,  4.50, 10.80, 24.20};
  vector<double> vect_x;
  vector<double> vect_y;
  for (unsigned idx = 0; idx < N; idx++) {
    vect_x.push_back(tmp_x[idx]);
    vect_y.push_back(tmp_y[idx]);
  }

  NTupleController *nt_controller = NTupleController::instance();
  NTuple *nt = new NTuple;
  nt->addColumn("x", vect_x);
  nt->addColumn("y", vect_y);
  nt_controller->registerNTuple(nt);

  const string histo ( "XY Plot" );
  vector < string > bindings;
  bindings.push_back ( "x" );
  bindings.push_back ( "y" );

  DisplayController * dc_controller = DisplayController::instance ();
  PlotterBase * plotter 
    = dc_controller->createDisplay ( histo, *nt, bindings );
  cout << "created plotter"
       << endl;
  CanvasWindow *window = app.currentCanvas();
  cout << "got window at " << window
       << endl;
  window->addDisplay(plotter);
  return app.exec();
}
