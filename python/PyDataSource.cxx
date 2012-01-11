/** @file

hippodraw::PyDataSource class implementation and python interface
implementation.
 
Copyright (C) 2005, 2006, 2007   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: PyDataSource.cxx,v 1.66 2007/12/13 19:20:27 pfkeb Exp $

*/

#ifdef HAVE_CONFIG_H
// for root and cfitsio
#include "config.h"
#endif

// For truncation warning
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

// include first to avoid _POSIX_C_SOURCE warning.
#include <boost/python.hpp>

#include "PyDataSource.h"

#include "ListTuple.h"
#include "PyApp.h"

#include "datasrcs/DataSourceController.h"
#include "datasrcs/NTuple.h"
#include "pattern/string_convert.h"

#ifdef HAVE_NUMARRAY
#include "numarray/NumArrayTuple.h"
#include "numarray/num_util.h"
#endif

#ifdef HAVE_CFITSIO
#include "fits/FitsNTuple.h"
#endif

#ifdef HAVE_ROOT
#include "root/QtRootNTuple.h"
#endif

using namespace boost::python;

#ifdef HAVE_NUMARRAY
namespace hippodraw {
  namespace Python {

  /** Copies and converts to double, if needed, data of type @a T from
      @a array to fill the std::vector @a col
   */
// template < typename T >
// void copy_direct ( boost::python::numeric::array array,
// 		   std::vector<double > & col )
// {
//   T * data = reinterpret_cast < T * > ( num_util::data ( array ) );
//   int size = num_util::size ( array );
//   std::copy ( data, data+ size, back_inserter( col ) );
// }

//   /** Extracts a vector from the numarray object. Extracts a vector
//       from the numarray object @a array and fills the vector @a col.
//       If @a array is contiguous, copies the data directly, otherwise
//       copies the data element by element.
//    */
//    void extractVector ( boost::python::numeric::array array,
// 			std::vector<double> & col )
// {
//   PyArray_TYPES type = num_util::type ( array );

//   if ( num_util::iscontiguous ( array ) &&
//        type != PyArray_NOTYPE ) {
//     switch ( type )
//       {
// #ifdef HAVE_NUMPY
//       case PyArray_BOOL:
// 	copy_direct < bool > ( array, col );
// 	break;
// #endif
//       case PyArray_CHAR:
// 	copy_direct < char > ( array, col );
// 	break;

//       case PyArray_SHORT :
// 	copy_direct < short > ( array, col );
// 	break;

//       case PyArray_INT :
// 	copy_direct < int > ( array, col );
// 	break;

//       case PyArray_UINT :
// 	copy_direct < unsigned int > ( array, col );
// 	break;
//       case PyArray_FLOAT :
// 	copy_direct < float > ( array, col );
// 	break;
//       case PyArray_DOUBLE :
// 	copy_direct < double > ( array, col );
// 	break;

//       default:
// 	std::string what ( "DataArray: Array type `" );
// 	what += num_util::type2string ( type );
// 	what += "' not supported.";
// 	throw std::runtime_error ( what );
// 	break;
//       }
//   }
//   else { // not contiguous
//       const numeric::array & my_array = array;
//       int rank = num_util::rank ( my_array );
//       if ( rank < 2 ) { // rank 1
// 	int size = num_util::size( my_array );
// 	col.clear();
// 	col.reserve(size);
// 	for (int i = 0; i < size; i++) {
// 	  boost::python::object result = my_array[i];
// 	  col.push_back( boost::python::extract<double>(result) );
// 	}
//       }
//       else { // rank > 1
// 	boost::python::object tarray = array.getflat();
// 	int size = num_util::size ( array );
// 	for ( int i = 0; i < size; i++ ) {
// 	  boost::python::object result = tarray[i];
// 	  col.push_back ( boost::python::extract < double > ( result ) );
// 	}
//       }
//   } 

// }
} // namespace
} // namespace
#endif

namespace hippodraw {
namespace Python {

void PyDataSourceExceptionTranslator( const PyDataSource::StopIteration & e ) {
   PyErr_SetString ( PyExc_StopIteration, e.what() );
}

void
export_DataArray()
{
   class_ < PyDataSource >
     ( "DataArray",
       "A DataArray wraps a DataSource object so that numerical Python\n"
       "arrays can be used as both input and output.   The numerical array\n"
       "can be either a numarray.array or Numeric.array depending on\n"
       "how the hippo module was configured.\n"
       "\n"
       "Access to the array is done like a Python list or dictionary.\n"
       "\tarray = my_data_array [ index ] # list form\n"
       "\tarray = my_data_array [ 'my_label' ] # dict form\n"
       "\n"
       "Storage to the array is also done like a Python list or dictionary.\n"
       "\tmy_data_array [ index ] = ... # list form\n"
       "\tmy_data_array [ 'my_label' ] = ... # dict form\n" )

      .def ( init < const std::string & > (
	     "DataArray ( string ) -> DataArray\n"
	     "\n"
	     "Creates a DataArray.   The string can be one of\n"
	     "\tListTuple\n"
	     "\tNTuple\n"
	     "\tNumArrayTuple" ) )

      .add_property ( "columns", 
                      &PyDataSource::columns )
      
      .add_property ( "rows",
                      &PyDataSource::rows )

     .def ( "getCurrent",
	    &PyDataSource::getCurrentDataSource,
	    return_value_policy < reference_existing_object > (),
	    "getCurrent () -> DataArray\n"
	    "\n"
	    "Returns a DataArray that wraps the current DataSource." )

     .staticmethod ( "getCurrent" )

     .def ( "dataSource",
	    &PyDataSource::dataSource,
	    return_value_policy < reference_existing_object > (),
	    "dataSource () -> DataSource\n"
	    "\n"
	    "Returns reference to underlying DataSource"  )

      .def ( "getTitle", 
             &PyDataSource::getTitle,
             return_value_policy < copy_const_reference > (),
	     "getTitle () -> string\n"
	     "\n"
	     "Returns title of the DataSource." )

      .def ( "setName",
             &PyDataSource::setName,
	     "setName ( string ) -> None\n"
	     "\n"
             "Sets the name of the DataSource. The name should be unique\n"
	     "with one application and may appear in the Inspector panel." )
      
      .def ( "setTitle",
             &PyDataSource::setTitle,
	     "setTitle ( string ) -> None\n"
	     "\n"
             "Sets the title of the DataSource.  The title is what appears,\n"
	     "by default, at the top of a Display." )
	   
      .def ( "getLabels",
             &PyDataSource::getLabels,
             return_value_policy < copy_const_reference > (),
	     "getLabels () -> list\n"
	     "\n"
             "Returns the list of string objects of column labels." )
      
      .def ( "addColumn", 
             ( int (PyDataSource:: * )
               (const std::string &, const std::vector<double> &) )
             &PyDataSource::addColumn,
	     "addColumn ( string, list ) -> value\n"
	     "addColumn ( string, array ) -> value\n"
	     "\n"
             "Adds a column. The string will be the label of the column.\n"
	     "A copy of the list or array values will be the contents.\n"
	     "The second form is only available if HippoDraw was configured\n"
	     "with numerical Python support. Returns the new column's index." )
             
#ifdef HAVE_NUMARRAY      
      .def ( "addColumn", 
             ( int (PyDataSource:: * )
               (const std::string &, boost::python::numeric::array) )
             &PyDataSource::addColumn )
#endif
             
      .def ( "getColumn", 
             ( const std::vector < double > & (PyDataSource:: * ) // fptr
               ( unsigned int ) const) // function signature
             &PyDataSource::getColumn,
             return_value_policy < copy_const_reference> (),
	     "getColumn ( value ) -> list\n"
	     "\n"
             "Returns a column as list of floats.  'value' maybe either\n"
	     "the column label or its index." )

      .def ( "getColumn", 
             ( const std::vector < double > & (PyDataSource:: * ) // fptr
               ( const std::string & ) const) // function signature
             &PyDataSource::getColumn,
             return_value_policy < copy_const_reference> () )
      
      .def ( "replaceColumn", 
             ( void (PyDataSource:: * )
               ( const std::string &, const std::vector<double> & ) )
             &PyDataSource::replaceColumn,
	     "replaceColumn ( value, list ) -> None\n"
	     "replaceColumn ( value, array ) -> None\n"
	     "\n"
             "Replace column by its label or index.  The second form is \n"
	     "only available if HippoDraw was configure with numerical\n"
	     "arrays." )
      
      .def ( "replaceColumn", 
             ( void (PyDataSource:: * ) // fptr
               ( unsigned int index, const std::vector<double> & ) )
             &PyDataSource::replaceColumn )

      .def ( "replaceColumn", 
             ( void (PyDataSource:: * ) // fptr
               ( const std::string &, numeric::array ) ) // function signature
             &PyDataSource::replaceColumn )
      
      .def ( "replaceColumn", 
             ( void (PyDataSource:: * ) // fptr
               ( unsigned int index, numeric::array ) ) // function signature
             &PyDataSource::replaceColumn )

// comment out until implemented for ListTuple and NumArrayTuple
//       .def ( "clear",
//              &PyDataSource::clear,
//              "Clears the data elements of the DataSource" )

      .def ("has_key",
            &PyDataSource::hasColumn,
	    "has_key ( string ) -> Boolean\n"
	    "\n"
            "Returns True if column with label exists.")

      .def ( "keys",
             &PyDataSource::getLabels,
             return_value_policy < copy_const_reference > (),
	     "keys () -> list\n"
	     "\n"
             "Returns the list of column labels." )
      
      .def ( "register",
             ( void (PyDataSource:: * ) ( const std::string & ) )
             &PyDataSource::registerNTuple,
	     "register ( string ) -> None\n"
	     "register ( ) -> string\n"
	     "\n"
             "Register the underlying DataSource with the\n"
	     "DataSourceController. The first form registers it with the\n"
	     "given name, while the second from returns a unique name\n"
	     "generated by the controller." )

      .def ( "register",
             ( std::string (PyDataSource:: * ) () )
             &PyDataSource::registerNTuple )


     // The following must come before the other __setitem__ so that
     // it is seen last, otherwise a numarray gets converted to
     // vector.
     .def ( "__setitem__",
	    &PyDataSource::saveColumnFrom,
	    "__setitem__ ( label, sequence ) -> None\n"
	    "\n"
	    "Copies the contensts of the sequence.   If column with label\n"
	    "already exists, replaces it, otherwise add a new column." )

     .def ( "addRow",
	    &PyDataSource::addRow,
	    "addRow ( sequence ) -> None\n"
	    "\n"
	    "Adds a row to the held DataSource object if supported, otherwise"
	    " throws Runtime exception." )

     .def ( "append",
	    ( void ( PyDataSource:: * ) // function pointer
	      ( const DataSource * ) ) // signature
	    &PyDataSource::append,
	    "append ( DataArray ) -> None\n"
	    "append ( DataSource ) -> None\n"
	    "\n"
	    "Appends contents of DataSource or DataArray to the DataArray." )

     .def ( "append",
	    ( void ( PyDataSource:: * ) // function pointer
	      ( const PyDataSource * ) ) // signature
	    &PyDataSource::append )


      .def ( "__getitem__", 
             ( numeric::array ( PyDataSource:: * )
               ( const std::string & ) const )
             &PyDataSource::columnAsNumArray,
             return_value_policy < return_by_value > (),
	     "__getitem__ ( value ) -> array\n"
	     "\n"
             "Returns a copy of the column as numerical array.   'value' can\n"
	     "be either the column label or its index." )

      .def ( "__getitem__", 
             ( numeric::array ( PyDataSource:: * )
               ( unsigned int ) const )
             &PyDataSource::columnAsNumArray,
             return_value_policy < return_by_value > () )

      .def ( "__setitem__", 
             ( void ( PyDataSource:: * )
               ( const std::string &,
		 numeric::array ) )
             &PyDataSource::saveColumnFromNumArray,
             return_value_policy < return_by_value > (),
	     "__setitem__ ( value, array ) -> None\n"
	     "\n"
             "Copies the contents of array.  If `'value' is an index, then\n"
	     "replaces the contents of the existing column.   If 'value' is\n"
	     "a label then either replaces existing column with that label\n"
	     "or adds a new column." )

      .def ( "__setitem__", 
             ( void ( PyDataSource:: * )
               ( unsigned int,
		 numeric::array ) )
             &PyDataSource::saveColumnFromNumArray,
             return_value_policy < return_by_value > () )

      ;
   register_exception_translator<PyDataSource::StopIteration>
      (&PyDataSourceExceptionTranslator);
}
} // namespace Python
} // namespace hippodraw

using namespace hippodraw;

PyDataSource::PyDataSource() {
   m_type = "NTuple";
   m_dataSource = new NTuple();
}

PyDataSource::
PyDataSource ( const std::string & name, DataSource * source )
  : m_type ( name ),
    m_dataSource ( source )
{
}

/** @request @@@@ Have a constructor that takes DataSource and figures
    out what type it is.
 */
PyDataSource::
PyDataSource(const std::string & dataSource)
  : m_type(dataSource) {
   if (dataSource == "ListTuple") {
      m_dataSource = new ListTuple();
   } else if (dataSource == "NTuple") {
      m_dataSource = new NTuple();
#ifdef HAVE_NUMARRAY
   } else if (dataSource == "NumArrayTuple") {
      m_dataSource = new NumArrayTuple();
#else
   } else if (dataSource == "NumArrayTuple") {
      throw std::runtime_error ("HippoDraw was not built with "
				"numeric Python soupport" );
#endif
   } else {
      throw std::runtime_error("Invalid DataSource: " + dataSource);
   }
}

PyDataSource::
~PyDataSource()
{
   delete m_dataSource;
}

template < typename T >
void
PyDataSource::
copy_direct ( boost::python::numeric::array array,
	      std::vector<double > & col )
{
#ifdef HAVE_NUMARRAY
  T * data = reinterpret_cast < T * > ( num_util::data ( array ) );
  int size = num_util::size ( array );
  std::copy ( data, data+ size, back_inserter( col ) );
#endif
}

void 
PyDataSource::
extractVector ( boost::python::numeric::array array,
		std::vector<double> & col )
{
#ifdef HAVE_NUMARRAY
  PyArray_TYPES type = num_util::type ( array );

  if ( num_util::iscontiguous ( array ) &&
       type != PyArray_NOTYPE ) {
    switch ( type )
      {
#ifdef HAVE_NUMPY
      case PyArray_BOOL:
	copy_direct < bool > ( array, col );
	break;
#endif
      case PyArray_CHAR:
	copy_direct < char > ( array, col );
	break;

      case PyArray_SHORT :
	copy_direct < short > ( array, col );
	break;

      case PyArray_INT :
	copy_direct < int > ( array, col );
	break;

      case PyArray_UINT :
	copy_direct < unsigned int > ( array, col );
	break;
      case PyArray_FLOAT :
	copy_direct < float > ( array, col );
	break;
      case PyArray_DOUBLE :
	copy_direct < double > ( array, col );
	break;

      default:
	std::string what ( "DataArray: Array type `" );
	what += num_util::type2string ( type );
	what += "' not supported.";
	throw std::runtime_error ( what );
	break;
      }
  }
  else { // not contiguous
      const numeric::array & my_array = array;
      int rank = num_util::rank ( my_array );
      if ( rank < 2 ) { // rank 1
	int size = num_util::size( my_array );
	col.clear();
	col.reserve(size);
	for (int i = 0; i < size; i++) {
	  boost::python::object result = my_array[i];
	  col.push_back( boost::python::extract<double>(result) );
	}
      }
      else { // rank > 1
	boost::python::object tarray = array.getflat();
	int size = num_util::size ( array );
	for ( int i = 0; i < size; i++ ) {
	  boost::python::object result = tarray[i];
	  col.push_back ( boost::python::extract < double > ( result ) );
	}
      }
  } 
#endif
}

PyDataSource *
PyDataSource::
getCurrentDataSource ()
{
  PyDataSource * array = 0;

  DataSourceController * controller = DataSourceController::instance ();
  DataSource * source = controller -> getCurrent ();

  if ( source != 0 ) {
    NTuple * ntuple = dynamic_cast < NTuple * > ( source );
    if ( ntuple != 0 ) {
      array = new PyDataSource ( "NTuple", source );
    }

    ListTuple * ltuple = dynamic_cast < ListTuple * > ( source );
    if ( ltuple != 0 ) {
      array = new PyDataSource ( "ListTuple", source );
    }

#ifdef HAVE_NUMARRAY
    NumArrayTuple * natuple = dynamic_cast < NumArrayTuple * > ( source );
    if ( natuple != 0 ) {
      array = new PyDataSource ( "NumArrayTuple", source );
    }
#endif

#ifdef HAVE_CFITSIO
    FitsNTuple * fntuple = dynamic_cast < FitsNTuple * > ( source );
    if ( fntuple != 0 ) {
      array = new PyDataSource ( "FitsNTuple", source );
    }
#endif

#ifdef HAVE_ROOT
    RootNTuple * rntuple = dynamic_cast < RootNTuple * > ( source );
    if ( rntuple != 0 ) {
      array = new PyDataSource ( "RootNTuple", source );
    }
#endif
  }


  return array;
}

unsigned int PyDataSource::columns() const {
   return m_dataSource->columns();
}

unsigned int PyDataSource::rows() const {
   return m_dataSource->rows();
}

const std::string & PyDataSource::getTitle() const {
   return m_dataSource->title();
}

void PyDataSource::setTitle(const std::string & title) {
   m_dataSource->setTitle(title);
}

void PyDataSource::setName(const std::string & name) {
   m_dataSource->setName(name);
}

const std::vector<std::string> & PyDataSource::getLabels() const {
   return m_dataSource->getLabels();
}

const std::vector<double> & 
PyDataSource::getColumn(const std::string & name) const {
   return m_dataSource->getColumn(name);
}

const std::vector<double> & 
PyDataSource::getColumn(unsigned int index) const {
   return m_dataSource->getColumn(index);
}

void
PyDataSource::
replaceColumn (const std::string & label,
	       const std::vector < double > & col )
{
   if ( m_type == "NTuple" || 
	m_type == "FitsNTuple" ||
	m_type == "RootNTuple" ) {
     m_dataSource -> replaceColumn ( label, col );
   } else if (m_type == "ListTuple") {
      ListTuple * nt = dynamic_cast<ListTuple *>(m_dataSource);
      boost::python::list seq(col);
      nt->replaceColumn(label, seq);
   } else {
      std::string what("Cannot replace a column of this type in a " + m_type);
      throw std::runtime_error(what);
   }
}

void
PyDataSource::
replaceColumn ( unsigned int index,
		const std::vector < double > & col)
{
   const std::vector<std::string> & names = m_dataSource->getLabels();
   if ( index < names.size() ) {
      replaceColumn(names[index], col);
   } else {
      std::string what ( "Invalid column index: " );
      what += hippodraw::String::convert ( index );
      throw std::runtime_error ( what );
   }
}

void
PyDataSource::
replaceColumn ( const std::string & label,
		boost::python::numeric::array array)
{
#ifdef HAVE_NUMARRAY
   NumArrayTuple * nt = dynamic_cast<NumArrayTuple *>(m_dataSource);
   if (!nt) {
      std::string what("Cannot replace a column of this type in a " + m_type);
      throw std::runtime_error(what);
   }
   nt->replaceColumn(label, array);
#else
   throw std::runtime_error ( "HippoDraw was not built with "
			      "numeric Python suppport" );
#endif
}

void
PyDataSource::
replaceColumn ( unsigned int index,
		boost::python::numeric::array array )
{
#ifdef HAVE_NUMARRAY
   NumArrayTuple * nt = dynamic_cast<NumArrayTuple *>(m_dataSource);
   if (!nt) {
      std::string what("Cannot replace a column of this type in a " + m_type);
      throw std::runtime_error(what);
   }
   nt->replaceColumn(index, array);
#else
   throw std::runtime_error ( "HippoDraw was not built with "
			      "numeric Python suppport" );
#endif
}

int PyDataSource::addColumn( const std::string & label,
                             const std::vector<double> & col ) {
   if (m_type == "NTuple") {
      NTuple * nt = dynamic_cast<NTuple *>(m_dataSource);
      return nt->addColumn(label, col);
   } else if (m_type == "ListTuple") {
      ListTuple * nt = dynamic_cast<ListTuple *>(m_dataSource);
      boost::python::list seq(col);
      return nt->addColumn(label, seq);
   } else {
      std::string what("Cannot add a column of this type to a " + m_type);
      throw std::runtime_error(what);
   }
   return m_dataSource->columns();
}

int PyDataSource::addColumn( const std::string & label, 
                             boost::python::numeric::array array ) {
#ifdef HAVE_NUMARRAY
   NumArrayTuple * nt = dynamic_cast<NumArrayTuple *>(m_dataSource);
   if (!nt) {
      std::string what("Cannot add a column of this type to a " + m_type);
      throw std::runtime_error(what);
   }
   return nt->addColumn(label, array);
#else
   throw std::runtime_error ( "HippoDraw was not built with "
			      "numeric Python support" );
#endif
}

void PyDataSource::clear() {
   m_dataSource->clear();
}

bool PyDataSource::hasColumn(const std::string & colname) const {
   const std::vector<std::string> & names = getLabels();
   return std::find(names.begin(), names.end(), colname) != names.end();
}

void PyDataSource::registerNTuple( const std::string & name ) {
   m_dataSource->setName(name);
   DataSourceController * controller  = DataSourceController::instance();
   controller->registerNTuple(name, m_dataSource);
}

std::string PyDataSource::registerNTuple() {
   DataSourceController * controller  = DataSourceController::instance();
   return controller->registerNTuple(m_dataSource);
}

boost::python::numeric::array
PyDataSource::
columnAsNumArray( const std::string & colname ) const
{
#ifdef HAVE_NUMARRAY
  if (m_type == "NumArrayTuple") {
      NumArrayTuple * nt = dynamic_cast<NumArrayTuple *>(m_dataSource);
      boost::python::numeric::array a = nt->getNumArray(colname);
      return a;
   }

#ifdef HAVE_ROOT
   if ( m_type == "RootNTuple" ) {
     QtRootNTuple * tuple = dynamic_cast < QtRootNTuple *> ( m_dataSource );
     boost::python::numeric::array a = tuple -> getColumnAsArray ( colname );
     return a;
   }
#endif

   typedef std::vector<double> vec;
   const vec & array = m_dataSource->getColumn(colname);
   std::vector < int > shape;
   m_dataSource -> fillShape ( shape, colname );
   numeric::array na 
     = num_util::makeNum ( &const_cast<vec &>( array )[0], shape );
   return na;
#else
   throw std::runtime_error ("HippoDraw was not built with "
			     "numeric Python support" );
#endif // HAVE_NUMARRAY
}

boost::python::numeric::array
PyDataSource::
columnAsNumArray ( unsigned int index ) const {
#ifdef HAVE_NUMARRAY
   if ( index < columns() ) {
      if (m_type == "NumArrayTuple") {
         NumArrayTuple * nt = dynamic_cast<NumArrayTuple *>(m_dataSource);
         return nt->getNumArray(index);
      } 
      typedef std::vector<double> vec;
      const vec & array = m_dataSource->getColumn(index);
      std::vector < int > shape;
      m_dataSource -> fillShape ( shape, index );
      numeric::array na = 
	num_util::makeNum ( &const_cast <vec &> ( array )[0], shape );
      return na;
   } else {
      throw StopIteration("index out-of-range");
   }
#else
   throw std::runtime_error ( "HippoDraw was not built with "
			      "numeric Python support" );
#endif
}

void
PyDataSource::
checkRank ( boost::python::numeric::array array )
{
#ifdef HAVE_NUMARRAY
  int rank = num_util::rank ( array );
  if ( rank > 1 ) {
    std::string what ( "DataArray: Can not add " );
    what += hippodraw::String::convert ( rank );
    what += " dimensional array\n to ";
    what += m_type;
    throw std::runtime_error ( what );
  }
#endif
}

void
PyDataSource::
saveColumn ( const std::string & label,
	     const std::vector < double > & v,
	     const std::vector < intptr_t > & shape )
{
  if ( hasColumn ( label ) ) {
    m_dataSource -> replaceColumn ( label, v, shape  );
  } else {
    m_dataSource -> addColumn ( label, v, shape );
  }
}

void
PyDataSource::
saveColumnFromNumArray ( const std::string & label,
			 boost::python::numeric::array array )
{
#ifdef HAVE_NUMARRAY
  PyApp::lock ();
   if (m_type == "NumArrayTuple") {
      if (hasColumn(label)) {
         replaceColumn(label, array);
      } else {
         addColumn(label, array);
      }
      PyApp::unlock();
      return;
   }

#ifdef HAVE_CFITSIO
   FitsNTuple * fnt = dynamic_cast < FitsNTuple * > ( m_dataSource );
   if ( fnt != 0 ) {
     std::vector < double > vec;
     extractVector ( array, vec );

     const std::vector < intptr_t > shape = num_util::shape ( array );
     saveColumn ( label, vec, shape );

      PyApp::unlock();
     return;
   }
#endif // cfitsio

   if (m_type == "ListTuple") {
      boost::python::list seq(array);
      ListTuple * nt = dynamic_cast<ListTuple *>(m_dataSource);
      if (hasColumn(label)) {
         nt->replaceColumn(label, seq);
      } else {
         nt->addColumn(label, seq);
      }
      PyApp::unlock();
      return;
   }

   checkRank ( array );
   std::vector<double> col;
   extractVector(array, col);

   if (m_type == "NTuple") {
      NTuple * nt = dynamic_cast<NTuple *>(m_dataSource);
      if (hasColumn(label)) {
         m_dataSource ->replaceColumn ( label, col );
      } else {
         nt->addColumn(label, col);
      }
      PyApp::unlock();
      return;
   }


#ifdef HAVE_ROOT
   RootNTuple * rnt = dynamic_cast < RootNTuple * > ( m_dataSource );
   if ( rnt != 0 ) {
     if ( hasColumn ( label ) ) {
       m_dataSource -> replaceColumn ( label, col );
     } else {
       rnt -> addColumn ( label, col );
     }
      PyApp::unlock();
     return;
   }
#endif // root
   PyApp::unlock();

   throw std::runtime_error("__setitem__ not supported for " + m_type);
#else
   throw std::runtime_error ("HippoDraw was not built with "
			     "numeric Python support" );
#endif
}

/** @bug @@@@@@ Should allow any type of DataSource if rank is acceptable.
 */
void PyDataSource::
saveColumnFromNumArray( unsigned int index,
                        boost::python::numeric::array array )
{
#ifdef HAVE_NUMARRAY
   if ( index < columns()) {

      if (m_type == "NumArrayTuple") {
         replaceColumn(index, array);
	 return;

      } else if (m_type == "NTuple") {
	 checkRank ( array );
         std::vector<double> col;
         extractVector ( array, col );
         NTuple * nt = dynamic_cast<NTuple *>(m_dataSource);
         nt->replaceColumn(index, col);
	 return;

      } else if (m_type == "ListTuple") {
         boost::python::list seq(array);
         ListTuple * nt = dynamic_cast<ListTuple *>(m_dataSource);
         nt->replaceColumn(index, seq);
	 return;
      }
#ifdef HAVE_CFITSIO
      else {
	FitsNTuple * fnt = dynamic_cast < FitsNTuple * > ( m_dataSource );
	if ( fnt != 0 ) {
	  std::vector<double> col;
	  extractVector ( array, col );
	  const std::vector < intptr_t > shape = num_util::shape ( array );
	  fnt -> replaceColumn ( index, col, shape );
	  return;
	}
      }
#endif
         throw std::runtime_error
            ("__setitem__ by index is not supported for " + m_type);
   } else {
     std::string what ( "DataArray:: Attempt to save column " );
      what += hippodraw::String::convert ( index );
      what += " with ";
      what += hippodraw::String::convert ( columns () );
      what += " columns in data source";
      throw std::runtime_error ( what );
   }
#else
   throw std::runtime_error ( "HippoDraw was not built with "
			      "numeric Python support" );
#endif
}


void
PyDataSource::
saveColumnFrom ( const std::string & label,
		 const std::vector < double > & array )
{
  if ( hasColumn ( label ) ) {
    m_dataSource -> replaceColumn ( label, array );
  }
  else {
    m_dataSource -> addColumn ( label, array );
  }
}

void
PyDataSource::
addRow ( const std::vector < double > & row )
{
  PyApp::lock ();
  try {
    m_dataSource -> addRow ( row );
  }
  catch ( const std::runtime_error & e ) {
    PyApp::unlock ();
    throw e;
  }
  PyApp::unlock ();
}

void
PyDataSource::
append ( const DataSource * source )
{
  PyApp::lock ();
  m_dataSource -> append ( source );
  PyApp::unlock ();
}
void
PyDataSource::
append ( const PyDataSource * source )
{
  m_dataSource -> append ( & source -> dataSource() );
}
