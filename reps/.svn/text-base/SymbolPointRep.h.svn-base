/* -*- mode: c++ -*- */

/** @file

hippodraw::SymbolPointRep class interface

Copyright (C) 2000, 2001, 2003-2006   The Board of Trustees of The
Leland Stanford Junior University.  All Rights Reserved.

$Id: SymbolPointRep.h,v 1.87 2007/03/14 23:28:28 xiefang Exp $

*/

#ifndef _SymbolPointRep_H_
#define _SymbolPointRep_H_

#include "PointRepBase.h"

#include "axes/Range.h"

namespace hippodraw {

class BinaryTransform;
class PeriodicBinaryTransform;

/** A PointRep class draws a symbol to represent the value.  The
    variety of symbols available is enumerated on in
    hippodraw::Symbol::Type. The following plot was made with
    SymbolPointRep.

    @bug The triangle is upside down, or should have both types.

    @bug A symbol can be drawn outside the drawing rectangle if for
    example, it is located at 0,0.

    @todo Could add diamond.

    @image html xyplot.png "Plot with SymbolPointRep"
    @image latex xyplot.eps "Plot with SymbolPointRep"

    @author Kaustuv <kaustuv@stanford.edu>
    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/
class MDL_HIPPOPLOT_API SymbolPointRep : public PointRepBase
{

protected:

  /** A cache of the points to be drawn on the X axis.  A cache is
      used so that all the points are delivered to the View with one
      call.  The cache is cleared with a call to #beginPlot.  However,
      X and Y caches are separate so that they can be transformed
      independently.
  */
  std::vector < double > m_x;

  /** A cache of the points to be drawn on the Y axis.  @sa #m_x */
  std::vector< double > m_y;

  /** The x error flag. If set @c true, error bars along the X axis
      are generated. */
  bool m_x_flag;

  /** The y error flag. If set @c true, error bars along the Y axis
      are generated. */
  bool m_y_flag;

 private: 

  /** Cached dynamic cast of BinaryTransform to
      PeriodicBinaryTransform if the %BinaryTransform is periodic. 
  */
  const PeriodicBinaryTransform * c_periodic_tf;

  /** Cached Range on X axis. */
  Range c_x_range;

  /** Cached Range on X axis. */
  Range c_y_range;

  /** The X error bar cap length. */
  double m_x_cap;

  /** The Y error bar cap length. */
  double m_y_cap;

 protected:

  /** The @c PlotSymbol currently used. */
  hippodraw::Symbol::Type  m_plot_symbol;

public:

  /** The following constructor takes the symbol and size as
      arguments. */
  SymbolPointRep ( hippodraw::Symbol::Type symbol, float size );

  /** The default constructor. */
  SymbolPointRep ( );

  /** The copy constructor. */
  SymbolPointRep ( const SymbolPointRep & point_rep );

  /** The destructor. */
  ~SymbolPointRep();

  /** The clone function returns an object of its own kind which
      is a copy of this object at this moment. */
  virtual RepBase * clone ();

  virtual void setColor ( const Color & color );

  /** Sets the size of the line drawing and that of the error bar.
   */
  virtual void setSize ( float value );

  virtual void setStyle ( unsigned int symbol );

  /** Returns the style of the point symbol.
   */
  virtual unsigned int getStyle ( ) const;

  virtual bool xError ( ) const;

  virtual bool yError ( ) const;

  /** Sets the display of the error bar.  Sets the display of the
      error bar on the specified @a axis to on or off. 
  */
  virtual void setErrorOn ( hippodraw::Axes::Type axis, bool yes = true );

  /** Transforms the cached coordinates using @a transform. */
  virtual void transformPoints ( const TransformBase & transform, 
				 ViewBase & view );

  virtual void drawProjectedValues ( const DataSource * ntuple,
				     TransformBase * transform,
				     ViewBase * view );


protected:

  /** Sets the value of cached data members.  This member function
      should be called before any projected values are transformed.
   */
  void beginPlot ( unsigned int size, 
		   const BinaryTransform * tf, 
		   const ViewBase * view );

  /** Draws a projected value.  Draws the projected value index by @a
      index if the coordinate is within range.  Returns @c true if the
      point is drawn, otherwise returns @c false.

      @attention The member function beginPlot must be called before a
      call this member function.
  */
  bool drawProjectedValue ( unsigned int i, 
			    const DataSource * ntuple, 
			    const BinaryTransform * transform,
			    ViewBase * view );

  /** Returns @c true as this class uses hippodraw::Symbol::Type as a
      property.
  */
  virtual bool uses ( hippodraw::Symbol::Type ) const;

};

} // namespace hippodraw

#endif // _SymbolPointRep_H_
