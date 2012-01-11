/* -*- mode: c++ -*- */

/** @file

FuncParmRep class interface

Copyright (C) 2001-2004   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: FuncParmRep.h,v 1.19 2005/10/30 00:10:17 pfkeb Exp $

*/

#ifndef _FuncParmRep_H_
#define _FuncParmRep_H_

#include "TextRepBase.h"

namespace hippodraw {

class FunctionBase;

/** A Representation that draws the values of function parameters. 

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/

class MDL_HIPPOPLOT_API FuncParmRep : public TextRepBase
{

public:

  /** The default constructor. */
  FuncParmRep ( );

  /** The copy constructor. */
  FuncParmRep( const FuncParmRep & point_rep );

  /** The clone function returns an object of its own kind which
      is a copy of this object at this moment. */
  virtual RepBase * clone ();

  virtual void drawProjectedValues ( ProjectorBase & projector,
				     ViewBase & view );
};

} // namespace hippodraw

#endif // _FuncParmRep_H_
