/* -*- mode: c++ -*- */

/** @file

hippodraw::PyCanvas class interface.
 
Copyright (C) 2002-2006   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: PyCanvas.h,v 1.50 2007/05/23 21:09:31 hurui Exp $

*/

#ifndef PyCanvas_H
#define PyCanvas_H

#include <string>
#include <vector>

namespace hippodraw {

  class CanvasViewProxy;
  class CanvasWindow;
  class QtDisplay;
  class QtCut;
  class QtView;
  class NTuple;

/** This class is the public interface the what the user sees as the
    @em canvas object from Python.  

    @request Be able to retrieve, from Python, information on active
    displays on the canvas such as a display identifier, axis labels,
    etc ...

    @request @@@@ Add interface to re-tile.

    @request @@@@ Get a Display by its plotter id.

    @author James Chiang <jchiang@slac.stanford.edu>
    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/
class PyCanvas
{

private:

  /** The actual canvas window in the application thread.
      @deprecated Should use the CanvasViewProxy instead.
   */
  CanvasWindow * m_canvas;

  /** The proxy for the CanvasView object.  All messages to the
      CanvasView should go through the proxy so that it is thread
      safe.
   */
  CanvasViewProxy * m_canvas_proxy;

  /** Set to @c true if this canvas as associated GUI CanvasWindow.
   */
  bool m_has_gui;

   /** Pointers to QtDisplay objects on the current canvas. */
   mutable std::vector<QtDisplay *> m_displays;

  /** Checks if the CanvasWindow has been closed and throws a
      runtime_error if it has been.
  */
  void check () const;

   /** Return the QtView associated with the given QtDisplay */
   QtView * findSelectedView ( QtDisplay * display );

public:

  /** A constructor taking as argument the real canvas used by this
      object. */
  PyCanvas ( CanvasWindow * );

  /** The default constructor. */
  PyCanvas ();

  /** Displays the canvas window on the screen.
   */
  void show ();

  /** Closes the canvas window.
   */
  void close ();

  /** Adds the display to the canvas. 
   */
  void addDisplay ( QtDisplay * display_wrap );

  /** Saves the document to the specified file.
   */
  void saveAs ( const std::string & filename );

   /** Returns the selected display on the canvas.  If none or more
       than one display is selected, this routine returns a null
       pointer */
   QtDisplay * getDisplay ();

   /** Returns all displays on the canvas. */
   const std::vector<QtDisplay *> & getDisplays () const;

   /** Returns a QtCut object or null pointer if its not a QtCut
       object. */
   QtCut * getCut();

   /** Select or un-select all the displays on the canvas. */
   void selectAllDisplays ( bool flag = true );

   /** Select a specific display. */
   void selectDisplay ( QtDisplay * display );

  /** Prints the canvas to a PostScript file.
   */
  void print ( const std::string & filename );

   /** Save the display as an image file. */
   void saveAsImage( QtDisplay * display, const std::string &filename );

   /** Save the selected images as an image file */
   void saveSelectedImages(const std::string & filename);

   /** Remove a display. */
   void removeDisplay ( QtDisplay * display );

  /** Returns the types of textual data representations available.
   */
  const std::vector < std::string > & getTextRepTypes () const;

  /** Adds a textual data representation to @a display or type @a
      type.
  */
  void addTextRep ( QtDisplay * display, const std::string & type );

   /** Add a BoxTextRep. */
   void addText( QtDisplay * display, const std::string &text );

   /** Add a BoxTextRep at a specific location in the selected display
       item's canvas coordinate system.
   */
   void addTextAt ( QtDisplay * display, const std::string &text,
                    double xrel, double yrel );

   /** Add a BoxTextRep at a specific location in the selected display
       item's canvas coordinate system.
   */
   void addTextAtAbs ( QtDisplay * display, const std::string &text,
                    double xabs, double yabs );

  /** Sets the number for @a columns and @a rows of plots on each
      page.
  */
  void setPlotMatrix ( unsigned int columns, unsigned int rows );

  /** Swaps the orientation from portrait to landscape and vice verse.
   */
  void swapOrientation ();

  /** Removes all items from the CanvasWindow.
   */
  void clear ();

  /** Returns the view's X coordinate for the @a display.
   */
  double getX ( QtDisplay * display ) const;

  /** Returns the view's Y coordinate for the @a display.
   */
  double getY ( QtDisplay * display ) const;

  /** Sets the view's X coordinate for the @a display.
   */
  void setX ( QtDisplay * display, double value );

  /** Sets the view's Y coordinate for the @a display.
   */
  void setY ( QtDisplay * display, double value );

  /** Returns the view's height for the @a display.
   */
  double getHeight ( QtDisplay * display ) const;

  /** Returns the view's width for the @a display.
   */
  double getWidth ( QtDisplay * display ) const;

  /** Sets the height of the view for the @a display.
   */
  void setHeight ( QtDisplay *, double h );

  /** Sets the width of the view for the @a display.
   */
  void setWidth ( QtDisplay *, double w );

  /** Gets the PickTable of selected @a display as a NTuple.
   */
  NTuple * getSelPickTable ();

  /** Gets the PickTable for the @a display.
   */
  NTuple * getPickTable ( QtDisplay * );

};

} // namespace hippodraw

#endif // PyCanvas_H
