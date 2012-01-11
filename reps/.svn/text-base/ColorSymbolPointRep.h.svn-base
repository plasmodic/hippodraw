/* -*- mode: c++ -*- */

/** @file

ColorSymbolPointRep class interface

Copyright (C) 2001, 2003-2005   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: ColorSymbolPointRep.h,v 1.25 2005/10/30 00:10:17 pfkeb Exp $

*/

#ifndef _ColorSymbolPointRep_H_
#define _ColorSymbolPointRep_H_

#include "SymbolPointRep.h"

namespace hippodraw {

class BinToColor;

/** A PointRep class draws a symbol to represent the coordinate in a
    color that represents the value.

    @todo Unlike the class it inherits from, this class doesn't
    currently support the drawing of error bars.  Or, maybe a class
    that can draw error bars should be a derived class of one that
    only draws symbols.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/
class MDL_HIPPOPLOT_API ColorSymbolPointRep : public SymbolPointRep
{
 private:

  /** A cache of the colors to be used.
   */
  std::vector < Color > m_colors;

  /** The color scale generator. */
  BinToColor * m_color_gen;

 public:

  /** The following constructor takes the symbol and size as
      arguments. */
  ColorSymbolPointRep ( hippodraw::Symbol::Type symbol, float size );

  /** The default constructor. */
  ColorSymbolPointRep ( );

  /** The copy constructor. */
  ColorSymbolPointRep( const ColorSymbolPointRep & point_rep );

  /** The destructor. */
  ~ColorSymbolPointRep();

  /** The clone function returns an object of its own kind which
      is a copy of this object at this moment. */
  virtual RepBase * clone();

  virtual const BinToColor * getValueTransform ( ) const;
  virtual void setValueTransform ( BinToColor * );
  virtual void drawProjectedValues ( const DataSource * ntuple,
				     TransformBase * transform,
				     ViewBase * view );

  /** Returns @c false as this classes uses color for a Z value.
   */
  virtual bool uses ( Color::Value ) const;

};

} // namespace hippodraw

#endif // _ColorSymbolPointRep_H_
