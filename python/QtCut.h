/* -*- mode: c++ -*- */

/** @file

hippodraw::QtCut class interface

Copyright (C) 2002-2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: QtCut.h,v 1.29 2007/03/27 19:34:08 pfkeb Exp $

*/

#ifndef _QtCut_H_
#define _QtCut_H_

#include "QtDisplay.h"

namespace boost {
  namespace python {
    namespace numeric {
      class array;
    }
  }
}

namespace hippodraw {

  class Cut1DPlotter;
  class Cut2DPlotter;
  class TupleCut;

/** Qt Displays wraps a derived class of Cut1DPlotter.  It locks the
    QApplication object before forwarding messages to the contained
    CutDisplay object.  It unlocks the QApplication object upon
    return.

    @author Paul_Kunz@slac.stanford.edu
*/


class QtCut : public QtDisplay
{

public:

  /** Create a cut. */
  QtCut( const DataSource & nt, 
	 const std::vector < std::string > & binding );

  /** Create a cut. */
  QtCut( const PyDataSource & nt, 
	 const std::vector < std::string > & binding );

  /** Constructor for creating a CutPlotter with axis @a binding to
      DataSource @a source, applying the TupleCut to @a target, and
      setting the range to @a low and @a high.
  */
  QtCut ( const DataSource & source,
	  const std::vector < std::string > & binding,
	  QtDisplay * target, double low, double high );

  /** Constructor for creating a CutPlotter with axis @a binding to
      PyDataSource @a nt, applying the TupleCut to @a target, and
      setting the range to @a low and @a high.
  */
  QtCut ( const PyDataSource & nt,  
	  const std::vector < std::string > & binding,
	  QtDisplay * target, double low, double high );

  /** Constructor for creating a CutPlotter using a numarray as a mask
      to the target display.
  */
  QtCut ( PyDataSource & source,
	  const std::string & label,
	  boost::python::numeric::array array,
	  QtDisplay * target );

  /** Create a cut object from a Cut[1/2]DPlotter object. If the plotter
      is neither then an assert is raised. */
  QtCut( PlotterBase * plotter );
  
  /** Adds a display as a target of the cut. */
  void addTarget ( QtDisplay * );
  
  /** Adds the displays as targets of the cut.
   */
  void addTargets ( const std::vector < QtDisplay * > & targets );

  /** Sets the range of the cut on the chosen axis. The choice of axis
      matters only when a 2D cut is being talked about  */
  void setCutRange ( double low, double high,  
		     const std::string & axis = "x" );
  
  /** Return the range of the cut. For a 1 D CutPlotter the range is
      returned as [Xlow, Xhi] while for a 2 D CutPlotter the range is returned
      as [Xlow, Xhi, Ylow, Yhi] */
  std::vector<double> cutRange();

  /** Sets the enabled status.  Sets the enabled status of the
      CutPlotter.  A plotter that is disabled keeps its target but
      does not apply the cut.
  */
  void setEnabled ( bool yes );

  /** Toggles the inversion status.
   */
  void toggleInverted ();

  static void
  fillCutList ( std::vector < const TupleCut * > & tuple_cut_list,
		const std::vector < QtCut * > & cut_list );

  /** Static method. Create a new NTuple using the column_list (labels)
      and cut_list (QtCut/CutPlotter). The returned NTuple is managed
      by Python.
  */
  static NTuple * 
  createNTuple ( const std::vector < std::string > & column_list,
		 const std::vector < QtCut * > & cut_list, 
		 DataSource * ds );

  /** Static method. Create a new NTuple using the column_list (labels)
      and cut_list (QtCut/CutPlotter). Then save it to a TNT file.
  */
  static void
  createTnt ( const std::vector < std::string > & column_list,
	      const std::vector < QtCut * > & cut_list, 
	      DataSource * ds,
	      const std::string & filename,
	      const std::string & dsname );

  /** Static method. Create a new NTuple using the column_list (labels)
      and cut_list (QtCut/CutPlotter). Then save it to a FITS file.
  */
  static void
  createFits ( const std::vector < std::string > & column_list,
	       const std::vector < QtCut * > & cut_list, 
	       DataSource * ds,
	       const std::string & filename,
	       const std::string & dsname );
    
private:

  /** Creates an uninitialized cut.
   */
  void createCut ( const DataSource & ds,
		   const std::vector < std::string > & binding );

};

} // namespace hippodraw

#endif // _QtCut_H_
