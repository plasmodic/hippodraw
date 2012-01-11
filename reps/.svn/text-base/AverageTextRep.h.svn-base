/* -*- mode: c++ -*- */

/** @file

hippodraw::AverageTextRep class interface

Copyright (C) 2001-2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: AverageTextRep.h,v 1.17 2006/09/27 18:18:54 pfkeb Exp $

*/

#ifndef _AverageTextRep_H_
#define _AverageTextRep_H_

#include "TextRepBase.h"

namespace hippodraw {

/** A TextRepBase class that draws the projected values as a
    calculated average.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
 */

class MDL_HIPPOPLOT_API AverageTextRep : public TextRepBase
{

private:

  /** The axis for which the average is calculated. */
  hippodraw::Axes::Type m_axis;

public:

  /** The default constructor. */
  AverageTextRep ( );

  /** Constructor that takes Axis Type. */
  AverageTextRep ( hippodraw::Axes::Type axis );

  /** The copy constructor. */
  AverageTextRep ( const AverageTextRep & point_rep );

  virtual RepBase * clone ();

  virtual void drawProjectedValues ( ProjectorBase & projector,
				     ViewBase & view );
};

} // namespace hippodraw

#endif // _AverageTextRep_H_
