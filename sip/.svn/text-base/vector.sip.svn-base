/* -*- mode:c++ -*- */

/** @file

This is the SIP interface definition for std::vector

Copyright (C)  2002-2004  The Board of Trustees of  
The Leland Stanford Junior University.  All Rights Reserved.

$Id: vector.sip,v 1.10 2005/06/17 18:08:00 pfkeb Exp $

@author Paul_Kunz@slac.stanford.edu

*/

%MappedType std::vector<double>
{
%TypeHeaderCode
#include <vector>
%End

%ConvertFromTypeCode
	// Handle no list.

	if (!sipCpp)
		return PyList_New(0);

	// Convert to a Python list of doubles

	PyObject *l;

	// Create the list.
        int size = static_cast < int > ( sipCpp -> size () );
	if ( ( l = PyList_New ( size ) ) == NULL)
		return NULL;

	// Get it.

	for ( unsigned int i = 0; i < sipCpp -> size(); ++i)
          if (PyList_SetItem(l,i,PyFloat_FromDouble((double)(*sipCpp)[i])) < 0)
		{
			Py_DECREF(l);

			return NULL;
		}

	return l;
%End

%ConvertToTypeCode
	// Convert a Python list of double to a vector<double> on the heap.
 
	if (sipIsErr == NULL)
		return PyList_Check(sipPy);

	if (sipPy == Py_None)
	{
		*sipCppPtr = NULL;

		return 0;
	}
        int size = PyList_GET_SIZE(sipPy);	
        vector<double> *qvl = new vector<double> ( size );
	PyErr_Clear();

	for (int i = 0; i < size; ++i)
	{
	  qvl -> operator [] (i) 
	       = ((double)PyFloat_AsDouble(PyList_GET_ITEM(sipPy,i)));
 
		if (PyErr_Occurred() != NULL)
		{
			delete qvl;
			*sipIsErr = 1;

			return 0;
		}
	}
 
	*sipCppPtr = qvl;
 
	return 1;
%End
};

%MappedType std::vector<bool>
{
%TypeHeaderCode
#include <vector>
%End

%ConvertFromTypeCode
	// Handle no list.

	if (!sipCpp)
		return PyList_New(0);

	// Convert to a Python list of ints

	PyObject *l;

	// Create the list.
	int size = static_cast < int > ( sipCpp -> size () );
	if ( ( l = PyList_New ( size ) ) == NULL)
		return NULL;

	// Get it.

	for ( unsigned int i = 0; i < sipCpp -> size(); ++i) {
          bool yes = sipCpp -> operator [] ( i );
	  long j = yes ? 1 : 0;
          if ( PyList_SetItem ( l, i, PyInt_FromLong ( j ) ) < 0 )
		{
			Py_DECREF(l);

			return NULL;
		}
	}
	return l;
%End

%ConvertToTypeCode
	// Convert a Python list of double to a vector<double> on the heap.
 
	if (sipIsErr == NULL)
		return PyList_Check(sipPy);

	if (sipPy == Py_None)
	{
		*sipCppPtr = NULL;

		return 0;
	}
        int size = PyList_GET_SIZE(sipPy);	
        vector < bool> *qvl = new vector < bool > ( size );
	PyErr_Clear();

	for (int i = 0; i < size; ++i)
	{
	  long j = ((long)PyInt_AsLong(PyList_GET_ITEM(sipPy,i)));
 
		if (PyErr_Occurred() != NULL)
		{
			delete qvl;
			*sipIsErr = 1;

			return 0;
		}
	  qvl -> operator [] (i) = ( j == 0 ) ? false : true;
	}
 
	*sipCppPtr = qvl;
 
	return 1;
%End
};

%MappedType std::vector<int>
{
%TypeHeaderCode
#include <vector>
%End

%ConvertFromTypeCode
	// Handle no list.

	if (!sipCpp)
		return PyList_New(0);

	// Convert to a Python list of ints

	PyObject *l;

	// Create the list.
	int size = static_cast < int > ( sipCpp -> size () );
	if ( ( l = PyList_New ( size ) ) == NULL)
		return NULL;

	// Get it.

	for ( unsigned int i = 0; i < sipCpp -> size(); ++i) {
          long j = sipCpp -> operator [] ( i );
          if ( PyList_SetItem ( l, i, PyInt_FromLong ( j ) ) < 0 )
		{
			Py_DECREF(l);

			return NULL;
		}
	}
	return l;
%End

%ConvertToTypeCode
	// Convert a Python list of int to a vector<int> on the heap.
 
	if (sipIsErr == NULL)
		return PyList_Check(sipPy);

	if (sipPy == Py_None)
	{
		*sipCppPtr = NULL;

		return 0;
	}
        int size = PyList_GET_SIZE(sipPy);	
        vector < int> *qvl = new vector < int > ( size );
	PyErr_Clear();

	for (int i = 0; i < size; ++i)
	{
	  long j = ((long)PyInt_AsLong(PyList_GET_ITEM(sipPy,i)));
 
		if (PyErr_Occurred() != NULL)
		{
			delete qvl;
			*sipIsErr = 1;

			return 0;
		}
		qvl -> operator [] (i) = j;
	}
 
	*sipCppPtr = qvl;
 
	return 1;
%End
};

%MappedType std::vector<std::string>
{
%TypeHeaderCode
#include <string>
#include <vector>

using std::string;
using std::vector;
%End

%ConvertFromTypeCode
	// Handle no list.

	if (!sipCpp)
		return PyList_New(0);

	// Convert to a Python list of doubles

	PyObject *l;

	// Create the list.
	int size = static_cast < int > ( sipCpp -> size () );
	if ( ( l = PyList_New( size ) ) == NULL )
		return NULL;

	// Get it.

	for ( unsigned int i = 0; i < sipCpp -> size(); ++i)
          if (PyList_SetItem(l, i,
	          PyString_FromString( (*sipCpp)[i].c_str())) < 0)
		{
			Py_DECREF(l);

			return NULL;
		}

	return l;
%End

%ConvertToTypeCode
	// Convert a Python list of string to a vector<string> on the heap.
 
	if (sipIsErr == NULL)
		return PyList_Check(sipPy);

	if (sipPy == Py_None)
	{
		*sipCppPtr = NULL;

		return 0;
	}

        vector<string> *qvl = new vector<string>;
	PyErr_Clear();

	for (int i = 0; i < PyList_GET_SIZE(sipPy); ++i)
	{
	  qvl -> push_back( PyString_AsString(PyList_GET_ITEM(sipPy,i)));
 
		if (PyErr_Occurred() != NULL)
		{
			delete qvl;
			*sipIsErr = 1;

			return 0;
		}
	}
 
	*sipCppPtr = qvl;
 
	return 1;
%End
};
