#include <datareps/St1DHistogram.h>
#include <qt/QtViewWidget.h>
#include <plotters/XyPlotter.h>
#include <qapplication.h>
#include <iostream>

using namespace hippodraw;

/* Test program from  Giulio Eulisse <giulio.eulisse@cern.ch> */
int
main (int argc, char **argv)
{
     QApplication *app = new QApplication (argc, argv);
     DataRep *rep = new St1DHistogram ();
     std::vector<double> values(10);
     for (int i = 0; i < 10 ; i++)
     {
         values[i] = i;
     }
     rep->addValues (values);
     PlotterBase *plot = new XyPlotter ();

     std::cerr << plot->hasNTupleBindings () << std::endl;
     plot->addDataRep (rep);
     QtViewWidget *widget = new QtViewWidget ();
     widget->setPlotter (plot);
     widget->show();
     app->setMainWidget (widget);
     app->exec ();
}

