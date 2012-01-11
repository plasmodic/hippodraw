/* -*- mode: c++ -*- */

/** @file

hippodraw::TupleCut class interface

Copyright (C) 2000-2005   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: TupleCut.h,v 1.51 2005/12/07 19:41:21 pfkeb Exp $

*/

#ifndef _TupleCut_H_
#define _TupleCut_H_

#include "axes/Range.h"
#include "axes/AxesType.h"

#ifdef _MSC_VER
#include <msdevstudio/MSconfig.h>
#endif

namespace hippodraw {

class DataSource;

/** The class expresses a cut on a DataSource, i.e. to limit the rows
    that are to be used.  Typically it would be used to filter the
    entries in to a DataRep, or to highlight regions that are
    displayed.

    @note This class has no derived class and no virtual functions.

    @request @@@@@@ Needs form of TupleCut, or some other class,
    allowing one to use only certain set of rows, independent of the
    contents.

    @author Kaustuv <kaustuv@stanford.edu>
    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/

class MDL_HIPPOPLOT_API TupleCut
{
private:

  /** The label for the cut.
   */
  std::string m_label;

  /** The Range object used by the cut.  If the cut is not inverted, then
      the accepted data is greater than or equal to the lower end of
      the range and less than the upper end of the range.
   */
  Range m_range;

  /** The column index used by the cut.
   */
  unsigned int m_column;

  /** The inversion flag.  If @c true then a quantity must be outside
      the range defined by entries of the vector #m_range.
  */
  bool m_invert;

  /** The enable flag.  When set @c true, the cut is active,
      otherwise it is inactive.  This allows cut to be toggled to
      inactive state without losing the cut information.  
  */
  bool m_enabled;

public:

  /** The default constructor.
   */
  TupleCut ( );

  /* Using the compiler generated copy constructor.
   */
  //   TupleCut ( const TupleCut & );

  /** Sets the column in the DataSource to be used.
   */
  void setColumn ( unsigned int col );
  
  /** Returns the column index used in the DataSource. 
   */
  unsigned int  getColumn () const;
  
  /** Sets the range of the cut.
   */
  void setRange ( const Range & range );

  /** Returns a reference to the range  for each dimension. */
  const Range& getRange() const;

  /** Sets the inversion state to @a state.
   */
  void setInversion ( bool state );

  /** Returns the state of the inversion for the axis. */
  bool  getInversion () const;
  
  /** Toggles the sense of the inversion flag.
   */
  void toggleInverted();

  /** Sets the cut to be enabled, i.e. active, or not.
   */  
  void setEnabled ( bool yes = true );

  /** Returns the state of the cut's enable status.
   */
  bool isEnabled ( ) const;

  /** Returns the label for the cut.
   */
  const std::string & getLabel () const;

  /** Sets the label for the cut.
   */
  void setLabel ( const std::string & label );

  /** Returns true if the cut value at the index @a i is within the
      range.  Calls the DataSource::valueAt member obtain the cut
      value.
  */
  bool acceptRow ( const DataSource * source, unsigned int i ) const;
  
};

} // namespace hippodraw

#endif // _TupleCut_H_
