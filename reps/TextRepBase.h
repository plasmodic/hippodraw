/* -*- mode: c++ -*- */

/** @file
 
hippodraw::TextRepBase class interface

Copyright (C) 2001, 2003-2004, 2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: TextRepBase.h,v 1.31 2007/06/08 20:40:52 pfkeb Exp $

*/

#ifndef _TextRepBase_H_
#define _TextRepBase_H_

#include "RepBase.h"

namespace hippodraw {

/** A base class for representations that draws a value as text.

    @bug @@@@@@ On Mac OS X, he bounding box is not big enough.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/
class MDL_HIPPOPLOT_API TextRepBase : public RepBase
{

protected:

  /** The following constructor takes the name and size as
      arguments. */
  TextRepBase ( const char * name );

public:

  /** Returns a copy of the object. */
  virtual RepBase * clone () = 0;

  /** Draws the values from the @a projector to the @a view. 
   */
  virtual void drawProjectedValues ( ProjectorBase & projector,
				     ViewBase & view ) = 0;

/** As a convenience for implementors of derived classes of this class,
    the implementation here does nothing but satisfy the pure virtual
    function in the base class. @sa RepBase::drawProjectedValues for
    details of what might be done. 
*/
  virtual void drawProjectedValues ( const DataSource * ntuple,
				     TransformBase * transform,
				     ViewBase * view );

};

} // namespace hippodraw

#endif // _TextRepBase_H_
