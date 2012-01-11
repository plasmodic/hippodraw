#include <controllers/DisplayController.h>
#include <datasrcs/NTuple.h>
#include <plotters/PlotterBase.h>
#include <qt/CanvasWindow.h>
#include <datareps/DataRep.h>
#include <qapplication.h>

using namespace hippodraw;

int main ( int argc, char** argv )
{
  QApplication a( argc, argv );

  NTuple* nt = new NTuple("myTestNtuple");
  nt->setTitle("test");
  std::string xLabel = "x";
  std::string yLabel = "y";
  std::vector<std::string> labels;
  labels.push_back(xLabel);
  labels.push_back(yLabel);
  nt->setLabels(labels);

  for (int x = 0; x < 10; x++)
  {
    for (int y = 4; y < 10; y++)
    {
      std::vector<double> row;
      row.push_back((double)(x));
      row.push_back((double)(y));
      nt->addRow(row);
    }
  }

  std::vector < std::string > bindings;
  bindings.push_back ( xLabel );
  bindings.push_back ( yLabel );

  DisplayController * dc_controller = DisplayController::instance ();
  PlotterBase * plotter
//    = dc_controller->createDisplay ( "Scatter Plot", *nt, bindings );
    = dc_controller->createDisplay ( "Profile", *nt, bindings );
  DataRep *rep = plotter->getDataRep(0);
  rep->setRepSize(5.0);
  

  CanvasWindow* cw;
  cw = new CanvasWindow();
  cw->addDisplay(plotter);
  cw->show();

  int result = a.exec();
  return result;

}
