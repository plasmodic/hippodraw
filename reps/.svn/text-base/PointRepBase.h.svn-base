/* -*- mode: c++ -*- */

/** @file

hippodraw::PointRepBase class interface

Copyright (C) 2000-2003, 2005, 2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: PointRepBase.h,v 1.50 2006/02/12 20:51:35 pfkeb Exp $

*/

#ifndef _PointRepBase_H_
#define _PointRepBase_H_

#include "reps/RepBase.h"

namespace hippodraw {

class Range;
class ViewBase;
class ErrorBarRep;

/** The base class for the point representation hierarchy. The
    coordinate system used by these classes is that of the graphic
    device.  The coordinates are maintained as floats in order to
    interface to Postscript properly. 

    @todo This class is too big for the base class.  For example,
    TextRepBase is implementing functions it doesn't have to. Should
    have a RepBase class.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/

class MDL_HIPPOPLOT_API PointRepBase : public RepBase
{

protected:

  /** The error bar representation. Will be a null pointer if a derived
      class doesn't have one. */
  ErrorBarRep * m_error_rep;

 public:

  /** A constructor with the @a name of the representation and its @a
      size. */
  PointRepBase ( const char * name, float size );

  /** The copy constructor. */
  PointRepBase( const PointRepBase & point_rep );

  /** The clone function returns an object of its own kind which
      is a copy of this object at this moment. */
  virtual RepBase * clone() = 0;

  /** Sets the selection state of the representation.  Also sets the
      selection state of the ErrorBarRep representation, if any.  If
      @a yes is @c false, the point representation should shows its
      deselected stated by some fashion, typically using a light gray
      color instead of its normal color.
  */
  virtual void setSelected ( bool yes );

};

} // namespace hippodraw

#endif // _PointRepBase_H_
