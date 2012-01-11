/* -*- mode:c++ -*- */

/** @file

This is the SIP interface definition for NTupleController

Copyright (C)  2002-2006  The Board of Trustees of  
The Leland Stanford Junior University.  All Rights Reserved.

$Id: NTupleController.sip,v 1.22 2007/02/24 22:24:38 pfkeb Exp $

@author Paul_Kunz@slac.stanford.edu

*/

class NTupleController
{
%TypeHeaderCode
#include "datasrcs/NTupleController.h"
using hippodraw::NTupleController;
%End


private:

  NTupleController();

public:

  /** Returns the pointer to the singleton instance. */
  static NTupleController * instance();

  /** Creates and registers an NTuple from data in file @a filename.
      If file has already been opened, return pointer to the existing
      NTuple. Otherwise attempts to read the file as NTuple and if
      successful returns a pointer to the created NTuple object.  If
      file was not found, or could not be parsed, then throws a
      std::runtime_error with an explanation.

      @note This method is deprecated.  Create your NTuple and
      register it instead.
 */
  DataSource * createNTuple ( const std::string & ) /Factory/ ;

  /** Creates and registers an empty NTuple.  Creates an empty NTuple
      with the number of columns equal to the length of the vector @a
      labels.  The labels for the columns is taken from the elements
      of the vector @a labels. 

      @note This method is deprecated.  Create your CircularBuffer and
      register it instead.
  */
  NTuple * createNTuple ( const std::vector < std::string > &  ) /Factory/;

  /** Creates and registers an empty CircularBuffer.  Creates an empty
      CircularBuffer with the number of columns equal to the length of
      the vector @a labels.  The labels for the columns is taken from
      the elements of the vector @a labels. 

      @note This method is deprecated.  Create your CircularBuffer and
      register it instead.
  */
  CircularBuffer * 
  createCircularBuffer ( const std::vector < std::string > & ) /Factory/;

  /** Creates and registers an empty NTuple.  Creates an empty NTuple
      with @a columns columns. 

      @note This method is deprecated.  Create your CircularBuffer and
      register it instead.
  */
  NTuple * createNTuple ( unsigned int  ) /Factory/;

  /** Creates and registers an empty CircularBuffer.  Creates an empty
      CircularBuffer with @a columns columns.
  */
  CircularBuffer * createCircularBuffer ( unsigned int ) /Factory/;

  /** Creates and registers an empty NTuple.  Creates an empty NTuple
      with no columns or rows.

      @note This method is deprecated.  Create your NTuple and
      register it instead.
  */
  NTuple * createNTuple ( ) /Factory/;

  /** Creates and registers an empty CircularBuffer.  Creates an empty
      CircularBuffer with no columns or rows.
  */
  CircularBuffer * createCircularBuffer ( ) /Factory/;

  /** Returns the DataSource object with name @a name.  

      @deprecated This method is deprecated.  Use the
      DataSourceController methods instead.
  */
  DataSource * findDataSource ( const std::string &  ) const;

  /** Changes the name of the DataSource object. 
  
      @deprecated.  This method is deprecated.  Use corresponding
      method in DataSourceController instead.
   */      
  void changeName ( DataSource * ,
		    const std::string &  );

  /** Writes the DataSource @a source to a file given by @a filename.
      Returns @c 0 if successful, otherwise returns a yet to be
      determined return code.
  */
  int writeNTupleToFile ( DataSource * ,
			  const std::string & );

  /** Writes the NTuple named @a name to the file @a filename.
      Returns @c 0 if successful, otherwise returns a yet to be
      determined return code. */
  int writeNTupleToFile ( const std::string & , const std::string & );

  /** Saves all the NTuple.  Saves all the NTuple objects of the
      application with the file name prefixed by @a prefix followed by
      index number and a suffix @a suffix.  After the file is saved,
      the the file name part of @a prefix is used to set the NTuple
      name.
  */
  void saveNTuples ( const std::string & , const std::string & );

  /** Register a NTuple.  

      @deprecated This method is deprecated.  Use
      DataSourceController::registerNTuple(DataSource *) instead.
   */
  std::string registerNTuple ( DataSource *  );

  /** Register a NTuple.  

      @deprecated This method is deprecated.  Use
      DataSourceController::registerNTuple(const std::string
      &,DataSource *) instead.
  */
  void registerNTuple ( const std::string &, DataSource *  );

};
