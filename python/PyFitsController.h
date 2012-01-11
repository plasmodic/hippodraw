/* -*- mode: c++ -*- */

/** @file

hippodraw::PyFitsController class interface.

Copyright (C) 2004, 2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: PyFitsController.h,v 1.8 2006/09/06 17:24:27 pfkeb Exp $

*/

#ifndef _PyFitsController_H_
#define _PyFitsController_H_

#include <string>
#include <vector>

namespace hippodraw {

  class DataSource;
  class FitsController;
  class FitsNTuple;
  class PyDataSource;
  class QtCut;

/** A wrapper for FitsController.  @note This wrapper was used instead
    of the export_ pattern so that this class could generate the
    PyDataSource object without have circular dependency between
    subdirectories.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/
class PyFitsController 
{

private:

  /** The singleton PyFitsController instance. */
  static PyFitsController * s_instance;

  /** The singleton FitsController object. */
  static FitsController * m_instance;

  /** The default constructor, protected from being used. */
  PyFitsController ();

  /** The copy  constructor, protected from being used. */
  PyFitsController ( const PyFitsController &);

public:

  /** The singleton instance method.
   */
  static PyFitsController * instance ();

  /** Creates a FitsNTuple object.
   */
  FitsNTuple * createNTuple ( const std::string & filename, 
			      const std::string & hduname );

  /** Creates a DataArray Python object from the FITS file @a filename
      and the FITS HDU named @a hduname.
  */
  PyDataSource * createDataArray ( const std::string & filename, 
				   const std::string & hduname );

  /** Writes the DataSource to a FITS file as a binary table.
   */
  void writeToFile ( const DataSource * source,
		     const std::string & filename );

  /** Writes the PyDataSource to a FITS file as a binary table.
   */
  void writeToFile ( const PyDataSource * source,
		     const std::string & filename );

  /** Writes a copy of the DataSource @a source to a file.  Write to a
      file a copy of the DataSource with only columns in the
      @a column_list and rows passing all the cuts in the @a cut_list.
  */
  void writeToFile ( const DataSource * source,
		     const std::string & filename,
		     const std::vector < QtCut * > & cut_list,
		     const std::vector < std::string > & column_list );

  /** Writes a copy of the PyDataSource @a source to a file, Write to
      a file a copy of the DataSource with only columns in the
      @a column_list and rows passing all the cuts in the @a cut_list.
  */
  void writeToFile ( const PyDataSource * source,
		     const std::string & filename,
		     const std::vector < QtCut * > & cut_list,
		     const std::vector < std::string > & column_list );

  /** Returns the names of the top level tree in the Root file.
   */
  const std::vector < std::string > & 
    getNTupleNames ( const std::string & filename );

};

} // namespace hippodraw

#endif // _PyFitsController_H_
