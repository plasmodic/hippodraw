/* -*- mode: c++ -*- */

/** @file

hippodraw::BinToColorMap class interface

Copyright (C) 2000, 2003-2005  The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: BinToColorMap.h,v 1.2 2007/01/30 22:49:59 pfkeb Exp $

*/

#ifndef _BinToColorMap_H_
#define _BinToColorMap_H_

#include "BinToColor.h"

namespace hippodraw {

/** The base class for the value to the color transformation. It is
    needed, for example, for the drawing of color plots when the color
    of a point depends on its value.  

    @author Stephane Bonneaud <gandalf@slac.stanford.edu> 
    @author Tao Xu <taox@stanford.edu>
*/

  class MDL_HIPPOPLOT_API BinToColorMap : public BinToColor
{

private:

  /** The gamma correction factor. */
  double m_gamma;

  /** Calculate the gamma factor based on current control points.
   */
  void calcGamma ( );

 protected:
  
  /** vector of reds for color map. */
  std::vector < int > m_reds;

  /** vector of greens for color map. */
  std::vector < int > m_greens;

  /** vector of blues for color map. */
  std::vector < int > m_blues;

  /** The constructor.
   */
  BinToColorMap ( const char * name );
 
  /** Another constructor
   */
  BinToColorMap ( const std::string &);

 public:

  /** The copy constructor. */
  BinToColorMap ( const BinToColorMap & bin_to_color );

  /** The destructor. */
  virtual ~BinToColorMap();

  /** Creates a copy of itself. */
  virtual BinToColor * clone () const = 0;

  /** Retruns @c true. */
  bool hasControlPoints ( ) const;

  virtual void setControlPoints ( const std::vector < double> & points );

  /** Returns the name of the value to color transform.
   */
  const std::string & name () const;

  /** This function does the actual calculation that transforms the
      given value into a color using the current Range.
  */
  virtual void doubleToColor ( double value, Color & color ) const;


};

} // namespace hippodraw

#endif // _BinToColor_H_
