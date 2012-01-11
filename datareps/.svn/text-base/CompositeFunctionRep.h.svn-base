/* -*- mode: c++ -*- */

/** @file

CompositeFunctionRep class interface

Copyright (C) 2005, 2007   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: CompositeFunctionRep.h,v 1.10 2007/09/14 22:42:36 pfkeb Exp $

*/

#ifndef _CompositeFunctionRep_H_
#define _CompositeFunctionRep_H_

#include "FunctionRep.h"


class FunctionParameter;

namespace hippodraw {

/** A Derived class of FunctionRep which hold a composite function.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/
class MDL_HIPPOPLOT_API CompositeFunctionRep : public FunctionRep
{

 public:

  /** The type of container for the list of FunctionRep objects.
   */
  typedef std::vector < FunctionRep * > FunctionRepList_t;

 private:

  /** The list of FunctionRep objects making up the composite.
   */
  FunctionRepList_t m_func_reps;


 public:

  /** The constructor.
   */
  CompositeFunctionRep ( FunctionBase *, DataRep * );

  virtual void addToComposite ( FunctionRep * frep );

  virtual void removeFromComposite ( FunctionRep * frep );

  /** Returns @c true if the FunctionRep is a member of the Composite.
   */
  bool isMember ( const FunctionRep * );

  /** Returns the number of FunctionRep objects contained by the composite.
   */
  unsigned int count () const;

  /** Returns the list of contained FunctionRep objects. */
  const FunctionRepList_t & getFunctionReps () const;

  /** Sets the fixed flags of its contained function representations
      and itself.
  */
  virtual void setFixedFlags ( const std::vector < int > & flags );

  virtual void drawProjectedValues ( TransformBase * transform,
				     ViewBase * view );

};

} // namespace hippodraw

#endif // _CompositeFunctionRep_H_
