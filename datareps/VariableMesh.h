/* -*- mode: c++ -*- */

/** @file

hippodraw::VariableMesh classes interface.

Copyright (C) 2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: VariableMesh.h,v 1.51 2006/11/22 01:35:51 xiefang Exp $

*/

#ifndef _VariableMesh_H_
#define _VariableMesh_H_

#include "datareps/DataRep.h"

namespace hippodraw {

/** Displays X Y mesh.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/

class MDL_HIPPOPLOT_API VariableMesh
  : public hippodraw::DataRep
{
 public:

  /** The default constructor. */
  VariableMesh ( );

  /** The clone function returns an object of its own kind which
      is a copy of this object at this moment. */
  virtual DataRep * clone();

  bool acceptFunction ( int num );

  /** Returns @c true.  Returns @c true PointRep class used by
      this class are capable of displaying error on the data points.
  */
  virtual bool hasErrorDisplay () const;

  /** Sets the point representation.  Sets the point representation to
      @ point_rep as DataRep::setRepresentation does.  Also sets the
      error display if appropriate.
   */
  virtual void setRepresentation ( RepBase * point_rep );

  /** Sets the axis bindings.  Sets binding of the axis with name axis
      to the DataSource column with label label.
  */
  virtual void setAxisBinding ( const std::string & axis,
				const std::string & label );

  /** Sets the axis bindings.  Sets binding all axes to the DataSource
      column with label in the vector.  Also set the error display flag
      in the SymbolPointRep.
   */
  virtual 
  void setAxisBindings ( const std::vector < std::string > & bindings );

  virtual bool hasAxis ( hippodraw::Axes::Type ) const;

};

} // namespace hippodraw

#endif // _VariableMesh
