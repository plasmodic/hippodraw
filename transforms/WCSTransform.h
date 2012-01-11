/* -*- mode: c++ -*- */

/** @file

hippodraw::WCSTransform  class interface

Copyright (C) 2001-2004, 2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: PeriodicBinaryTransform.h,v 1.15 2006/02/02 22:53:15 xiefang Exp $

*/

#ifndef _WCSTransform_H_
#define _WCSTransform_H_

#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h" // for CLONE_DEFECT
#endif

#include "PeriodicBinaryTransform.h"

#include "axes/AxisTick.h"
#include "axes/Range.h"


// Use C struct in WCSLIB.
struct wcsprm;


namespace hippodraw {

/** A Periodic transform that transforms coordinates from one 2D coordinate
    system to another using the WCS coordinate library.
 
    @author kaustuv <kaustuv@stanford.edu>
    @author Xie Fang <xiefang@stanford.edu>
*/
class MDL_HIPPOPLOT_API WCSTransform : public PeriodicBinaryTransform
{

protected:
  
  /** Initialize the WCS transform type. */
  void initwcs(const std::string &transformName, double* crpix, 
               double* crval, double* cdelt, 
               double crota2, bool galactic);

  /*
   * Structure defined in WCSLIB wcs.h.  This contains all
   * transform information.
   */
  wcsprm* m_wcs;
  char m_wcs_struct[2000];


  /** The default constructor. */
  WCSTransform();

  /** The constructor that take the z transform as argument also it takes
      and argument which states if it is a periodic transformation
      (default false), next optional argument says if this transform needs
      a grid or not (default is false), and if it needs x and y ticks or
      not (Default is true). Finally we introduce xlo, xhi, ylow and yhi
      which define the appropriate limits on X and Y axis */
  WCSTransform ( UnaryTransform *, bool = false, bool  = false,
			    bool  = true, bool  = true,
			    double = -180, double = +180, double =UNITS_DEGREES,
			    double =  -90, double =  +90, double =UNITS_DEGREES
		);
  
  /** The copy constructor. */
  WCSTransform ( const WCSTransform & );
  
public:
  
  /** The virtual destructor. */
  virtual ~WCSTransform();

  virtual void setLimitX(const Range &);
  virtual void setLimitY(const Range &);

  // Use wcslib to do transform.
  virtual void transform (double & lon, double & lat) const;
  virtual bool inverseTransform (double & lon, double & lat) const;
  virtual void transform ( std::vector< double > & lon,
			   std::vector< double > & lat) const;

};

} // namespace hippodraw

#endif // _WCSTransform_H_
