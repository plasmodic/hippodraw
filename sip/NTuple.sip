// -*- mode:c++ -*-

/** @file

This is the SIP interface definition for NTuple

Copyright (C)  2002, 2004-2006  The Board of Trustees of  
The Leland Stanford Junior University.  All Rights Reserved.

$Id: NTuple.sip,v 1.28 2007/02/24 22:24:38 pfkeb Exp $

Author: Paul_Kunz@slac.stanford.edu

*/

class NTuple : DataSource
{
%TypeHeaderCode
#include "datasrcs/NTuple.h"
#include <string>
using hippodraw::NTuple;
%End


public:

  /** Constructs an NTuple object with name @a name.
   */
  NTuple ( const std::string &  );

  /** Constructs an NTuple object with name @a name.

      @note because explicit is used, this method is different from
      the one that take std::string & argument"
  */
  NTuple ( const char * );

  /** Constructor taking a vector of column labels as argument.
   */
  NTuple ( const std::vector < std::string > &  );

  /** The copy constructor creates an ntuple that is a copy of an
      existing one. 
  */
  NTuple ( const NTuple & );

  /** A constructor that creates an ntuple from another DataSource
      object.
  */
  NTuple ( const DataSource * );

  /** A constructor that creates an ntuple with @a n columns. Creates
      an NTuple with @a n columns.  The objects will be empty, i.e. 0
      rows.
   */
  NTuple ( unsigned int  );

//   /** A constructor for creating a null object.  A null NTuple is one
//       that is a place holder for one that got deleted while other
//       objects were referencing it.  This can happen, for example, if a
//       user deletes an NTuple before deleting other objects that use
//       it.  Classes that reference NTuple objects can be Observer of
//       then in order to receive the Observer::willDelete signal.  When
//       received they can replace the referenced NTuple with a null one.
//       They can then continue to function until a valid ntuple is given
//       to them.    The parameter @a dummy is not used.
//   */
//   NTuple ( bool );

  /** The default constructor creating an ntuple with 0 columns.
   */
  NTuple ();

  /** The destructor. 
   */
  virtual ~NTuple();

  /** Copies @a rhs data.
   */
  virtual void copy ( const DataSource & );

  /** Clears the NTuple.  Clears all the data elements of the
      %NTuple.  Does not change the other attributes or capacity.
  */
  virtual void clear();
  
  /** Returns @c true, if NTuple is empty, i.e. has no rows. 
   */
  virtual bool empty () const;

  /** Adds a row to the end of the ntuple.  If the size of the input
      vector is the same as the width of the ntuple, adds the row to
      the end of the ntuple and notifies its Observer objects.
      Otherwise, throws a std::runtime_error object.
  */
  virtual void addRow ( const std::vector<double> &  );

  /** Inserts a Row.  Adds a row before the existing row specified by
      @a index from the elements of @a v. 
  */
  virtual void insertRow ( unsigned int ,
			   const std::vector < double > &  );

  /** Erases a row from the NTuple.  Removes the row @a index from
      the %NTuple.  Rows beyond the removal point move forward to fill
      the space.  If @a index is out of range, and exception is
      thrown.
  */
  virtual void eraseRow ( unsigned int );

  /** Returns a temporary vector of data elements in one row.  Returns
      a temporary vector with data elements copied from the NTuple row
      indexed by @a index.  If @a index is out of range, throws a
      std::runtime_error object.
  */
  virtual const std::vector < double > & getRow ( unsigned int  ) const;

  virtual void reserve ( unsigned int );

  /** Adds a column to the end of the ntuple and returns the index to
      the added column.  
      If the size of the input vector is the same
      as the size of the existing columns adds a copy of the input
      vector to then end of the NTuple. Gives the column the label @a
      label.  Returns the index of the newly created column.  If the
      size of the input vector differs from the size of existing
      columns, then throws a std::runtime_error object.
  */
  int addColumn ( const std::string & , const std::vector < double > &  );

  /** Replaces the data in column @a col.  If @a index is in a valid
      range and if the size of @a data is the same as the existing
      column, then replaces the elements of the specified column with
      a copy of @ data elements.  If @a index is not a valid range or
      the sizes differ, then throws a std::runtime_error object.
  */
  void replaceColumn ( unsigned int , const std::vector< double > & );

  /** Replaces the column.  Replaces the column at the column with
      label @a label.  This is a cnvienence member function for using
      indexOf() and replaceColumn ( unsigned int, const std::vector<
      double > & ).
  */
//  void replaceColumn ( const std::string & , const std::vector < double > & );


  /** Replaces the data in the row @a i. If @a index is in a valid
      range and if the size of @a data is the same as the existing
      column, then replaces the elements of the specified column with
      a copy of @ data elements.  If @a index is not a valid range or
      the sizes differ, then throws a std::runtime_error object
   */
  void replaceRow ( unsigned int , const std::vector < double > &  );

  /** Assigns the label to each column from the vector of strings. If
      the vector is the same size as the width of the ntuple, then it
      sets the label and returns @c true; otherwise, it throws a
      std::runtime_error.  If the NTuple is empty, i.e. has no rows,
      then changes the NTuple's number of columns to the size of the
      vector.
  */
  void setLabels ( const std::vector<std::string> & );


//   virtual double 
//   operator [] (  std::vector < unsigned int > &  ) const;

  virtual double valueAt ( unsigned int , unsigned int  ) const;

  /* Collision with next prototype:
     virtual const std::vector< double > & 
     getColumn ( unsigned int  ) const; */

  /** Returns a non-const reference to column at the index @a index. 
      @a index is in a valid range, returns a reference to the the
      column at index @a index.  If @a index is not in a valid range,
      throws a std::runtime_error object.
  */
  virtual std::vector< double > & getColumn ( unsigned int );

  const std::vector< double > & getColumn ( const std::string &  ) const;

  virtual unsigned int indexOfMinElement ( unsigned int  ) const;
  virtual unsigned int indexOfMaxElement ( unsigned int  ) const;

  /** Returns the value of the minimum element of column @a index.
   */
  virtual double minElement ( unsigned int  ) const;

  /** Returns the value of the maximum element of column @a index.
   */
  virtual double maxElement ( unsigned int ) const;

  /** Returns minimum element in a column whose label is given by name
   */
  double columnMin( const std::string &  ) const;

  /** Returns minimum element in a column whose label is  given by name
   */
  double columnMax( const std::string & ) const;

  virtual void notifyObservers () const;

  /** Sets interval counting to be enabled.  If @a yes is @c false,
      then a notifyObservers is immediately issued.
  */
  void setIntervalEnabled ( bool = true );

  /** Returns @c true if interval counting is enabled, otherwise
      returns false.
  */
  bool isIntervalEnabled () const;

  /** Sets the interval count. One needs to call setIntervalEnabled
      for this to have effect. When interval counting is enabled, then
      only each @a number notifyObsevers messages will trigger an
      Observer::update message to the list of Observer objects.
  */
  void setIntervalCount ( int );

  /** Returns the current interval count.
   */
  unsigned int getIntervalCount ( ) const;

//   virtual void  getRange ( unsigned int , Range & ) const;
  virtual double sum ( unsigned int ) const;

  virtual unsigned rows () const;
  virtual int indexOf ( const std::string & ) const;
  bool isValidLabel ( const std::string & ) const;

};
