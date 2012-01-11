/* -*- mode:c++ -*- */

/** @file

DrawBorder class interface

Copyright (C)  2001  The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: DrawBorder.h,v 1.5 2005/10/30 00:10:15 pfkeb Exp $

*/

#ifndef _DrawBroder_h
#define _DrawBroder_h
namespace hippodraw {

class ViewBase;

/** A singleton class to draw the border with knobies around the
    selected display. */

class DrawBorder
{
private:

  /** The instance of the application. */
  static DrawBorder * s_instance;

  /** A private copy constructor to avoid copying. */
  DrawBorder ( const DrawBorder & );

  /** The View object. */
  ViewBase * m_view;

  /** A default constructor for avoiding creation except by itself. */
  DrawBorder();

 public:

  /** Returns the pointer to the singleton instance. */
  static DrawBorder * instance ();

  /** Sets the View object. */
  void setView (ViewBase * view);

  /** Gets the view object. */
  ViewBase * getView ();

  /** Does the actual drawing in the view. */
  void draw();

  /** Draws a knob centered at x, y. */
  void drawKnob ( double x, double y );

};

} // namespace hippodraw

#endif // _DrawBroder_h
