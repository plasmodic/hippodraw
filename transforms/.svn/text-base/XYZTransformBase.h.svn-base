/*
 * HippoPlot XYZTransformBase  class interface
 *
 * Copyright (C) 2001-2003   The Board of Trustees of The Leland
 * Stanford Junior University.  All Rights Reserved.
 *
 * $Id: XYZTransformBase.h,v 1.6 2005/10/30 00:10:18 pfkeb Exp $
 */

#ifndef _XYZTransformBase_H_
#define _XYZTransformBase_H_

#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h" // for CLONE_DEFECT
#endif

#include "TransformBase.h"

namespace hippodraw {

/** A transform that transforms coordinates from one 3D coordinate
    system to another.
 
    @todo This class should go away.
 
    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
    @author Oded Wurman <owurman@stanford.edu>
*/

class MDL_HIPPOPLOT_API XYZTransformBase : public TransformBase
{

protected:

  /** The default constructor. */
  XYZTransformBase ();

  /** The copy constructor. */
  XYZTransformBase ( const XYZTransformBase & );

 public:

  /** The virtual destructor. */
  virtual ~XYZTransformBase();

  /** Creates a new XYZTransform object by copying an existing one. */
#ifdef CLONE_DEFECT
  virtual TransformBase * clone () const = 0;
#else
  virtual XYZTransformBase  * clone () const = 0;
#endif

};

} // namespace hippodraw

#endif // _XYZTransformBase_H_
