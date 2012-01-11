/* -*- mode: c++ -*- */

/** @file

hippodraw::PyDataSource class interface
 
Copyright (C)  2005, 2006, 2007  The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: PyDataSource.h,v 1.28 2007/08/24 22:19:16 pfkeb Exp $

*/

#ifndef _PyDataSource_H
#define _PyDataSource_H

#include <exception>
#include <string>
#include <vector>

#ifndef _MSC_VER
#include <inttypes.h>
#endif

namespace boost {
  namespace python {
    namespace numeric {
      class array;
    }
  }
}

namespace hippodraw {

  class DataSource;

/** This class is the public interface to a DataSource object that the 
    user sees as the @em DataArray object from Python.  

    @author J. Chiang <jchiang@slac.stanford.edu>
*/

class PyDataSource
{

private:

  /** Checks the rank of the array.  Check of the @a array and if the
      rank is greater than @c 1, throws a std::runtime_error.
  */
  void checkRank ( boost::python::numeric::array array );

public:

   /** Default constructor wraps an ordinary NTuple */
   PyDataSource();

   /** Constructor taking the kind of DataSource as an argument. */
   PyDataSource(const std::string & dataSource);

   /** Constructor taking an existing DataSource and its name as an
       argument.
   */
   PyDataSource ( const std::string & name, DataSource * source );

   virtual ~PyDataSource();

  /** Extracts a vector from the numarray object. Extracts a vector
      from the numarray object @a array and fills the vector @a col.
      If @a array is contiguous, copies the data directly, otherwise
      copies the data element by element.
   */
   void static extractVector ( boost::python::numeric::array array,
			       std::vector<double> & col );

  /** Copies and converts to double, if needed, data of type @a T from
      @a array to fill the std::vector @a col
   */
  template < typename T >
  void static copy_direct ( boost::python::numeric::array array,
			    std::vector<double > & col );

  /** Returns a new PyDataSource that wraps the current DataSource in
      the DataSourceController.
  */
  static PyDataSource * getCurrentDataSource ();

   /** Return a reference to the underlying DataSource */
   const DataSource & dataSource() const { return *m_dataSource; }

   unsigned int columns() const;

   unsigned int rows() const;

   /** The title of the DataSource. */
   const std::string & getTitle () const;

   void setTitle(const std::string & title);

   /** Set the name of the DataSource.
    */
   void setName(const std::string & name);

   /** Get the column names */
   const std::vector<std::string> & getLabels() const;

   /** Get a column as a tuple of floats by column name */
   const std::vector<double> & getColumn(const std::string & name) const;
   /** Get a column as a tuple of floats by column index */
   const std::vector<double> & getColumn(unsigned int index) const;

   /** Replace a column by its label */
   void replaceColumn(const std::string &,
                      const std::vector<double> & col);

   /** Replace a column by its index */
   void replaceColumn(unsigned int index, 
                      const std::vector<double> & col);

   /** Replace a column by its label.
    */
   void replaceColumn ( const std::string &,
			boost::python::numeric::array array );

   /** Replace a column by its index.
    */
   void replaceColumn ( unsigned int index, 
			boost::python::numeric::array array);

   /** Add a column to an NTuple or ListTuple */
   int addColumn( const std::string & label,
                  const std::vector<double> & col );

   /** Add a column to a NumArrayTuple.

       @request @@@@ This method only works if contained DataSource is
       NumArrayTuple.  It could accept more like
       saveColumnFromNumArray does.
 */
   int addColumn( const std::string & label, 
		  boost::python::numeric::array array );

   /** Clear the data elements of the DataSource */
   void clear();

   /** Return true if column with label @a name exists in DataSource.
    */
   bool hasColumn(const std::string & name) const;

   /** Register this DataSource, returning a unique name.
    */
   std::string registerNTuple();

   /** Register this DataSource by a chosen @a name.
    */
   void registerNTuple( const std::string & name );

   /** Return a column from the DataSource as a numarray object, 
       indexing by column @a label.
   */
   boost::python::numeric::array
   columnAsNumArray( const std::string & label ) const;

   /** Return a column from the DataSource as a numarray object, 
       indexing by column number.
   */
   boost::python::numeric::array
   columnAsNumArray( unsigned int index ) const;

  /** Replaces or add column vector. 
   */
  void saveColumn ( const std::string & label,
		    const std::vector < double > & v,
		    const std::vector < intptr_t > & shape );

   /** Replace or add a column from a numarray object,
       indexing by column label.
   */
   void saveColumnFromNumArray( const std::string & label,
                                boost::python::numeric::array array );

   /** Replace or add a column from a numarray object,
       indexing by column label.
   */
   void saveColumnFromNumArray( unsigned int index,
                                boost::python::numeric::array array );


  /** Replace or add a column from vector.
   */
  void saveColumnFrom ( const std::string & label,
			const std::vector < double > & array );

  /** Adds a row to the DataSource.
   */
  void addRow ( const std::vector < double > & array );

  /** Adds all the rows of @a source to the DataArray.
   */
  void append ( const DataSource * source );

  /** Adds all the rows of @a source to the DataArray.
   */
  void append ( const PyDataSource * source );

   class StopIteration : public std::exception {
   public:
      StopIteration(const std::string & what) : m_what(what) {}
      ~StopIteration() throw() {}
      const char * what() const throw() {
         return m_what.c_str();
      }
   private:
      const std::string m_what;
   };

  
private:

   /** The type of data source. */
   std::string m_type;

   /** The actual DataSource object. */
   DataSource * m_dataSource;

};

} // namespace hippodraw

#endif // _PyDataSource_H
