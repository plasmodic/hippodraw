/** @file
 
export hippodraw::PyCanvas class to Python.
 
Copyright (C) 2002, 2004-2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: export_Canvas.cxx,v 1.5 2007/05/23 21:09:31 hurui Exp $

*/

// for dll interface warning
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

// include first to avoid _POSIX_C_SOURCE warning.
#include <boost/python.hpp>

#include "PyCanvas.h"

#include "QtCut.h"

#include "datasrcs/NTuple.h"

using namespace boost::python;

namespace hippodraw {
namespace Python {

void 
export_Canvas()
{
	class_ < PyCanvas >
	  ( "Canvas",
	    "The HippoDraw canvas.\n\n"
	    "This class provides an interface to the canvas.\n"
	    "One can take various actions that are also\n"
	    "available from menu items in the canvas window." )

	  .def ( "show", &PyCanvas::show,
		 "show () -> None\n"
		 "\n"
		 "Displays the canvas window on the screen.   One only needs\n"
		 "to use this method if running HippoDraw in the Python\n"
		 "thread." )

	  .def ( "close", &PyCanvas::close,
		 "close () -> None\n"
		 "\n"
		 "Closes the window." )

           .def ( "addDisplay", &PyCanvas::addDisplay,
		  with_custodian_and_ward < 1, 2 > (),
		  "addDisplay ( Display ) -> None\n"
		  "\n"
		  "Adds a display to the canvas." )

           .def ( "saveAs", &PyCanvas::saveAs,
		  "saveAs ( string ) -> None\n"
		  "\n"
		  "Save the canvas as a XML file." )

 	   .def ( "printTo", &PyCanvas::print,
		  "printTo ( string ) -> None\n"
		  "\n"
		  "Prints the canvas to PostScript file." )

           .def ( "getDisplay", &PyCanvas::getDisplay,
                  return_value_policy < reference_existing_object > (),
		  "getDisplay () -> Display\n"
		  "\n"
		  "Returns the selected Display object." )

           .def ( "getDisplays", &PyCanvas::getDisplays,
                  return_value_policy < copy_const_reference > (),
		  "getDisplays () -> tuple\n"
		  "\n"
		  "Returns a tuple of all Display objects on the canvas." )

           .def ( "getCut", &PyCanvas::getCut,
                  return_value_policy < reference_existing_object > (),
		  "getCut () -> Cut\n"
		  "\n"
		  "Returns the currently selected Cut object." )

           .def ( "selectAllDisplays", &PyCanvas::selectAllDisplays,
		  "selectAllDisplays ( Boolean ) -> None\n"
		  "\n"
		  "Sets all displays to selected state or not." )

           .def ( "selectDisplay", &PyCanvas::selectDisplay,
		  "selectDisplay ( Display ) -> None\n"
		  "\n"
		  "Sets a display to selected state." )

           .def ( "saveAsImage", &PyCanvas::saveAsImage,
		  "saveAsImage ( Display, string ) -> None\n"
		  "\n"
		  "Save a display as an image file.\n"
		  "The suffix of the file name controls the image type." )

           .def ( "saveSelectedImages", &PyCanvas::saveSelectedImages,
		  "saveSelectedImages ( string ) -> None\n"
		  "\n"
                  "Save the selected displays as an image file.\n"
		  "The suffix of the file name controls the image type." )

           .def ( "removeDisplay", &PyCanvas::removeDisplay,
		  "removeDisplay ( Display ) -> None\n"
		  "\n"
		  "Removes the display from the canvas." )

           .def ( "addText", &PyCanvas::addText,
		  "addText ( Display, string ) -> None\n"
		  "\n"
		  "Adds text to display." )

           .def ( "addTextAt", &PyCanvas::addTextAt,
		  "addTextAt ( Display, string, value, value ) -> None\n"
		  "\n"
		  "Adds text to a display at specified position\n"
		  "Position is fraction of width and height." )

           .def ( "addTextAtAbs", &PyCanvas::addTextAtAbs,
		  "addTextAt ( Display, string, value, value ) -> None\n"
		  "\n"
		  "Adds text to a display at specified position\n"
		  "Position is absolute value of the date point." )

	  .def ( "addTextRep", &PyCanvas::addTextRep,
		 "addTextRep ( Display, string ) -> None\n"
		 "\n"
		 "Adds textual data representation to display.  Use\n"
		 "Canvas.getTextRepTypes() to see available types." )

	  .def ( "setPlotMatrix", &PyCanvas::setPlotMatrix,
		 "setPlotMatrix ( columns, rows ) -> None\n"
		 "\n"
		 "Sets the number of columns and "
		 "rows of plots for each page." )

	  .def ( "swapOrientation", &PyCanvas::swapOrientation,
		 "swapOrientation ( ) -> None\n"
		 "\n"
		 "Swaps the canvas' orientation from portrait to landscape\n"
		 "or vice verse." )

	  .def ( "getTextRepTypes", &PyCanvas::getTextRepTypes,
		 return_value_policy < copy_const_reference > (),
		 "getTextRepTypes () -> tuple\n"
		 "\n"
		 "Returns the types of textual data representations"
		 " available." )

	  .def ( "clear", &PyCanvas::clear,
		 "clear () -> None\n"
		 "\n"
		 "Removes all items from the canvas" )
	  
	  .def ( "getX", &PyCanvas::getX,
		 "getX ( Display ) -> value\n"
		 "\n"
		 "Returns the X coordinate of the Display." )

	  .def ( "getY", &PyCanvas::getY,
		 "getY ( Display ) -> value\n"
		 "\n"
		 "Returns the Y coordinate of the Display.  Note that Y=0 is\n"
		 "at the top and Y increases downward." )

	  .def ( "setX", &PyCanvas::setX,
		 "setX ( Display, value ) -> None\n"
		 "\n"
		 "Sets the X coordinate of the Display" )

	  .def ( "setY", &PyCanvas::setY,
		 "setY ( Display, value ) -> None\n"
		 "\n"
		 "Sets the Y coordinate of the Display.   Note that Y = 0 is\n"
		 "at the top and Y increases downward." )

	  .def ( "getHeight", &PyCanvas::getHeight,
		 "getHeight ( Display ) -> value\n"
		 "\n"
		 "Returns the height of the Display." )

	  .def ( "getWidth", &PyCanvas::getWidth,
		 "getWidth ( Display ) -> value\n"
		 "\n"
		 "Returns the width of the Display." )

	  .def ( "setHeight", &PyCanvas::setHeight,
		 "setHeight ( Display, value ) -> None\n"
		 "\n"
		 "Sets the height of the Display." )

	  .def ( "setWidth", &PyCanvas::setWidth,
		 "setWidth ( Display, value ) -> None\n"
		 "\n"
		 "Sets the width of the Display." )
	 
	  .def ( "getSelPickTable", &PyCanvas::getSelPickTable,
		 return_value_policy < reference_existing_object > (),
		 "getSelPickTable () -> NTuple\n"
		 "\n"
		 "Gets the pick table for selected Display." )

	  .def ( "getPickTable", &PyCanvas::getPickTable,
		 return_value_policy < reference_existing_object > (),
		 "getPickTable ( Display ) -> NTuple\n"
		 "\n"
		 "Gets the pick table of the Display." )

           ;
}

} // namespace Python
} // namespace hippodraw
