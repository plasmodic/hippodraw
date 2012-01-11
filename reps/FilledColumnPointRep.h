/* -*- mode: c++ -*- */

/** @file

hippodraw::FilledColumnPointRep class interface

Copyright (C) 2006 The Board of Trustees of The Leland Stanford Junior
University.  All Rights Reserved.

$Id: FilledColumnPointRep.h,v 1.4 2006/02/04 22:40:03 pfkeb Exp $

*/

#ifndef _FilledColumnPointRep_H_
#define _FilledColumnPointRep_H_

#include "ColumnPointRep.h"

namespace hippodraw {

/** A PointRep class that draws a filled column to represent the
    value.  The following plot was made using FilledColumnPointRep.

    @image html filledHist.png "Plot with FilledColumnPointRep"
    @image latex filledHist.eps "Plot with FilledColumnPointRep"

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>

    @request Only using solid pattern for the fill for now.  Qt will
    allow us to use other patterns.
*/
class MDL_HIPPOPLOT_API FilledColumnPointRep : public ColumnPointRep
{

  /** The color to use for the fill.
   */
  Color m_fill_color;

  /** Does the drawing based on the accumulated polyline.
   */
  virtual void drawValues ( ViewBase * view ) const;

public:

  /** The default constructor. */
  FilledColumnPointRep();
  
  /** The copy constructor. */
  FilledColumnPointRep( const FilledColumnPointRep & point_rep );

  /** The destructor. */
  ~FilledColumnPointRep();

  /** The clone function returns an object of its own kind which
      is a copy of this object at this moment. */
  virtual RepBase * clone();

  /** Sets the color fill.
   */
  virtual void setColor ( const Color & color );

  /** Get the color fill.
   */
  virtual const Color & getColor () const;

};

} // namespace hippodraw

#endif // _FilledColumnPointRep_H_
