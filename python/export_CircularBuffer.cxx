/** @file

Part of Python extension module for hippodraw::CircularBuffer class

Copyright (C) 2000-2004, 2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: export_CircularBuffer.cxx,v 1.10 2006/03/31 22:33:19 pfkeb Exp $

*/
#ifdef _MSC_VER
// nonstandard extension used 'extern' before...
# pragma warning(disable:4231)  

// needs to have dll-interface used by client
# pragma warning(disable:4251)  

// non dll-interface struct
# pragma warning(disable:4275)  

// 'int' : forcing value to bool 'true' or 'false' (performance warning)
# pragma warning(disable:4800)  
#endif

// include first to avoid _POSIX_C_SOURCE warning.
#include <boost/python.hpp>

#include "datasrcs/CircularBuffer.h"

using std::vector;
using namespace boost::python;

namespace hippodraw {
namespace Python {

void
export_CircularBuffer()
{
  class_ < CircularBuffer, bases < NTuple > >
    ( "CircularBuffer",
      "A circular buffer in a NTuple that adds rows up to a defined number,\n"
      "then starts replacing the oldest rows.",
      init<> 
      ( "CircularBuffer ( None ) -> CircularBuffer\n"
	"CircularBuffer ( value ) -> CircularBuffer\n"
	"CircularBuffer ( string ) -> CircularBuffer\n"
	"CircularBuffer ( sequence ) -> CircularBuffer \n"
	"CircularBuffer ( CircularBuffer ) -> CircularBuffer\n"
	"\n"
	"There are five forms of construction of a CircularBuffer.   The\n"
	"first form creates an empty one.   The second from creates an empty\n"
	"one with `value' number of columns.   The third from creates an\n"
	"empty one the number of columns equal to the size of the sequence.\n"
	"The column labels are taken from the sequence.   The final form is\n"
	"the copy constructor." ) )

    .def ( init < int > () )

    .def ( init < const std::string & > () )

    .def ( init < const std::vector < std::string  > & > () )

    .def ( init < const CircularBuffer & > () )

    .add_property ( "rows", &CircularBuffer::rows )

    .add_property ( "columns", &CircularBuffer::columns )

    .def ( "addRow", &CircularBuffer::addRow,
	   "addRow ( sequence ) -> None\n"
	   "\n"
	   "Append a row at the end of the table until it is full,\n"
	   "then replace the oldest row." )

    .def ( "reserve", &CircularBuffer::reserve,
	   "reserve ( value ) -> None\n"
	   "\n"
	   "Sets the maximum size of the buffer" )

     .def ( "clear", &CircularBuffer::clear,
	    "clear ( None ) -> None\n"
	    "\n"
            "Clears the contents of the buffer" )

    ; 
}

} // namespace Python
} // namespace hippodraw
