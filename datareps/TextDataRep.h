/* -*- mode: c++ -*- */

/** @file

hippodraw::TextDataRep class interface

Copyright (C) 2002-2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: TextDataRep.h,v 1.26 2006/12/05 00:08:07 xiefang Exp $
*/

#ifndef _TextDataRep_H_
#define _TextDataRep_H_

#include "DataRep.h"

namespace hippodraw {

/** Displays data in textual representation.  Can be used to display a
    few numbers from the projector of another DataRep object in
    conjunction with a derived class of TextRepBase.  Can also be used
    as a stand a lone display for a few numbers or a table showing the
    values in a DataSource object.

    @request @@ The TextDataRep view should always be locked to it's
    target.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/

class MDL_HIPPOPLOT_API TextDataRep
  : public hippodraw::DataRep
{
private:

  /** The copy constructor.  The copy constructor is private, so the
      public clone() function should be used to make a copy. 
  */
  TextDataRep ( const TextDataRep & );

public:

  /** A constructor taking the @a target DataRep and RepBase as
      arguments.  The @a target is the DataRep object containing the
      source of the information to be displayed. */
  TextDataRep ( RepBase * rep );

  /** The default constructor. */
  TextDataRep ( );

  /** The clone function returns an object of its own kind which
      is a copy of this object at this moment. */
  virtual DataRep * clone();

  /** Returns @c false as this class does not have DataSource bindings. */
  virtual bool hasNTupleBindings () const;

  /** Returns the ProjectorBase object from the target DataRep.
   */
  virtual ProjectorBase * getTargetProjector ( ) const;

  /** Overrides the base class implementation to draw the values based
      on the target DataRep. */
  virtual void drawProjectedValues ( TransformBase * transform, 
				     ViewBase * view );

  /** Returns @c false.  A TextDataRep is not meant to be target of
      another data representation.
  */
  virtual bool isTargetable () const;

  virtual bool hasAxis ( hippodraw::Axes::Type ) const;

};

} // namespace hippodraw

#endif // _TextDataRep
