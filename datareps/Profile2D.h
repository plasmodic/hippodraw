/* -*- mode: c++ -*- */

/** @file

Profile2D class interface

Copyright (C)  2000-2005  The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: Profile2D.h,v 1.34 2006/11/29 21:54:42 xiefang Exp $

*/

#ifndef _Profile2D_H_
#define _Profile2D_H_

#include "DataRep.h"

namespace hippodraw {

/** A DataRep for two dimensional profile.  This data representations
    uses slices of two columns on the X and Y axis and shows the
    average value of a third column in color.  An example is shown
    below.

    @image html profile2d.png "Profile2D data representation
    @image latex profile2d.eps "Profile2D data representation

    @request @@ Implement a static version and expose to Python via
    SIP.  Don't forget to implement Bins2DProfile.setContents.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
    @author Matan Shacham <matan@slac.stanford.edu>
*/
class MDL_HIPPOPLOT_API Profile2D
  : public hippodraw::DataRep
{

public:

  /** The default constructor. */
  Profile2D ( );

  /** The clone function returns an object of its own kind which
      is a copy of this object at this moment. */
  DataRep * clone();

  virtual bool hasAxis ( hippodraw::Axes::Type ) const;

};

} // namespace hippodraw

#endif // _Profile2D
