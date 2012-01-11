/* -*- c++ -*- */

/** @file

hippodraw::FitsController class interface.

Copyright (C) 2004-2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: FitsController.h,v 1.31 2006/11/14 23:14:17 pfkeb Exp $

*/

#ifndef FitsController_H
#define FitsController_H

#include "pattern/libhippo.h"

#include <map>
#include <string>
#include <vector>

namespace hippodraw {

  class DataSource;
  class FitsFile;
  class PlotterBase;
  class TupleCut;


/** A Controller class for FITS files 

    @bug @@@@@@ Writing to file under cuts doesn't seem to work on
    Windows with cfitsio 2.5.

    @author Xie Fang <xiefang@stanford.edu>
    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/
class MDL_HIPPOPLOT_API FitsController
{

 private:

  /** A map of open FITS files.
   */
  std::map < std::string, FitsFile * > m_file_map;

  /** The version of cfitsio being used. 
   */
  mutable std::string m_version;

  /** The singleton instance of the FitsController.
   */
  static FitsController * s_instance;

  /** A list of possible DataSource names in the FITS file.
   */
  std::vector < std::string > m_ntuple_names;

  /** Returns the number of elements in a column. If a column contains
      an array, returns the size of the array for a single row,
      otherwise returns @c 1.
  */
  std::size_t calcColumnWidth ( const DataSource * source,
				unsigned int column ) const;

  /** Closed the named file, if found in list of opened files.
   */
  void closeFile ( const std::string & name );

 public:

  /** Returns the singleton instance of the FitsController.
   */
  static FitsController * instance ();

  /** The destructor. 
   */
  virtual ~FitsController();

  /** Returns the version of cfitsio being used.
   */
  const std::string & version () const;

  /** Opens the named file, if not already opened.  Opens and returns
      pointer to a FITS file.  If @a file is not found, or @a file is
      not a FITS file, then throws DataSourceException.
   */
  FitsFile * openFile ( const std::string & file );

  /** Returns a temporarily list of the names of the DataSource
      objects in the FITS file.
  */
  const std::vector < std::string > & 
  getNTupleNames ( const std::string & file_name );

  /** Creates a FitsNTuple from a ASCII or binary table in a FITS
      file. Looks from an HDU with extension name @a name.  If none
      exists, throws a DataSourceException. If it does exist, then
      returns the created FitsNTuple.
  */
  DataSource * createNTuple ( const std::string & filename,
			       const std::string & name );

  /** Creates a FitsNTuple from a ASCII or binary table in a FITS
      file.  Returns a FitsNTuple object from the file @a filename,
      with HDU number @a index.  Use @a name as the HDU name.  name @a
      name.  If the object already exists, return it, otherwise
      creates and returns a new one.  If error occurs in the creation,
      throws a DataSourceException.  Since HDU may not have a name,
      one uses the @a index to find it.
  */
  DataSource * createNTuple ( const std::string & filename, 
			      const std::string & name,
			      int index  );

  /** Check and fix plotter for FITS image data source.  If the data
      source for the Plotter is a FITS image file, then sets the
      parameters of the image.  Otherwise does nothing.
  */
  void checkForImage ( PlotterBase * plotter,
		       const DataSource & source );


  /** Write a DataSource to a FITS file as binary table.
      @bug Should throw exception if couldn't write to file, for
      example if the path is invalid.
   */
  void writeNTupleToFile ( const DataSource * ntuple,
			   const std::string & filename );

  /** Write a DataSource to a FITS file as binary table.
      @bug Should throw exception if couldn't write to file, for
      example if the path is invalid.
   */
  void writeNTupleToFile ( const std::string & name,
			   const std::string & filename );

  /** Write a image to a FITS image.
      @bug Should throw exception if couldn't write to file, for
      example if the path is invalid.
   */
  void writeImageToFile ( unsigned int x, unsigned int y,
			  const std::vector <double> & data,
			  const std::string & filename );

  /** Writes the DataSource to a FITS file as binary table.
      @param source The DataSource of any type
      @param filename The file name.
      @param name The HDU extension name.
      @param column_list The list of columns to be written to file
      @param cut_list The list of cuts on the rows of the table.
      @bug Should throw exception if couldn't write to file, for
      example if the path is invalid.
   */
  int writeNTupleToFile ( const DataSource * source,
			  const std::string & filename,
			  const std::string & name, 
			  const std::vector < std::string > & column_list,
			  const std::vector < const TupleCut * > & cut_list );


};

} // namespace hippodraw

#endif // FitsController_H
