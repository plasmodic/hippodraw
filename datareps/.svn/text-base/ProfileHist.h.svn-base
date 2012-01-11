/* -*- mode: c++ -*- */

/** @file

ProfileHist class interface.

Copyright (C)  2000-2005  The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: ProfileHist.h,v 1.55 2005/10/30 00:10:10 pfkeb Exp $

*/

#ifndef _ProfileHist_H_
#define _ProfileHist_H_

#include "datareps/DataRep.h"

namespace hippodraw {

/**  A DataRep showing a profile.  A profile plot is one that takes
     slices of one column along the X axis and shows the average value
     of another column along the Y axis.   An example is shown below

     @image html profilehist.png "ProfileHist data representation.
     @image latex profilehist.eps "ProfileHist data representation.

     @request @@ Implement static version.  Don't forget to implement
     Bins1DProfile::setContents().

     @author Stephane Bonneaud <gandalf@slac.stanford.edu>
     @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
     @author Sanket B. Malde <sanket@stanford.edu>
*/
class MDL_HIPPOPLOT_API ProfileHist
  : public hippodraw::DataRep
{
 public:

  /** The default constructor. */
  ProfileHist ( );

  /** The clone function returns an object of its own kind which is a
      copy of this object at this moment. */
  virtual DataRep * clone ();

  virtual bool acceptFunction ( int num );

  /** Returns @c true.  Returns @c true SymbolPointRep class used by
      this class are capable of displaying error on the data points.
  */
  virtual bool hasErrorDisplay () const;

  virtual bool hasAxis ( hippodraw::Axes::Type ) const;

};

} // namespace hippodraw

#endif // _ProfileHist
