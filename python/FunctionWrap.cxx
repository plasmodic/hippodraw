/** @file

hippodraw::FunctionWrap class implementation and Python extension module.

Copyright (C) 2004-2006  The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: FunctionWrap.cxx,v 1.54 2007/07/02 22:45:09 pfkeb Exp $

*/

// include first to avoid _POSIX_C_SOURCE warning.
#include <boost/python.hpp>

#ifdef HAVE_CONFIG_H
// for boost defect
#include "config.h"
#endif


#include "FunctionWrap.h"
#ifndef BOOST_DEFECT

#include <stdexcept>

using std::string;
using std::vector;

using namespace boost::python;

namespace hippodraw {
  namespace Python {
    void
    export_FunctionBase ()
    {
      class_ < FunctionWrap, std::auto_ptr < FunctionWrap> >
	( "FunctionBase",
	  "The base class for raw functions.   FunctionBase objects need to\n"
	  "wrapped with Function objects to be displayed.   FunctionBase\n"
	  "can be derived from and cloned so they can be added to the\n"
	  "FunctionFactory.",
	  init < const FunctionBase & >
	  ( "FunctionBase () -> FunctionBase\n"
	    "FunctionBase ( FunctionBase ) -> FunctionBase\n"
	    "\n"
	    "Constructors of the FunctionBase object.\n" ) )

	.def ( init <> () )

	.def ( init < const FunctionWrap & > () )

	.def ( "initialize", &FunctionWrap::initialize,
	       "initialize () -> None\n"
	       "\n"
	       "Initializes the function parameter values and the names.\n"
	       "The number of parameters is taken from the number of names." )

	.def ( "name", &FunctionBase::name,
	       return_value_policy < copy_const_reference > (),
	       "name () -> string\n"
	       "\n"
	       "Returns the name of the function." )

	.def ( "setName", &FunctionWrap::setName,
	       "setName ( string ) -> None\n"
	       "\n"
	       "Sets the name of the function." )

	.def ( "parmNames",
	       &FunctionBase::parmNames,
	       return_value_policy < copy_const_reference > (),
	       "parmNames () -> sequence\n"
	       "\n"
	       "Returns the names of the parameters" )

	.def ( "setParmNames",
	       &FunctionWrap::setParmNames,
	       "setParmNames ( sequence ) -> None\n"
	       "\n"
	       "Sets the parameter names and re-sizes the parameters" )

	.def ( "setParameters", 
	       ( void ( FunctionBase::* ) // function pointer cast
	       ( const std::vector < double > & ) )  // function signature
	       &FunctionBase::setParameters,
	       "setParameters ( sequence ) -> None\n"
	       "\n"
	       "Sets the values of the parameters." )

	.def ( "getParameters", &FunctionBase::getParameters,
	       return_value_policy < copy_const_reference > (),
	       "getParameters () -> sequence\n"
	       "\n"
	       "Returns the current function parameter values." )

	;
    }
  } // namespace Python
} // namespace hippodraw

using namespace hippodraw;

FunctionWrap::
FunctionWrap ( )
  : FunctionBase ()
{
}

FunctionWrap::
FunctionWrap ( const FunctionBase & base )
  : FunctionBase ( base )//,
{
}

FunctionWrap::
FunctionWrap ( const FunctionWrap & wrap )
  : FunctionBase ( wrap )
{
}

FunctionWrap::
~FunctionWrap ()
{
#ifndef HAVE_OLD_PYTHON
  PyGILState_STATE state = PyGILState_Ensure ();

  extract<std::auto_ptr<FunctionWrap>&> x(get_owner(this));
  if ( x.check() ) x().release();

  PyGILState_Release ( state );
#endif
}

template <class T>
object
FunctionWrap::
get_owner(T* ) const // me
{
      // Use secret interface to get the Python object
      // that owns *this.  I guess I will have to make that
      // interface public. -- Dave Abrahams
      return
object ( handle<> ( borrowed ( detail::wrapper_base_::get_owner(*this))));
}

unsigned int
FunctionWrap::
dimensions () const
{
  unsigned int dims = 0;
#ifndef HAVE_OLD_PYTHON
  PyGILState_STATE state = PyGILState_Ensure ();

  if ( override member = this -> get_override ( "dimensions" ) ) {
    FunctionWrap * fw = const_cast < FunctionWrap * > ( this );
    object self = get_owner ( fw );
    dims = call_method < unsigned int > ( self.ptr(), "dimensions" );
  }
  else { // didn't override it
    dims = FunctionBase::dimensions ();
  }
  PyGILState_Release ( state );
#endif

  return dims;
}

bool
FunctionWrap::
hasDerivatives () const
{
  bool yes = false;

  if ( override member = this -> get_override ( "derivByParm" ) ) {
    yes = true;
  }

  return yes;
}

FunctionBase *
FunctionWrap::
clone () const
{
#ifndef HAVE_OLD_PYTHON
  PyGILState_STATE state = PyGILState_Ensure ();

  object py_result;

  if (override clone = this->get_override("clone")) {
     try {
	// The Python class author overrode clone; do
	// whatever she says
	py_result ( clone() );
     } catch ( error_already_set & ) {
	PyErr_Print ();
	handle_exception ();
	PyGILState_Release ( state );
        return 0;
     }
  } else {
     FunctionWrap * t = const_cast < FunctionWrap * > ( this );
     try {
	object self = get_owner ( t );
	// Find its most-derived Python class
	object my_class = self.attr("__class__");
	// call the default constructor
	py_result = my_class ( );
     } catch ( error_already_set & ) {
	PyErr_Print ();
	handle_exception ();
	PyGILState_Release ( state );
	return 0;
     }
  }

  FunctionWrap* result = extract<FunctionWrap*>(py_result);

  // Make the C++ result control the destiny of the Python result.
  result->invert_ownership = py_result;
  PyGILState_Release ( state );

  return result;

#else // old Python
  return 0;
#endif
}

/** @note We have this function here so that FunctionBase::setName can
    remain protected and yet the Boost.Python and SIP interfaces can
    call this public member function.
*/
void
FunctionWrap::
setName ( const std::string & name )
{
  FunctionBase::setName ( name.c_str() );
}

void
FunctionWrap::
setParmNames ( const std::vector < std::string > & names )
{
  m_parm_names = names;
  resize ();
}

/** @note This member function is only available with Python 2.3 or later.

    @bug Should attempt to call derivByParm to implement hasDerivatives()
 */
double
FunctionWrap::
derivByParm ( int i , double x ) const
{
#ifndef HAVE_OLD_PYTHON
  double value = 0.;
  PyGILState_STATE state = PyGILState_Ensure ();

  FunctionWrap * t = const_cast < FunctionWrap * > ( this );
  object self = get_owner ( t );

  try {
    value = call_method < double, int, double > 
      ( self.ptr(), "derivByParm", i, x );
  }  catch ( ... ) {
    value = 0.;
  }

  PyGILState_Release ( state );

  return value;
#else // old Python
  return 0;
#endif
}

/** @note This function is only available for Python 2.3 or later.
 */
void
FunctionWrap::
initialize ()
{
#ifndef HAVE_OLD_PYTHON
  PyGILState_STATE state = PyGILState_Ensure ();

  FunctionWrap * t = const_cast < FunctionWrap * > ( this );
  object self = get_owner ( t );
  call_method < void > ( self.ptr(), "initialize" );
  resize ();

  PyGILState_Release ( state );
#endif
}

/** @note This function is only available for Python 2.3 or later.
    @request Why is exception being thrown for logParabolic.py and are
    we handling the fix correctly
*/
double
FunctionWrap::
operator () ( double x ) const
{
  double value = 0;

#ifndef HAVE_OLD_PYTHON
  PyGILState_STATE state = PyGILState_Ensure ();
  FunctionWrap * t = const_cast < FunctionWrap * > ( this );
  object self = get_owner ( t );

  try {
    value = call_method < double, double > ( self.ptr(), "valueAt", x );
    PyGILState_Release ( state );
  }
  catch ( const error_already_set & ) {
    PyGILState_Release ( state );
//     handle_exception ();
  }    
#endif

  return value;
}

/** @note This function is only available for Python 2.3 or later.
 */
double
FunctionWrap::
operator () ( const std::vector < double > & x ) const
{
  double value = 0;

#ifndef HAVE_OLD_PYTHON
  PyGILState_STATE state = PyGILState_Ensure ();
  FunctionWrap * t = const_cast < FunctionWrap * > ( this );
  object self = get_owner ( t );
  unsigned int size = x.size();

  switch ( size ) {
  case 2 :
    value = call_method < double, double, double > 
      ( self.ptr(), "valueAt", x[0], x[1] );
    break;
  case 3 :
    value = call_method < double, double, double, double > 
      ( self.ptr(), "valueAt", x[0], x[1], x[2] );
    break;
  default:
    break;
  }
  PyGILState_Release ( state );
#endif

  return value;
}

void
FunctionWrap::
initialParameters ( const FunctionHelper * )
{
}

#endif // BOOST_DEFECT
