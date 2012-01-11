/* -*- mode: c++ -*- */

/** @file

hippodraw::NTupleController class interface

Copyright (C) 2001-2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: NTupleController.h,v 1.75 2006/11/14 23:14:17 pfkeb Exp $

*/

#ifndef _NTupleController_H_
#define _NTupleController_H_

#include "pattern/libhippo.h"

#include <string>
#include <vector>

namespace hippodraw {

class CircularBuffer;
class DataSource;
class NTuple;
class Observable;
class TupleCut;

/** A singleton class that is the interface between GUI and the
    NTuple objects.  It frequently implements the application logic which
    should neither be in the GUI nor in the NTuple objects.

    @todo This class should probably inherit from DataSource
    controller as it delegates some member functions to that class and
    uses it often.  However, DataSourceController wants to be
    singleton, so can we have derived classes of singleton?

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/

class MDL_HIPPOPLOT_API NTupleController
{

private:

  /** The pointer to the singleton object. */
  static NTupleController * s_instance;


  /** A default constructor for avoiding creation except by itself. */
  NTupleController();

  /** A private copy constructor in order to avoid copying. */
  NTupleController ( const NTupleController & );

  /** Find and return position of white space.  The argument @a left
      is the starting position.  If @a tabs_only is @c true, then only
      tabs are excepted as white space.
   */
  std::string::size_type findWhite ( const std::string & line, 
				     unsigned int left = 0,
				     bool tabs_only = false );

  /** Reads ASCII formatted file and fills the NTuple.  Returns @c -1
      if file could not be opened.  Returns -2 file if there was an
      error reading the file.  Otherwise, returns 0.
   */
  int readAsciiNTuple ( NTuple * ntuple, const std::string & filename );

  /** Splits the @a line at white space and fills @a values with
      strings found.
   */
  void splitAndFill ( const std::string & line,
		      std::vector < std::string > & values );

public:

  /** Returns the pointer to the singleton instance. */
  static NTupleController * instance ();

  /** Creates and registers an NTuple from data in file @a filename.
      If file has already been opened, return pointer to the existing
      NTuple. Otherwise attempts to read the file as NTuple and if
      successful returns a pointer to the created NTuple object.  If
      file was not found, or could not be parsed, then throws a
      DataSourceException with an explanation.
  */
  DataSource * createNTuple ( const std::string & filename );

  /** Fills the empty NTuple @a source from reading data from the @a file. */
  void fillFromFile ( const std::string & file, NTuple * source );

  /** Creates and registers an empty NTuple.  Creates an empty NTuple
      with the number of columns equal to the length of the vector @a
      labels.  The labels for the columns is taken from the elements
      of the vector @a labels. 

      @deprecated Create the NTuple and registered it explicitly
      instead.

  */
  NTuple * createNTuple ( const std::vector < std::string > & labels );

  /** Creates and registers an empty CircularBuffer.  Creates an empty
      CircularBuffer with the number of columns equal to the length of
      the vector @a labels.  The labels for the columns is taken from
      the elements of the vector @a labels. 
  */
  CircularBuffer * 
  createCircularBuffer ( const std::vector < std::string > & labels );

  /** Creates and registers an empty NTuple.  Creates an empty NTuple
      with @a columns columns. 

      @deprecated Create the NTuple and registered it explicitly
      instead.

  */
  NTuple * createNTuple ( unsigned int columns );

  /** Creates and registers an empty CircularBuffer.  Creates an empty
      CircularBuffer with @a columns columns.

      @deprecated Create the CircularBuffer and registered it
      explicitly instead.

  */
  CircularBuffer * createCircularBuffer ( unsigned int columns );

  /** Creates and registers an empty NTuple.  Creates an empty NTuple
      with no columns or rows.
  */
  NTuple * createNTuple ( );

  /** Creates and registers an empty CircularBuffer.  Creates an empty
      CircularBuffer with no columns or rows.

      @deprecated Create the CircularBuffer and registered it
      explicitly instead.

  */
  CircularBuffer * createCircularBuffer ( );

  /** Returns the DataSource object with name @a name.  

      @deprecated Use the DataSourceController methods instead.
  */
  DataSource * findDataSource ( const std::string & name ) const;

  /** Changes the name of the DataSource object. 
  
      @deprecated.  There's no need to call this member function.
   */      
  void changeName ( DataSource * source,
		    const std::string & newname );

  /** Writes the DataSource @a source to a file given by @a filename.
      Returns @c 0 if successful, otherwise returns a yet to be
      determined return code.
      @bug Should throw exception if couldn't write to file, for
      example if the path is invalid.
  */
  int writeNTupleToFile ( DataSource * source,
			  const std::string & filename );

  /** Writes the NTuple named @a name to the file @a filename.
      Returns @c 0 if successful, otherwise returns a yet to be
      determined return code.
      @bug Should throw exception if couldn't write to file, for
      example if the path is invalid.
  */
  int writeNTupleToFile ( const std::string & name,
			  const std::string & filename );

  /** Saves all the NTuple.  Saves all the NTuple objects of the
      application with the file name prefixed by @a prefix followed by
      index number and a suffix @a suffix.  After the file is saved,
      the the file name part of @a prefix is used to set the NTuple
      name.
      @bug Should throw exception if couldn't write to file, for
      example if the path is invalid.
  */
  void saveNTuples ( const std::string & prefix,
		     const std::string & suffix );

  /** Register a NTuple.  

      @deprecated Use DataSourceController::registerNTuple(DataSource *)
      instead.
   */
  std::string registerNTuple ( DataSource * nt );

  /** Register a NTuple.  

      @deprecated Use 
      DataSourceController::registerNTuple(const std::string &,DataSource *)
      instead.
  */
  void registerNTuple ( const std::string &, DataSource * ntuple );

  /** Create NTuple with column list and cut list.
   */
  NTuple * createNTuple ( const std::vector < std::string > & column_list,
			  const std::vector < const TupleCut * > & cut_list,
			  DataSource * ds );

  /** Create NTuple with column list and cut list and save to a file.
   */
  int createNTupleToFile( const std::vector < std::string > & column_list,
			  const std::vector < const TupleCut * > & cut_list, 
			  DataSource * ds,
			  const std::string & filename,
			  const std::string & dsname);

};

} // namespace hippodraw

#endif // _NTupleController_H_
