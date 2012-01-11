/* -*- mode: c++ -*- */

/** @file

hippodraw::DataView class interface

Copyright (C) 2005, 2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: DataView.h,v 1.9 2007/03/05 22:49:48 xiefang Exp $

*/

#ifndef _DataView_H_
#define _DataView_H_

#include "ViewBase.h"



namespace hippodraw {

/** The abstract base class for views that have a region for drawing
    data points with area around it for drawing axis labels, titles,
    etc.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/

class MDL_HIPPOPLOT_API DataView : virtual public ViewBase
{

private:

  /** The copy constructor. */
  DataView ( const DataView & );

protected:

  /** The default constructor. */
  DataView();

  /** The rectangle area in which the projected points are plotted.
      The coordinate system is that of the view device.  X increase to
      the right and Y increases downward.  Outside of this area are
      the axis labels, title, etc.
  */
  Rect    m_margin_rect; 

  /** The following constructor takes a plotter as an argument to use
      with a drawing.  */
  DataView ( PlotterBase * plotter );

  /** The virtual destructor. */
  virtual ~DataView();

  /** Prepares the margin rectangle.
   */
  void prepareMarginRect ( );

  /** Converts the user X coordinate into the margin X coordinate.
   */
  virtual double userToMarginX ( double x ) const;

  /** Converts the user X coordinate into the inverted X coordinate.
   */
  virtual double userToInvertedMarginX ( double x ) const;

  /** Converts the user Y coordinate into the margin Y coordinate.
   */
  virtual double userToMarginY ( double y ) const;

  /** Converts the user Y coordinate into the top-left based margin Y
      coordinate.
  */
  virtual double userToInvertedMarginY ( double y ) const;

  /** Converts the user Z coordinate into the margin color (X) coordinate.
   */
  virtual double userToMarginColor ( double c ) const;


  /** Converts from view coordinate to data coordinate, without taking
      into account transforms or scaling. 
  */
  virtual double marginToUserX ( double x ) const;

  /** Converts from view coordinate to data conordiate, take into account 
      the inversion of x axes.
  */
  virtual double marginToInvertedUserX ( double x ) const;

  /** Converts from view coordinate to data coordinate, without taking
      into account transforms or scaling.
  */
  virtual double marginToUserY ( double y ) const;

  /** Converts from inverted view coordinate to data coordinate,
      without taking into account transforms or scaling.
  */
  virtual double marginToInvertedUserY ( double y ) const;


 public:

  /** Returns the rectangle area in which data points are drawn.
   */
  const Rect & getMarginRect () const;

  /** Sets the margin rectangle. Sets the rectangular are in which the
      data points are drawn.  The coordinate system is that of the
      view.
  */
  void setMarginRect ( const Rect & );

};

} // namespace hippodraw

#endif // _DataView_H_
