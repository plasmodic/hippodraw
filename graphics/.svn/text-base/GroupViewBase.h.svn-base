/* -*- mode: c++ -*- */

/** @file

hippodraw::GroupViewBase class interface

Copyright (C) 2000-2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: GroupViewBase.h,v 1.2 2006/05/03 22:52:40 xiefang Exp $

*/

#ifndef _GroupViewBase_H_
#define _GroupViewBase_H_

#include "ViewBase.h"

namespace hippodraw {

/**  This is the abstract base class for a group view. It contains
     a vector of views in a group.

     @author Xie Fang <xiefang@stanford.edu>
*/

class MDL_HIPPOPLOT_API GroupViewBase : virtual public ViewBase
{
 protected:
  /** A vector that keeps all view in the same group view. */
  std::vector < ViewBase * > mViews;

 public:
  /** Constructor. */
  GroupViewBase ( );

  /** Constructor that takes a vector of views as argument. */
  GroupViewBase ( const std::vector < ViewBase * > & inViews );

  /** Copy constructor. */
  GroupViewBase ( const GroupViewBase & );

  /** Destructor. */
  virtual ~GroupViewBase ();

  /** Get all views in this group. */
  const std::vector < ViewBase * > & getViews ();

  /** Get plotters of every view. */
  const std::vector < PlotterBase * > getPlotters () const;

  /** Add a view to the group. */
  void addView ( ViewBase * inView );

  /** Remove a view in the group. */
  void removeView ( ViewBase * inView );

};

} // namespace hippodraw

#endif // _GroupViewBase_H_

  
