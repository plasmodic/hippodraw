/* -*- mode: c++ -*- */

/** @file

hippodraw::BinToColor class interface

Copyright (C) 2000, 2003-2005  The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: BinToColor.h,v 1.37 2007/01/30 22:49:58 pfkeb Exp $

*/

#ifndef _BinToColor_H_
#define _BinToColor_H_

#include "axes/Range.h"
#include "graphics/Color.h"

namespace hippodraw {

/** The base class for the value to the color transformation. It is
    needed, for example, for the drawing of color plots when the color
    of a point depends on its value.  

    @author Stephane Bonneaud <gandalf@slac.stanford.edu> 
    @author Tao Xu <taox@stanford.edu>
*/

class MDL_HIPPOPLOT_API BinToColor
{

 protected:

  /** The name of this value transform. */
  std::string m_name;

  /** A set of control points that might be used for the value to
      color transform.  

      @note This vector is declared in the base class so that the
      implementation of getControlPoints in this base class can return
      an empty vector, otherwise some compilers would flag an error
      for not returning anything.
  */
  std::vector < double > m_control_points;

  /** The distance of the expected values, i.e. maximum - minimum.
   */
  double m_dv;

  /** The minimum value expected.
   */
  double  m_vmin;

  /** The constructor.
   */
  BinToColor ( const char * name );
 
  /** Another constructor
   */
  BinToColor ( const std::string &);

 public:

  /** The copy constructor. */
  BinToColor ( const BinToColor & bin_to_color );

  /** The destructor. */
  virtual ~BinToColor();

  /** Creates a copy of itself. */
  virtual BinToColor * clone () const = 0;

  /** Returns the name of the value to color transform.
   */
  const std::string & name () const;

  /** Sets the range of the values passed. */
  void setRange( const Range & r );

  /** Get the Range of expected values.
   */
  Range getRange () const;

  /** This function does the actual calculation that transforms the
      given value into a color using the current Range.
  */
  virtual void doubleToColor ( double value, Color & color ) const = 0;

  /** Returns @c true if class has control points by which the value
      to color transform can be varied.  The implementation in this
      base class returns @c false.
  */
  virtual bool hasControlPoints () const;

  /** Returns a list of control points.  If a derived class has
      control points to vary the transform of value to color returns
      the list of control points.  Otherwise, the returned list is
      empty.
  */
  const std::vector < double > & getControlPoints () const;

  /** Set the value of the control points.  Interpretation of control
      points is left to derived classes.
  */
  virtual void setControlPoints ( const std::vector < double > & points );

  /** Returns @c true if object can be user defined.  Returns @c true
      if value to color transform can be user defined.  That is, the
      control points for the transform are editable.  This base class
      implementation returns @c false.
   */
  virtual bool isUserDefined () const;

  /** Returns @c true if object can accept changing color,i.e. enable
      the "Change color" button in plot inspector tabbed panel within
      inspector.  Reimplemented in BinToBlack and BinToGreyScale. The
      base class implementation returns @c false.
  */
  virtual bool acceptChangeColor () const;

};

} // namespace hippodraw

#endif // _BinToColor_H_
