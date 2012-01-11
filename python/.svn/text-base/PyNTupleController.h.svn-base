/* -*- mode: c++ -*- */

/** @file

hippodraw::PyNTupleController class interface.

Copyright (C) 2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: PyNTupleController.h,v 1.5 2006/10/24 19:48:56 pfkeb Exp $

*/

#ifndef _PyNTupleController_H_
#define _PyNTupleController_H_

#include <string>
#include <vector>

namespace hippodraw {

  class CircularBuffer;
  class DataSource;
  class NTupleController;
  class NTuple;
  class PyDataSource;

/** A wrapper for NTupleController for use from Python.  @note This
    wrapper was used instead of the export_ pattern so that this class
    could generate the PyDataSource object without have circular
    dependency between sub-directories.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/
class PyNTupleController 
{

private:

  /** The singleton PyNTupleController instance.
   */
  static PyNTupleController * s_instance;

  /** The singleton NTupleController object.
   */
  static NTupleController * m_instance;

  /** The default constructor, protected from being used. */
  PyNTupleController ();

  /** The copy  constructor, protected from being used. */
  PyNTupleController ( const PyNTupleController &);

public:

  /** The singleton instance method.
   */
  static PyNTupleController * instance ();

  /** Creates an empty NTuple object.
   */
  NTuple * createNTuple ();

  /** Creates a NTuple object from file @a filename.
   */
  DataSource * createNTuple ( const std::string & filename );

  /** Creates an empty CircularBuffer object with @a n columns.
   */
  CircularBuffer * createCircularBuffer ( unsigned int n );

  /** Creates a DataArray Python object from the file @a filename.
   */
  PyDataSource * createDataArray ( const std::string & filename );

  /** Finds the DataSource by @a name
   */
  DataSource * findDataSource ( const std::string & name ) const;

  /** Registers the DataSource object and returns its assigned name.
   */
  std::string registerNTuple ( DataSource * source );

  /** Registers the DataSource object as @a name
   */
  void registerNTuple ( const std::string & name, 
			DataSource * source );

  /** Writes the DataSource to the file filename.  Returns 0 if
      successful, otherwise returns a yet to be determined return
      code.
  */
  int writeNTupleToFile ( DataSource * source,
			  const std::string & filename );

  /** Changes the name of the DataSource.
   */
  void changeName ( DataSource * source, const std::string & new_name );

};

} // namespace hippodraw

#endif // _PyNTupleController_H_
