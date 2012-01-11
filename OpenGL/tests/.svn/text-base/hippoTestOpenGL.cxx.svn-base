/*
 * $Id: hippoTestOpenGL.cxx,v 1.5 2006/09/15 18:08:18 pfkeb Exp $
 *
 */

#include "controllers/DisplayController.h"
#include "plotters/PlotterBase.h"
#include "datasrcs/NTupleController.h"
#include "datasrcs/NTuple.h"

#include "OpenGL/OpenGLApp.h"
#include "OpenGL/OpenGLWindow.h"

#include <iostream>

using namespace hippodraw;

int main ( int /*argc*/, char** /*argv*/)
{
  OpenGLApp openGLApp;
  if(!openGLApp.initialize()) return 0;

  NTupleController* ntupleController = NTupleController::instance();
  const std::string filename( "aptuple.tnt" );
  DataSource* ntuple = ntupleController->createNTuple(filename);
  if(!ntuple) {
    std::cout << "Can't open " << filename << std::endl;
    return 0;
  }

  DisplayController* displayController = DisplayController::instance();


/*
 {// First view :
  std::vector<std::string> bindings;
  bindings.push_back("Category");
  //bindings.push_back("Age");
  PlotterBase* plotter = displayController->createDisplay("Histogram",
							  *ntuple,
							  bindings);
  //plotter->autoScale();
  //plotter->setRange("x",0.0,30000.);
  OpenGLWindow* view = new OpenGLWindow(openGLApp.getXDisplay(),
				        openGLApp.getXColormap(),
			                openGLApp.getXVisualInfo(),
			                openGLApp.getGLXContext());

  view->setPlotter(plotter);
  openGLApp.fViews.push_back(view);}
*/

 {// Second view :
  std::vector<std::string> bindings;
  bindings.push_back("Category");
  //bindings.push_back("Age");
  bindings.push_back("Cost");
  PlotterBase* plotter = displayController->createDisplay("Contour Plot", 
							  *ntuple, 
							  bindings);
  //plotter->autoScale();
  //plotter->setRange("x",0.0,30000.);
  OpenGLWindow* view = new OpenGLWindow(openGLApp.getXDisplay(),
				        openGLApp.getXColormap(),
			                openGLApp.getXVisualInfo(),
			                openGLApp.getGLXContext());

  view->setPlotter(plotter);
  openGLApp.fViews.push_back(view);}

  int status = openGLApp.exec();

  //FIXME
  //delete plotter;
  //delete view;

  return status;
}
